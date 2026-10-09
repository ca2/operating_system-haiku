#include "platform.h"
#include "shell.h"
#include "native_file_icon.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "aura/graphics/image/drawing.h"
#include "acme/graphics/image/pixmap.h"
#include <vector>
namespace windowing_haiku {
shell::shell()
{
   m_bGetFileImageByIconPath = false;
   m_bGetFileImageByFileTypeImage = false;
   m_bGetFileImageByFileImage = true;
}
bool shell::defer_get_file_image_by_file_image(_get_file_image_ &request)
{
   auto path = final_path(request);
   if (path.is_empty()) path = processed_path(request);
   if (path.is_empty()) return false;
   bool folder = request.m_imagekey.m_eattribute == e_file_attribute_directory
      || directory()->is(path);
   ::i32_array_base sizes;
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      sizes = m_iaSize;
   }
   std::vector<::image::image_pointer> icons;
   for (auto size : sizes)
   {
      if (size <= 0) return false;
      BBitmap native(BRect(0, 0, size - 1, size - 1), B_RGBA32);
      if (native_file_icon(path.c_str(), folder, native) != B_OK) return false;
      auto icon = image()->create_image({size, size}, draw2d_domain());
      {
         auto pixels = icon->map(::image::e_map_discard);
         auto indexes = pixels->color_indexes();
         for (int y = 0; y < size; ++y)
         {
            auto *source = static_cast<const unsigned char *>(native.Bits()) + y * native.BytesPerRow();
            auto *target = reinterpret_cast<unsigned char *>(pixels->data()) + y * pixels->scan();
            for (int x = 0; x < size; ++x, source += 4, target += 4)
            {
               // Native B_RGBA32 is straight BGRA; ca2 uses premultiplied pixels.
               unsigned alpha = source[3];
               target[indexes.red()] = (source[2] * alpha + 127) / 255;
               target[indexes.green()] = (source[1] * alpha + 127) / 255;
               target[indexes.blue()] = (source[0] * alpha + 127) / 255;
               target[indexes.opacity()] = alpha;
            }
         }
      }
      icons.push_back(icon);
   }
   if (icons.empty()) return false;
   request.m_iImage = _reserve_image(request.m_imagekey);
   if (request.m_iImage < 0) return false;
   for (::collection::index i = 0; i < sizes.size(); ++i)
   {
      ::image::image_source source(icons[i]);
      ::image::image_drawing_options options(::f64_rectangle(::i32_size(sizes[i], sizes[i])));
      set_image(request.m_iImage, sizes[i], {options, source}, draw2d_domain());
   }
   return true;
}
bool shell::_get_file_image(_get_file_image_ &request)
{
   request.m_iImage = I32_MINIMUM;
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      m_loadingKey = request.m_imagekey;
      m_iconLoading = true;
   }
   bool ok = false;
   try
   {
      ok = ::user::shell::_get_file_image(request);
   }
   catch (const ::exception &e)
   {
      warning() << "Haiku shell icon failed for " << request.m_imagekey.m_strPath << ": " << e.get_message();
   }
   catch (...) {}
   if (!ok) request.m_iImage = I32_MINIMUM;
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      m_iconLoading = false;
   }
   return ok;
}
::i32 shell::get_file_image(const image_key &key)
{
   {
      synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      // _reserve_image allocates cache slots before set_image fills the atlas.
      // Keep callers subscribed to the completion redraw until all sizes exist.
      if (m_iconLoading && key == m_loadingKey) return I32_MINIMUM;
   }
   return ::user::shell::get_file_image(key);
}
bool shell::get_image_by_file_extension(_get_file_image_ &request)
{
   return defer_get_file_image_by_file_image(request);
}
shell::enum_folder shell::get_folder_type(::particle *, const ::scoped_string &path)
{
   if (directory()->is(path)) return e_folder_file_system;
   ::file::path filepath(path);
   if (filepath.final_extension().case_insensitive_order("zip") == 0) return e_folder_zip;
   return e_folder_none;
}
}
