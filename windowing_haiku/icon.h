#pragma once
#include "_.h"
#include "aura/windowing/icon.h"
#include <Bitmap.h>
#include <map>
#include <memory>
#include <mutex>

namespace windowing_haiku
{
   class CLASS_DECL_WINDOWING_HAIKU icon : virtual public ::windowing::icon
   {
   public:
      icon();
      ~icon() override;
      void set_file(const ::payload &file) override;
      void set_app_tray_icon(const ::scoped_string &appId) override;
      ::image::image_pointer get_image(const ::i32_size &size) override;
      void get_sizes(::i32_size_array &sizes) override;
      void *get_os_data(const ::i32_size &size) const override;
      ::pointer<::innate_ui::icon> innate_ui_icon(const ::i32_size &size) override;

   private:
      mutable std::recursive_mutex m_mutex;
      ::image::image_pointer m_source;
      mutable std::map<std::pair<int, int>, std::unique_ptr<BBitmap>> m_bitmaps;
   };
}
