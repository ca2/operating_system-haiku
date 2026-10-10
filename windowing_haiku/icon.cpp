#include "platform.h"
#include "icon.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "acme/graphics/image/pixmap.h"
#include "apex/innate_ui/icon.h"
#include "acme/platform/system.h"
#include <algorithm>

namespace windowing_haiku
{
   icon::icon()
   {
      m_sizea = {{16, 16}, {24, 24}, {32, 32}, {48, 48}, {128, 128}};
   }

   icon::~icon() = default;

   void icon::set_file(const ::payload &file)
   {
      std::lock_guard<std::recursive_mutex> lock(m_mutex);
      ::windowing::icon::set_file(file);
      m_source.release();
      m_bitmaps.clear();
   }

   void icon::set_app_tray_icon(const ::scoped_string &appId)
   {
      ::windowing::icon::set_app_tray_icon(appId);
      set_matter("main/icon.png");
   }

   ::image::image_pointer icon::get_image(const ::i32_size &size)
   {
      if (size.is_empty()) return {};
      std::lock_guard<std::recursive_mutex> lock(m_mutex);
      if (m_source.nok()) m_source = image()->load_image(m_payload);
      if (m_source.nok()) return {};
      return m_source->get_image(size);
   }

   void icon::get_sizes(::i32_size_array &sizes)
   {
      sizes = m_sizea;
   }

   void *icon::get_os_data(const ::i32_size &size) const
   {
      if (size.is_empty()) return nullptr;
      std::lock_guard<std::recursive_mutex> lock(m_mutex);
      auto key = std::make_pair(size.cx, size.cy);
      auto found = m_bitmaps.find(key);
      if (found != m_bitmaps.end()) return found->second.get();
      auto image = const_cast<icon *>(this)->get_image(size);
      if (image.nok()) return nullptr;
      auto bitmap = std::make_unique<BBitmap>(BRect(0, 0, size.cx - 1, size.cy - 1), B_RGBA32);
      if (bitmap->InitCheck() != B_OK) return nullptr;
      auto pixels = image->map();
      auto indexes = pixels->color_indexes();
      for (int y = 0; y < size.cy; ++y)
      {
         auto source = reinterpret_cast<const unsigned char *>(pixels->data()) + y * pixels->scan();
         auto target = static_cast<unsigned char *>(bitmap->Bits()) + y * bitmap->BytesPerRow();
         for (int x = 0; x < size.cx; ++x, source += 4, target += 4)
         {
            unsigned alpha = source[indexes.opacity()];
            // ca2 pixels are premultiplied; native B_RGBA32 uses straight BGRA.
            target[0] = alpha ? std::min(255u, (source[indexes.blue()] * 255u + alpha / 2) / alpha) : 0;
            target[1] = alpha ? std::min(255u, (source[indexes.green()] * 255u + alpha / 2) / alpha) : 0;
            target[2] = alpha ? std::min(255u, (source[indexes.red()] * 255u + alpha / 2) / alpha) : 0;
            target[3] = alpha;
         }
      }
      auto native = bitmap.get();
      m_bitmaps.emplace(key, std::move(bitmap));
      return native;
   }

   ::pointer<::innate_ui::icon> icon::innate_ui_icon(const ::i32_size &size)
   {
      std::lock_guard<std::recursive_mutex> lock(m_mutex);
      system()->defer_innate_ui();
      auto icon = createø<::innate_ui::icon>();
      icon->create(m_payload, size);
      return icon;
   }
}
