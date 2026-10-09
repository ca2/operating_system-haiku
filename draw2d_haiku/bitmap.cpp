// Created by camilo on 2026-10-08 01:39 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "acme/graphics/image/pixmap.h"
#include "bitmap.h"
#include "aura/graphics/image/image.h"
#include <cmath>

namespace draw2d_haiku
{

   bitmap::bitmap()
   {


   }

   bitmap::~bitmap() { destroy(); }

   void bitmap::destroy()
   {
      delete m_pbbitmap;
      m_pbbitmap = nullptr;
      m_pbview = nullptr;
      m_iSavedState = 0;
      m_btransforma.clear();
      m_bregionaClip.clear();
      ::draw2d::bitmap::destroy();
   }

   void bitmap::create_bitmap(::draw2d::graphics *g, const ::i32_size &s) { create_bitmap(g, s, nullptr); }

   void bitmap::create_bitmap(::draw2d::graphics *, const ::i32_size &s, ::pixmap *pixels)
   {
      if (s.cx <= 0 || s.cy <= 0 || s.cx > 32767 || s.cy > 32767)
         throw ::exception(error_bad_argument);
      // A mapped image can reference this bitmap's CPU buffer. Snapshot its
      // pixels before destroy/reallocation/clearing can invalidate that source.
      ::memory saved;
      bool aliasesBuffer = pixels && pixels->m_pimage32Raw
         && pixels->m_pimage32Raw == reinterpret_cast<::image32_t *>(m_memoryDraw2dBitmap.data());
      ::i32_size copySize;
      ::i32 copyScan = 0;
      if (pixels && pixels->m_pimage32Raw && pixels->m_iScan > 0)
      {
         copySize = {minimum(s.cx, minimum(pixels->m_sizeRaw.cx, pixels->m_iScan / 4)),
                     minimum(s.cy, pixels->m_sizeRaw.cy)};
         if (copySize.cx > 0 && copySize.cy > 0)
         {
            copyScan = copySize.cx * 4;
            saved.set_size((::memsize)copyScan * copySize.cy);
            for (int y = 0; y < copySize.cy; ++y)
            {
               auto sourceY = pixels->m_bTopLeft ? y : pixels->m_sizeRaw.cy - 1 - y;
               auto *source = reinterpret_cast<const ::u8 *>(pixels->m_pimage32Raw)
                  + (::memsize)sourceY * pixels->m_iScan;
               memory_copy(saved.data() + (::memsize)y * copyScan, source, copyScan);
            }
         }
      }
      destroy();
      m_pbbitmap = new BBitmap(BRect(0, 0, s.cx - 1, s.cy - 1), B_BITMAP_ACCEPTS_VIEWS, B_RGBA32);
      if (m_pbbitmap->InitCheck() != B_OK || !m_pbbitmap->Lock())
         throw ::exception(error_failed);
      memory_set(m_pbbitmap->Bits(), 0, m_pbbitmap->BitsLength());
      m_pbview = new BView(m_pbbitmap->Bounds(), "ca2-offscreen", B_FOLLOW_NONE, B_WILL_DRAW);
      m_pbbitmap->AddChild(m_pbview);
      m_pbbitmap->Unlock();
      m_size = s;
      m_iStride = s.cx * 4;
      m_memoryDraw2dBitmap.set_size((::memsize)m_iStride * s.cy);
      memory_set(m_memoryDraw2dBitmap.data(), 0, m_memoryDraw2dBitmap.size());
      if (saved.size() > 0)
         write_pixels(copySize, {}, reinterpret_cast<const ::image32_t *>(saved.data()), copyScan, true);
      if (aliasesBuffer)
      {
         // Keep the image's borrowed pixmap valid for later acquisitions too.
         pixels->m_memoryPixmap.reference_data(m_memoryDraw2dBitmap.data(), m_memoryDraw2dBitmap.size());
         pixels->m_pimage32Raw = reinterpret_cast<::image32_t *>(m_memoryDraw2dBitmap.data());
         pixels->m_iScan = m_iStride;
         pixels->m_sizeRaw = s;
         pixels->m_bTopLeft = true;
         pixels->pixmap_map();
      }
   }

   void bitmap::set_size(const ::i32_size &s, bool preserve)
   {
      if (s == m_size && m_pbbitmap)
         return;
      ::memory saved;
      auto old = m_size;
      if (preserve && m_pbbitmap)
      {
         read_pixels();
         saved = m_memoryDraw2dBitmap;
      }
      create_bitmap(nullptr, s);
      if (preserve && saved.size() > 0)
         write_pixels(old, {}, (const ::image32_t *)saved.data(), old.cx * 4, true);
   }

   void bitmap::preserve_image(const ::i32_size &size, ::image::image *image)
   {
      if (!image || size.is_empty() || image->m_pdraw2dbitmap != this)
         throw ::exception(error_bad_argument);
      if (image->has_active_destination_graphics_lease() || image->m_pimagepixmaplease
         || (image->m_ppixmapOwned && image->m_ppixmapOwned->m_interlockedcountMap > 0))
         throw ::exception(error_wrong_state, "Cannot preserve an image while mapped or drawing");
      if (!m_pbbitmap || !m_pbview || m_pbbitmap->InitCheck() != B_OK)
         throw ::exception(error_wrong_state, "No Haiku bitmap to preserve");

      // Flush CPU edits before copying native pixels. set_size preserves the
      // overlap and initializes the newly allocated pixels to transparent.
      if (image->m_bWasMappedAfterLastGraphicsAcquisition && image->m_ppixmapOwned)
         defer_write_pixels(*image->m_ppixmapOwned);
      auto sizeRaw = image->raw_size().maximum(image->m_point + size);
      set_size(sizeRaw, true);
      image->m_size = size;
      image->m_sizeRaw = sizeRaw;
      image->m_iScan = m_iStride;
      // Rebuild borrowed mappings on demand instead of retaining an old address.
      image->m_ppixmapOwned.release();
      image->m_bGraphicsWasAcquiredAfterLastMap = true;
      image->m_bWasMappedAfterLastGraphicsAcquisition = false;
   }

   void bitmap::write_pixels(const ::i32_size &s, const ::i32_point &point, const ::image32_t *data, ::i32 scan,
                             bool topDown)
   {
      if (!data || scan < s.cx * 4 || s.cx <= 0 || s.cy <= 0)
         throw ::exception(error_bad_argument);
      ::memory copy;
      auto copyScan = s.cx * 4;
      copy.set_size((::memsize)copyScan * s.cy);
      for (int y = 0; y < s.cy; ++y)
         memory_copy(copy.data() + (::memsize)y * copyScan,
            reinterpret_cast<const ::u8 *>(data) + (::memsize)y * scan, copyScan);
      read_pixels();
      for (int y = 0; y < s.cy; y++)
      {
         int dy = point.y + y;
         if (dy < 0 || dy >= m_size.cy)
            continue;
         int start = maximum(0, -point.x), end = minimum(s.cx, m_size.cx - point.x);
         if (end <= start)
            continue;
         auto *src = copy.data() + (::memsize)(topDown ? y : s.cy - 1 - y) * copyScan + start * 4;
         auto *dst = m_memoryDraw2dBitmap.data() + (dy * m_size.cx + point.x + start) * 4;
         memory_copy(dst, src, (end - start) * 4);
      }
      commit_pixels();
   }

   void bitmap::read_pixels()
   {
      graphics_lock lock(this);
      m_pbview->Sync();
      const auto *src = static_cast<const uint8_t *>(m_pbbitmap->Bits());
      auto *dst = m_memoryDraw2dBitmap.data();
      // Haiku B_RGBA32 stores straight alpha; ca2 stores premultiplied alpha.
      for (int y = 0; y < m_size.cy; y++)
         for (int x = 0; x < m_size.cx; x++)
         {
            const auto *a = src + y * m_pbbitmap->BytesPerRow() + x * 4;
            auto *b = dst + (y * m_size.cx + x) * 4;
            b[3] = a[3];
            for (int c = 0; c < 3; c++)
               b[c] = uint8_t((unsigned(a[c]) * a[3] + 127) / 255);
         }
   }

   void bitmap::commit_pixels()
   {
      graphics_lock lock(this);
      m_pbview->Sync();
      auto *dst = static_cast<uint8_t *>(m_pbbitmap->Bits());
      const auto *src = m_memoryDraw2dBitmap.data();
      for (int y = 0; y < m_size.cy; y++)
         for (int x = 0; x < m_size.cx; x++)
         {
            const auto *a = src + (y * m_size.cx + x) * 4;
            auto *b = dst + y * m_pbbitmap->BytesPerRow() + x * 4;
            b[3] = a[3];
            for (int c = 0; c < 3; c++)
               b[c] = a[3] ? uint8_t(std::fmin(255., (unsigned(a[c]) * 255 + a[3] / 2) / a[3])) : 0;
         }
   }


}



