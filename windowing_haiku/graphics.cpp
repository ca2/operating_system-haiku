// Created by camilo on 2026-10-08 02:16 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "graphics.h"
#include "window.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "aura/graphics/graphics/buffer_item.h"
#include "aura/graphics/image/image.h"


namespace windowing_haiku
{


   void graphics::update_screen()
   {
      if (m_pwindow)
      {
         m_pwindow->window_update_screen();
      }
   }


   void graphics::on_update_screen(::graphics::buffer_item *pbufferitem)
   {


      ::cast < window > pwindow = m_pwindow;
      if (!pwindow || !pwindow->m_native || !pbufferitem || !pbufferitem->m_pimageBufferItem)
         return;
      synchronous_lock imageLock(pbufferitem->m_pmutex, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
      auto point = pbufferitem->m_pointBufferItem;
      auto size = pbufferitem->m_sizeBufferItem;
      int x = 0, y = 0, w = 0, h = 0;
      haiku_window_bounds(pwindow->m_native, &x, &y, &w, &h);
      if (x != point.x || y != point.y || w != size.cx || h != size.cy)
         haiku_window_frame(pwindow->m_native, point.x, point.y, size.cx, size.cy);
      auto ppixmapImageBufferItem = pbufferitem->m_pimageBufferItem->map(::image::e_map_load, {point, size});
      if (!ppixmapImageBufferItem || !ppixmapImageBufferItem->m_pimage32)
         return;
      auto source = ppixmapImageBufferItem->m_point; // Present the window region of the fullscreen bitmap.
      source.x = constrained(source.x, 0, ppixmapImageBufferItem->m_sizeRaw.cx);
      source.y = constrained(source.y, 0, ppixmapImageBufferItem->m_sizeRaw.cy);
      auto width = minimum(size.cx, ppixmapImageBufferItem->m_sizeRaw.cx - source.x);
      auto height = minimum(size.cy, ppixmapImageBufferItem->m_sizeRaw.cy - source.y);
      if (width <= 0 || height <= 0)
         return;
      auto *data = ppixmapImageBufferItem->m_pimage32;

      informationf("buffer_item map: %d %d %d %d", point.x, point.y, size.cx, size.cy);
      informationf("haiku_window_present: %d %d %d %d", source.x, source.y, width, height);
      haiku_window_present(pwindow->m_native, data, width, height, ppixmapImageBufferItem->m_iScan);
   }


}
