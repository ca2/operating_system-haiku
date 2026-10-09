// Created by camilo on 2026-10-08 01:37 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/draw2d/bitmap.h"
#include "operating_system-haiku/draw2d_haiku/object.h"

#include <Bitmap.h>
#include <Region.h>
#include <View.h>
#include <AffineTransform.h>



namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU bitmap :
      virtual public ::draw2d::bitmap,
      virtual public ::draw2d_haiku::object
   {
   public:
      BBitmap *m_pbbitmap = nullptr;
      BView *m_pbview = nullptr; // Owned by BBitmap.
      int m_iSavedState = 0;
      ::array<BAffineTransform> m_btransforma;
      ::array<BRegion> m_bregionaClip;
      bitmap();

      ~bitmap() override;
      void destroy() override;
      void create_bitmap(::draw2d::graphics *, const ::i32_size &) override;
      void create_bitmap(::draw2d::graphics *, const ::i32_size &, ::pixmap *) override;
      ::i32 stride_for_width(::i32 w) override { return w * 4; }
      ::i32_size size() const override { return m_size; }
      void set_size(const ::i32_size &, bool preserve = false) override;
      void preserve_image(const ::i32_size &, ::image::image *) override;
      void write_pixels(const ::i32_size &, const ::i32_point &, const ::image32_t *, ::i32, bool) override;
      void read_pixels();
      void commit_pixels();
   };

   class graphics_lock
   {
   public:
      bitmap *m_pbitmap;

      explicit graphics_lock(bitmap *p) :
         m_pbitmap(p)
      {
         if (!p || !p->m_pbbitmap || !p->m_pbview || !p->m_pbbitmap->Lock())
            throw ::exception(error_wrong_state, "No Haiku drawing bitmap");
      }

      ~graphics_lock()
      {
         m_pbitmap->m_pbview->Sync();
         m_pbitmap->m_pbbitmap->Unlock();
      }
   };
}
