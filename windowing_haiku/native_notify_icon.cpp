#include "native_notify_icon.h"
#include <AppFileInfo.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <File.h>
#include <Looper.h>
#include <MenuItem.h>
#include <Message.h>
#include <Messenger.h>
#include <Mime.h>
#include <OS.h>
#include <PopUpMenu.h>
#include <View.h>
#include <Window.h>
#include <dlfcn.h>
#include <map>
#include <memory>
#include <unistd.h>

static const char * signature = "application/x-vnd.ca2-deskbar";
static const uint32 commandMessage = 'c2nc', bitmapMessage = 'c2nb';

class Ca2DeskbarView : public BView
{
public:
   BMessenger owner;
   team_id team;
   std::unique_ptr<BBitmap> bitmap;
   std::unique_ptr<BPopUpMenu> menu;
   Ca2DeskbarView(const char * name, const BBitmap * icon, BMessenger target)
      : BView(BRect(0,0,15,15),name,B_FOLLOW_NONE,B_WILL_DRAW | B_PULSE_NEEDED), owner(target), team(getpid()),
        bitmap(icon ? new BBitmap(icon) : nullptr), menu(new BPopUpMenu(name)) {}
   explicit Ca2DeskbarView(BMessage * archive) : BView(archive)
   {
      archive->FindMessenger("owner", &owner);
      archive->FindInt32("team", &team);
      BMessage data;
      if (archive->FindMessage("bitmap", &data) == B_OK) bitmap.reset(new BBitmap(&data));
      if (archive->FindMessage("menu", &data) == B_OK) menu.reset(new BPopUpMenu(&data));
   }
   static BArchivable * Instantiate(BMessage * archive);
   status_t Archive(BMessage * archive, bool deep = true) const override
   {
      auto status = BView::Archive(archive, deep);
      archive->AddString("class", "Ca2DeskbarView");
      archive->AddString("add_on", signature);
      archive->AddMessenger("owner", owner);
      archive->AddInt32("team", team);
      BMessage data;
      if (bitmap && bitmap->Archive(&data) == B_OK) archive->AddMessage("bitmap", &data);
      data.MakeEmpty();
      if (menu && menu->Archive(&data) == B_OK) archive->AddMessage("menu", &data);
      return status;
   }
   void AttachedToWindow() override
   {
      SetViewColor(B_TRANSPARENT_COLOR);
      if (Parent()) SetLowColor(Parent()->ViewColor());
      if (menu) menu->SetTargetForItems(owner);
   }
   void Draw(BRect) override
   {
      SetDrawingMode(B_OP_ALPHA);
      if (bitmap && bitmap->InitCheck() == B_OK) DrawBitmap(bitmap.get(), bitmap->Bounds(), Bounds());
   }
   void MouseDown(BPoint point) override
   {
      int32 buttons = B_PRIMARY_MOUSE_BUTTON;
      if (Window()->CurrentMessage()) Window()->CurrentMessage()->FindInt32("buttons", &buttons);
      if (buttons & B_SECONDARY_MOUSE_BUTTON)
      {
         if (menu) { ConvertToScreen(&point); menu->Go(point, true, false, true); }
      }
      else
      {
         BMessage message(commandMessage); message.AddString("command", "notify_icon_topic"); owner.SendMessage(&message);
      }
   }
   void MessageReceived(BMessage * message) override
   {
      if (message->what == bitmapMessage)
      {
         BMessage data;
         if (message->FindMessage("bitmap", &data) == B_OK) { bitmap.reset(new BBitmap(&data)); Invalidate(); }
      }
      else BView::MessageReceived(message);
   }
   void Pulse() override
   {
      team_info info;
      if (get_team_info(team, &info) != B_OK) BDeskbar().RemoveItem(Name());
   }
};
BArchivable * Ca2DeskbarView::Instantiate(BMessage * archive)
{
   return validate_instantiation(archive,"Ca2DeskbarView") ? new Ca2DeskbarView(archive) : nullptr;
}

class NotifyListener : public BLooper
{
public:
   void (*callback)(void *, const char *);
   void * context;
   NotifyListener(void (*function)(void *, const char *), void * data) : BLooper("ca2 Deskbar callbacks"), callback(function), context(data) {}
   void MessageReceived(BMessage * message) override
   {
      const char * command;
      if (message->what == commandMessage && message->FindString("command", &command) == B_OK) callback(context, command);
      else BLooper::MessageReceived(message);
   }
};
struct notify_handle
{
   NotifyListener * listener;
   std::unique_ptr<Ca2DeskbarView> view;
   std::map<int, BMenu *> menus;
   int32 id = -1;
};
extern "C" void * haiku_notify_create(const char * name, const void * bitmap, void (*callback)(void *, const char *), void * context)
{
   auto handle = new notify_handle;
   handle->listener = new NotifyListener(callback, context);
   handle->listener->Run();
   handle->view.reset(new Ca2DeskbarView(name, static_cast<const BBitmap *>(bitmap), BMessenger(handle->listener)));
   handle->menus[0] = handle->view->menu.get();
   return handle;
}
extern "C" void haiku_notify_menu(void * data, int parent, int id, const char * label, const char * command, bool enabled, bool checked)
{
   auto h = static_cast<notify_handle *>(data);
   auto menu = h->menus[parent];
   if (!menu) return;
   if (!label || !*label) { menu->AddSeparatorItem(); return; }
   if (!command || !*command)
   {
      auto child = new BMenu(label); menu->AddItem(child); h->menus[id] = child;
   }
   else
   {
      auto message = new BMessage(commandMessage); message->AddString("command", command);
      auto item = new BMenuItem(label, message); item->SetEnabled(enabled); item->SetMarked(checked); menu->AddItem(item);
   }
}
extern "C" int haiku_notify_install(void * data)
{
   auto h = static_cast<notify_handle *>(data);
   Dl_info info;
   if (!dladdr(reinterpret_cast<void *>(&haiku_notify_install), &info)) return B_ENTRY_NOT_FOUND;
   BFile file(info.dli_fname, B_READ_WRITE);
   BAppFileInfo appInfo(&file);
   auto status = appInfo.SetSignature(signature);
   if (status != B_OK) return status;
   update_mime_info(info.dli_fname, false, true, false);
   create_app_meta_mime(info.dli_fname, false, true, false);
   BDeskbar deskbar;
   if (deskbar.HasItem(h->view->Name())) deskbar.RemoveItem(h->view->Name());
   return deskbar.AddItem(h->view.get(), &h->id);
}
extern "C" bool haiku_notify_exists(void * data)
{
   auto h = static_cast<notify_handle *>(data);
   return h && h->id >= 0 && BDeskbar().HasItem(h->id);
}
extern "C" void haiku_notify_bitmap(void * data, const void * bitmap)
{
   auto h = static_cast<notify_handle *>(data);
   if (!bitmap) return;
   h->view->bitmap.reset(new BBitmap(static_cast<const BBitmap *>(bitmap)));
   BDeskbar deskbar;
   if (h->id >= 0) deskbar.RemoveItem(h->id);
   h->id = -1;
   deskbar.AddItem(h->view.get(), &h->id);
}
extern "C" void haiku_notify_destroy(void * data)
{
   auto h = static_cast<notify_handle *>(data);
   if (!h) return;
   if (h->id >= 0) BDeskbar().RemoveItem(h->id);
   if (h->listener->Lock()) h->listener->Quit();
   delete h;
}
