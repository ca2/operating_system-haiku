#pragma once
#include <Bitmap.h>
namespace windowing_haiku {
status_t native_file_icon(const char *path, bool directory, BBitmap &bitmap);
}
