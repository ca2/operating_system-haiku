#include <Application.h>
#include <AppFileInfo.h>
#include <Bitmap.h>
#include <File.h>
#include <IconUtils.h>
#include <TranslationUtils.h>
#include <View.h>
#include <cstdio>
#include <cstring>
#include <memory>

static status_t set_icon(BAppFileInfo &info, BBitmap &source, icon_size size)
{
   BBitmap scaled(BRect(0, 0, size - 1, size - 1), B_BITMAP_ACCEPTS_VIEWS, B_RGBA32);
   if (scaled.InitCheck() != B_OK) return scaled.InitCheck();
   if (!scaled.Lock()) return B_ERROR;
   std::memset(scaled.Bits(), 0, scaled.BitsLength());
   auto view = new BView(scaled.Bounds(), "icon", B_FOLLOW_NONE, B_WILL_DRAW);
   scaled.AddChild(view);
   view->SetDrawingMode(B_OP_ALPHA);
   view->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_COMPOSITE);
   view->DrawBitmap(&source, source.Bounds(), scaled.Bounds(), B_FILTER_BITMAP_BILINEAR);
   view->Sync();
   scaled.Unlock();
   BBitmap palette(scaled.Bounds(), B_CMAP8);
   if (palette.InitCheck() != B_OK) return palette.InitCheck();
   auto status = BIconUtils::ConvertToCMAP8(&scaled, &palette);
   if (status != B_OK) return status;
   return info.SetIcon(&palette, size, true);
}

int main(int argc, char **argv)
{
   if (argc != 4)
   {
      std::fprintf(stderr, "Usage: %s executable icon.png application-signature\n", argv[0]);
      return 1;
   }
   BApplication application("application/x-vnd.ca2.app-metadata");
   std::unique_ptr<BBitmap> icon(BTranslationUtils::GetBitmap(argv[2]));
   BFile file(argv[1], B_READ_WRITE);
   BAppFileInfo info(&file);
   auto status = file.InitCheck();
   if (status == B_OK) status = info.InitCheck();
   if (status == B_OK && !icon) status = B_BAD_DATA;
   if (status == B_OK)
   {
      info.SetInfoLocation(B_USE_BOTH_LOCATIONS);
      status = info.SetType(B_APP_MIME_TYPE);
   }
   if (status == B_OK) status = info.SetSignature(argv[3]);
   if (status == B_OK) status = set_icon(info, *icon, B_MINI_ICON);
   if (status == B_OK) status = set_icon(info, *icon, B_LARGE_ICON);
   if (status != B_OK)
   {
      std::fprintf(stderr, "Cannot assign application icon to %s: %s\n", argv[1], std::strerror(status));
      return 1;
   }
   return 0;
}
