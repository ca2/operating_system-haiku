#include "native_file_icon.h"
#include <Node.h>
#include <NodeInfo.h>
#include <MimeType.h>
#include <Mime.h>
#include <cstring>
namespace windowing_haiku {
status_t native_file_icon(const char *path, bool directory, BBitmap &bitmap)
{
   if (bitmap.InitCheck() != B_OK) return bitmap.InitCheck();
   memset(bitmap.Bits(), 0, bitmap.BitsLength());
   auto size = static_cast<icon_size>(bitmap.Bounds().IntegerWidth() + 1);
   BNode node(path);
   if (node.InitCheck() == B_OK)
   {
      BNodeInfo info(&node);
      auto status = info.GetTrackerIcon(&bitmap, size);
      if (status == B_OK) return status;
   }
   // Use the MIME database for missing/untyped entries, without writing any
   // MIME attributes or changing the user's file associations.
   BMimeType type;
   if (!directory && BMimeType::GuessMimeType(path, &type) == B_OK
      && type.GetIcon(&bitmap, size) == B_OK) return B_OK;
   type.SetTo(directory ? "application/x-vnd.Be-directory" : B_FILE_MIME_TYPE);
   return type.GetIcon(&bitmap, size);
}
}
