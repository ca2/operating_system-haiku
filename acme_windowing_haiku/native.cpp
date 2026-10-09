#include "native.h"
#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Message.h>
#include <Screen.h>
#include <OS.h>
#include <map>
#include <mutex>
#include <memory>
#include <new>
#include <cstring>
namespace {
constexpr uint32 kJob='c2jb',kEvent='c2ev';
struct job { void (*call)(void *);void *context;void (*dispose)(void *);sem_id done=-1; };
struct state;
std::mutex registry_mutex;
int64 next_id=1;
std::map<int64,state *> windows;
std::map<int64,std::unique_ptr<job>> jobs;
struct state { int64 id;BWindow *window=nullptr;void *owner;void (*event)(void *,const haiku_window_event *); };
class application : public BApplication {
public:
 explicit application(const char *signature):BApplication(signature){}
 void MessageReceived(BMessage *message) override {
  int64 id=0;if(message->FindInt64("id",&id)!=B_OK){BApplication::MessageReceived(message);return;}
  if(message->what==kJob){
   std::unique_ptr<job> work;{std::lock_guard<std::mutex> guard(registry_mutex);auto i=jobs.find(id);if(i==jobs.end())return;work=std::move(i->second);jobs.erase(i);}
   work->call(work->context);work->dispose(work->context);if(work->done>=0)release_sem(work->done);
  }else if(message->what==kEvent){
   state *s=nullptr;{std::lock_guard<std::mutex> guard(registry_mutex);auto i=windows.find(id);if(i!=windows.end())s=i->second;}
   const void *data=nullptr;ssize_t size=0;if(s && message->FindData("event",B_RAW_TYPE,&data,&size)==B_OK && size==sizeof(haiku_window_event))s->event(s->owner,static_cast<const haiku_window_event *>(data));
  }else BApplication::MessageReceived(message);
 }
};
application *app=nullptr;
void notify(state *s,const haiku_window_event &event){if(!app)return;BMessage m(kEvent);m.AddInt64("id",s->id);m.AddData("event",B_RAW_TYPE,&event,sizeof(event));app->PostMessage(&m);}
class view : public BView {
public:
 state *owner;
 std::unique_ptr<BBitmap> bitmap;
 view(state *s,BRect r):BView(r,"ca2-client",B_FOLLOW_ALL,B_WILL_DRAW|B_FRAME_EVENTS),owner(s){SetViewColor(245,245,245);}
 void AttachedToWindow() override {BView::AttachedToWindow();SetEventMask(B_POINTER_EVENTS,B_NO_POINTER_HISTORY);}
 void mouse_event(int kind,BPoint p){auto screen=ConvertToScreen(p);notify(owner,{kind,int(p.x),int(p.y),int(screen.x),int(screen.y)});}
 void MouseDown(BPoint p) override {SetMouseEventMask(B_POINTER_EVENTS,B_LOCK_WINDOW_FOCUS);mouse_event(4,p);}
 void MouseUp(BPoint p) override {mouse_event(5,p);}
 void MouseMoved(BPoint p,uint32,const BMessage *) override {mouse_event(6,p);}
 void Draw(BRect) override {if(bitmap){SetDrawingMode(B_OP_ALPHA);SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);DrawBitmap(bitmap.get(),BPoint(0,0));}}
};
class window : public BWindow {
public:
 state *owner;view *client;
 window(state *s,const char *title,int x,int y,int w,int h):BWindow(BRect(x,y,x+w-1,y+h-1),title,B_NO_BORDER_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,0),owner(s),client(new view(s,Bounds())){AddChild(client);}
 void WindowActivated(bool active) override {BWindow::WindowActivated(active);notify(owner,{7,active?1:0});}
 bool QuitRequested() override {notify(owner,{3});return false;}
 void FrameResized(float w,float h) override {notify(owner,{1,0,0,int(w)+1,int(h)+1});client->Invalidate();}
 void FrameMoved(BPoint p) override {notify(owner,{2,int(p.x),int(p.y)});}
};
}
extern "C" int haiku_app_initialize(const char *signature){if(app)return 1;if(be_app)return 0;app=new(std::nothrow) application(signature);if(!app)return 0;if(app->InitCheck()!=B_OK){delete app;app=nullptr;return 0;}return 1;}
extern "C" int haiku_app_is_main_thread(){return app && find_thread(nullptr)==app->Thread();}
extern "C" void haiku_app_run(){if(app)app->Run();}
extern "C" void haiku_app_quit(){if(app)app->PostMessage(B_QUIT_REQUESTED);}
extern "C" int haiku_app_post(void (*call)(void *),void *context,void (*dispose)(void *),int wait){
 if(!app){dispose(context);return 0;}if(haiku_app_is_main_thread() && wait){call(context);dispose(context);return 1;}
 auto work=std::make_unique<job>();work->call=call;work->context=context;work->dispose=dispose;sem_id done=wait?create_sem(0,"ca2-main-send"):-1;
 if(wait && done<0){dispose(context);return 0;}work->done=done;int64 id;
 {std::lock_guard<std::mutex> guard(registry_mutex);id=next_id++;jobs[id]=std::move(work);}
 BMessage message(kJob);message.AddInt64("id",id);
 if(app->PostMessage(&message)!=B_OK){std::lock_guard<std::mutex> guard(registry_mutex);jobs.erase(id);dispose(context);if(done>=0)delete_sem(done);return 0;}
 if(done>=0){status_t result;do{result=acquire_sem(done);}while(result==B_INTERRUPTED);delete_sem(done);return result==B_OK;}return 1;
}
extern "C" void *haiku_window_create(void *owner,void (*event)(void *,const haiku_window_event *),const char *title,int x,int y,int w,int h){
 if(!app || w<=0 || h<=0)return nullptr;auto *s=new(std::nothrow) state;if(!s)return nullptr;s->owner=owner;s->event=event;
 {std::lock_guard<std::mutex> guard(registry_mutex);s->id=next_id++;windows[s->id]=s;}
 s->window=new window(s,title,x,y,w,h);return s;
}
extern "C" void haiku_window_destroy(void *p){auto *s=static_cast<state *>(p);if(!s)return;{std::lock_guard<std::mutex> guard(registry_mutex);windows.erase(s->id);}if(s->window->Lock())s->window->Quit();delete s;
 bool empty;{std::lock_guard<std::mutex> guard(registry_mutex);empty=windows.empty();}if(empty)haiku_app_quit();}
extern "C" void haiku_window_show(void *p,int show){auto *s=static_cast<state *>(p);if(s && s->window->Lock()){if(show){if(s->window->IsHidden())s->window->Show();}else{if(!s->window->IsHidden())s->window->Hide();}s->window->Unlock();}}
extern "C" void haiku_window_title(void *p,const char *title){auto *s=static_cast<state *>(p);if(s && s->window->Lock()){s->window->SetTitle(title);s->window->Unlock();}}
extern "C" void haiku_window_frame(void *p,int x,int y,int w,int h){auto *s=static_cast<state *>(p);if(s && s->window->Lock()){s->window->MoveTo(x,y);s->window->ResizeTo(w-1,h-1);s->window->Unlock();}}
extern "C" void haiku_window_activate(void *p){auto *s=static_cast<state *>(p);if(s && s->window->Lock()){s->window->Activate();s->window->Unlock();}}
extern "C" void haiku_window_bounds(void *p,int *x,int *y,int *w,int *h){auto *s=static_cast<state *>(p);if(s && s->window->Lock()){auto f=s->window->Frame();*x=int(f.left);*y=int(f.top);*w=f.IntegerWidth()+1;*h=f.IntegerHeight()+1;s->window->Unlock();}}
extern "C" void haiku_window_present(void *p,const void *data,int w,int h,int stride){
 auto *s=static_cast<state *>(p);if(!s || !data || w<=0 || h<=0 || stride<w*4)return;
 auto bitmap=std::make_unique<BBitmap>(BRect(0,0,w-1,h-1),B_RGBA32);if(bitmap->InitCheck()!=B_OK)return;
 for(int y=0;y<h;y++)for(int x=0;x<w;x++){const auto *a=static_cast<const uint8_t *>(data)+y*stride+x*4;auto *b=static_cast<uint8_t *>(bitmap->Bits())+y*bitmap->BytesPerRow()+x*4;b[3]=a[3];for(int c=0;c<3;c++){unsigned v=a[3]?(unsigned(a[c])*255+a[3]/2)/a[3]:0;b[c]=v>255?255:v;}}
 if(s->window->Lock()){auto *v=static_cast<window *>(s->window)->client;v->bitmap=std::move(bitmap);v->Invalidate();s->window->Unlock();}
}
extern "C" void haiku_screen_bounds(int *x,int *y,int *w,int *h){BScreen screen;auto f=screen.Frame();*x=int(f.left);*y=int(f.top);*w=f.IntegerWidth()+1;*h=f.IntegerHeight()+1;}
extern "C" void haiku_mouse_position(int *x,int *y){*x=0;*y=0;state *s=nullptr;{std::lock_guard<std::mutex> guard(registry_mutex);if(!windows.empty())s=windows.begin()->second;}if(s && s->window->Lock()){auto *v=s->window->ChildAt(0);BPoint p;uint32 buttons;v->GetMouse(&p,&buttons,false);v->ConvertToScreen(&p);*x=int(p.x);*y=int(p.y);s->window->Unlock();}}
#include <Deskbar.h>
extern "C" int haiku_screen_info(int index,int *bounds,int *workspace){
 if(index<0 || !bounds || !workspace)return 0;
 BScreen screen;if(!screen.IsValid())return 0;
 for(int i=0;i<index;i++)if(screen.SetToNext()!=B_OK)return 0;
 auto frame=screen.Frame();auto work=frame;
 BDeskbar deskbar;
 if(deskbar.IsRunning() && !deskbar.IsAutoHide()){
  auto bar=deskbar.Frame();
  if(frame.Intersects(bar)){
   switch(deskbar.Location()){
    case B_DESKBAR_TOP:work.top=bar.bottom+1;break;
    case B_DESKBAR_BOTTOM:work.bottom=bar.top-1;break;
    case B_DESKBAR_LEFT_TOP:case B_DESKBAR_LEFT_BOTTOM:work.left=bar.right+1;break;
    case B_DESKBAR_RIGHT_TOP:case B_DESKBAR_RIGHT_BOTTOM:work.right=bar.left-1;break;
   }
  }
 }
 bounds[0]=int(frame.left);bounds[1]=int(frame.top);bounds[2]=int(frame.right)+1;bounds[3]=int(frame.bottom)+1;
 workspace[0]=int(work.left);workspace[1]=int(work.top);workspace[2]=int(work.right)+1;workspace[3]=int(work.bottom)+1;
 return 1;
}

extern "C" int haiku_window_is_active(void *p){auto *s=static_cast<state *>(p);if(!s || !s->window->Lock())return 0;bool active=s->window->IsActive();s->window->Unlock();return active?1:0;}
