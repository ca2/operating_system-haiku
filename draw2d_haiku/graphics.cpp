// Created by camilo on 2026-10-07 22:40 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "aura/graphics/image/image.h"
#include "graphics.h"
#include "../../../source/app/aura/graphics/draw2d/graphics.h"

#include "bitmap.h"
#include "font.h"
#include "acme/prototype/geometry2d/matrix.h"
#include "acme/prototype/geometry2d/line.h"
#include "acme/prototype/geometry2d/lines.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "acme/prototype/geometry2d/arc.h"
#include "acme/prototype/geometry2d/polygon.h"
#include "aura/graphics/draw2d/domain.h"
#include "aura/graphics/draw2d/path.h"
#include "aura/graphics/draw2d/brush.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/graphics/image/drawing.h"
#include "acme/exception/interface_only.h"
#include <AffineTransform.h>
#include <GradientLinear.h>
#include <GradientRadial.h>
#include <cmath>


namespace draw2d_haiku
{


   rgb_color as_rgb_color(const ::color::color &c)
   {
      return {c.u8_red(), c.u8_green(), c.u8_blue(), c.u8_opacity()};
   }


   BRect as_brect(const ::f64_rectangle &r)
   {
      return BRect(r.left, r.top, r.right - 1, r.bottom - 1);
   }


   graphics::graphics()
   {


   }


   graphics::~graphics()
   {


   }


   bitmap *graphics::target_bitmap()
   {
      defer_on_target_rectangle_update();

      ::cast<bitmap> pbitmap = m_pdraw2dbitmap;

      if (!pbitmap || !pbitmap->m_pbbitmap)
      {
         throw ::exception(error_wrong_state, "No Haiku drawing bitmap");
      }

      return pbitmap;
   }


   void graphics::set_alpha_mode(::draw2d::enum_alpha_mode mode)
   {
      ::draw2d::graphics::set_alpha_mode(mode);
      ::cast<bitmap> nativeBitmap = m_pdraw2dbitmap;
      if (!nativeBitmap || !nativeBitmap->m_pbbitmap || !nativeBitmap->m_pbview)
         return;
      graphics_lock lock(nativeBitmap);
      auto view = nativeBitmap->m_pbview;
      view->SetDrawingMode(mode == ::draw2d::e_alpha_mode_blend ? B_OP_ALPHA : B_OP_COPY);
      view->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_COMPOSITE);
   }

   void graphics::_001ColorSelect(const ::color::color &color)
   {
      update_matrix();

      auto pbview = target_bitmap()->m_pbview;

      pbview->SetHighColor(as_rgb_color(color));

      pbview->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_COMPOSITE);

   }


   void graphics::set(::draw2d::bitmap *pbitmap)
   {


      ::cast<bitmap> pdraw2dhaikubitmap = pbitmap;

      if (!pdraw2dhaikubitmap)
      {

         throw ::exception(error_wrong_state);

      }

      m_pdraw2dbitmap = pbitmap;

   }


   void graphics::create_bitmap_graphics(::draw2d::bitmap *pbitmap, ::draw2d::domain *pdraw2ddomain)
   {

      set_draw2d_domain(pdraw2ddomain);

      set(pbitmap);


   }


   void graphics::on_acquire_memory_graphics(
      bool external,
      ::image::image *ptarget,
      const ::i32_size &size,
      ::draw2d::domain *domain)
   {
      //m_pointBitmapOrigin=target?::f64_point(target->m_point) : ::f64_point();

      if (ptarget)
      {
         auto pbitmap = ptarget->get_bitmap_as_target(this);

         set(pbitmap);
      }

      ::draw2d::graphics::on_acquire_memory_graphics(external, ptarget, size, domain);
   }


   void graphics::_create_memory_graphics(const ::i32_size &size, ::draw2d::domain *domain)
   {
      set_draw2d_domain(domain);

      constructø(m_pimageOwned);

      m_pimageOwned->update_as_render_target(size, domain, this);

      m_pimageOwned->m_pgraphicsOwned = this;

      set(m_pimageOwned->m_pdraw2dbitmap);

      m_pimageTarget = m_pimageOwned;

      set_ok_flag();
   }


   void graphics::line(double a, double b, double c, double d)
   {
      line(a, b, c, d, m_pdraw2dpen);
   }


   void graphics::line(double a, double b, double c, double d, ::draw2d::pen *p)
   {
      if (!p || p->m_epen == ::draw2d::e_pen_null)
      {
         return;
      }

      graphics_lock lock(target_bitmap());

      _001ColorSelect(p->m_color);

      auto pbview = lock.m_pbitmap->m_pbview;
      pbview->SetPenSize(p->m_dWidth);
      pbview->StrokeLine(BPoint(a, b), BPoint(c, d));
   }


   void graphics::fill_rectangle(const ::f64_rectangle &r) { fill_rectangle(r, m_pdraw2dbrush); }

   void graphics::fill_rectangle(const ::f64_rectangle &r, ::draw2d::brush *pdraw2dbrush)
   {


      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      if (!pdraw2dbrush || pdraw2dbrush->m_ebrush == ::draw2d::e_brush_null)
         return;
      if (pdraw2dbrush->m_ebrush == ::draw2d::e_brush_solid)
      {
         fill_rectangle(r, pdraw2dbrush->m_color);
         return;
      }
      m_bshape.Clear();
      m_bBeginFigure = true;
      _set(r);
      _paint_shape(pdraw2dbrush, nullptr, false);
   }


   void graphics::fill_rectangle(const ::f64_rectangle &rectangle, const ::color::color &color)
   {
      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }

      graphics_lock lock(target_bitmap());

      _001ColorSelect(color);

      static int traceCount=0;if(traceCount++<8){fprintf(stderr,"RECT %g,%g,%g,%g scale=%g origin=%g,%g\n",rectangle.left,rectangle.top,rectangle.right,rectangle.bottom,double(lock.m_pbitmap->m_pbview->Scale()),double(lock.m_pbitmap->m_pbview->Origin().x),double(lock.m_pbitmap->m_pbview->Origin().y));auto view=lock.m_pbitmap->m_pbview;double tx=0,ty=0;view->Transform().GetTranslation(&tx,&ty);auto bb=lock.m_pbitmap->m_pbbitmap->Bounds();auto vb=view->Bounds();fprintf(stderr,"DRAW bitmap=%p transform=%g,%g nativeBitmap=%g,%g,%g,%g nativeView=%g,%g,%g,%g\n",static_cast<::draw2d::bitmap *>(lock.m_pbitmap),tx,ty,double(bb.left),double(bb.top),double(bb.right),double(bb.bottom),double(vb.left),double(vb.top),double(vb.right),double(vb.bottom));}
      lock.m_pbitmap->m_pbview->FillRect(as_brect(rectangle));
   }


   void graphics::draw_rectangle(const ::f64_rectangle &rectangle) { draw_rectangle(rectangle, m_pdraw2dpen); }

   void graphics::draw_rectangle(const ::f64_rectangle &rectangle, ::draw2d::pen *pdraw2dpen)
   {



      if (!pdraw2dpen || pdraw2dpen->m_epen == ::draw2d::e_pen_null)
         return;
      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      graphics_lock lock(target_bitmap());
      _001ColorSelect(pdraw2dpen->m_color);
      lock.m_pbitmap->m_pbview->SetPenSize(pdraw2dpen->m_dWidth);
      lock.m_pbitmap->m_pbview->StrokeRect(as_brect(rectangle));
   }

   void graphics::fill_ellipse(const ::f64_rectangle &rectangle)
   {

      auto *pbrush = m_pdraw2dbrush.m_p;

      if (!pbrush || pbrush->m_ebrush == ::draw2d::e_brush_null)
         return;


      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      if (pbrush->m_ebrush != ::draw2d::e_brush_solid)
      {
         m_bshape.Clear();

         m_bBeginFigure = true;

         _arc_shape(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, 0, 2 * MATH_PI);

         m_bshape.Close();

         _paint_shape(pbrush, nullptr, false);

         return;
      }

      graphics_lock lock(target_bitmap());

      ::draw2d::save_context savecontext(this);

      _001ColorSelect(pbrush->m_color);

      auto pbview = lock.m_pbitmap->m_pbview;

      //auto transform = pbview->Transform();

      auto rect = as_brect(rectangle);

      //BPoint corners[2] = {rect.LeftTop(), rect.RightBottom()};

      //transform.Apply(corners, 2);

      //pbview->SetTransform(BAffineTransform());

      //pbview->FillEllipse(BRect(corners[0], corners[1]));

      pbview->FillEllipse(rect);

      //pbview->SetTransform(transform);

      //auto tr = lock.m_pbitmap->m_pbview->Transform();

      //double tx = 0, ty = 0;
      //tr.

        // GetTranslation(&tx, &ty);
   }


   void graphics::draw_ellipse(const ::f64_rectangle &rectangle)
   {
      auto *pdraw2dpen = m_pdraw2dpen.m_p;

      if (!pdraw2dpen || pdraw2dpen->m_epen == ::draw2d::e_pen_null)
      {
         return;
      }

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }



      graphics_lock lock(target_bitmap());

      _001ColorSelect(pdraw2dpen->m_color);

      //::draw2d::save_context savecontext(this);

      auto pbview = lock.m_pbitmap->m_pbview;

      //auto transform = pbview->Transform();

      auto rect = as_brect(rectangle);

      //BPoint corners[2] = {rect.LeftTop(), rect.RightBottom()};

      //transform.Apply(corners, 2);

      //pbview->SetTransform(BAffineTransform());

      pbview->SetPenSize(pdraw2dpen->m_dWidth);

      //pbview->StrokeEllipse(BRect(corners[0], corners[1]));
      pbview->StrokeEllipse(rect);

      //pbview->SetTransform(transform);

      //pbview->Sync();

      //double tx = 0, ty = 0;

//      pbview->Transform().GetTranslation(&tx, &ty);

  //    int xx = int(tx + 12), yy = int(ty);

    //  if (xx >= 0 && yy >= 0 && xx < lock.m_pbitmap->m_size.cx && yy < lock.m_pbitmap->m_size.cy)
      //{
        // auto *bits = static_cast<unsigned char *>(lock.m_pbitmap->m_pbbitmap->Bits());
     // }
   }


   void graphics::TextOutRaw(double x, double y, const ::scoped_string &scopedstr)
   {
      ::string s(scopedstr);

      BFont f(be_plain_font);
      ::cast<font> pdraw2dhaikufont = m_pwritetextfont;
      if (pdraw2dhaikufont)
      {
         pdraw2dhaikufont->update(this);
         f = *pdraw2dhaikufont->m_pbfont;
      }

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }



      font_height h{};

      f.GetHeight(&h);

      graphics_lock lock(target_bitmap());

      _001ColorSelect(m_pdraw2dbrush ? m_pdraw2dbrush->m_color : ::argb(255, 0, 0, 0));

      auto pbview = lock.m_pbitmap->m_pbview;

      pbview->SetFont(&f);

      static int traceText=0;if(traceText++<6)fprintf(stderr,"TEXT text=%s point=%g,%g target=%g,%g matrix=%g,%g bitmap=%p\n",s.c_str(),x,y,m_pointTarget.x,m_pointTarget.y,m_matrix.c1,m_matrix.c2,m_pdraw2dbitmap.m_p);
      pbview->DrawString(s.c_str(), s.size(), BPoint(x, y + h.ascent));
   }


   ::f64_size graphics::get_text_extent(const ::scoped_string &scopedstr)
   {
      ::string s(scopedstr);

      BFont f(be_plain_font);
      ::cast<font> pdraw2dhaikufont = m_pwritetextfont;
      if (pdraw2dhaikufont)
      {
         pdraw2dhaikufont->update(this);
         f = *pdraw2dhaikufont->m_pbfont;
      }

      font_height h{};

      f.GetHeight(&h);

      return {f.StringWidth(s.c_str(), s.size()), h.ascent + h.descent + h.leading};
   }


   ::f64_size graphics::_get_text_extent(const ::scoped_string &scopedstr)
   {
      return get_text_extent(scopedstr);
   }


   void graphics::prepare_path(::draw2d::path *pdraw2dpath)
   {
      if (!pdraw2dpath)
      {
         throw ::exception(error_null_pointer);
      }

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      m_bshape.Clear();

      m_bBeginFigure = true;

      for (auto &item: pdraw2dpath->m_itema)
      {
         if (!::draw2d::graphics::_set(item))
         {
            throw ::interface_only("Haiku path item pending");
         }
      }
   }


   void graphics::_paint_shape(::draw2d::brush *pbrush, ::draw2d::pen *ppen, bool alternate)
   {
      if ((!pbrush && !ppen) || (pbrush && ppen))
         throw ::exception(error_bad_argument);

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      graphics_lock lock(target_bitmap());
      update_matrix();
      static int traceShape=0;if(traceShape++<5){auto bounds=m_bshape.Bounds();fprintf(stderr,"SHAPE bounds=%g,%g,%g,%g target=%g,%g matrix=%g,%g\n",double(bounds.left),double(bounds.top),double(bounds.right),double(bounds.bottom),m_pointTarget.x,m_pointTarget.y,m_matrix.c1,m_matrix.c2);}
        auto view = lock.m_pbitmap->m_pbview;
      // Native shapes are relative to the pen; CA2 paths use absolute coordinates.
      view->MovePenTo(BPoint(0, 0));
      view->SetFillRule(alternate ? B_EVEN_ODD : B_NONZERO);
      if (ppen)
         view->SetPenSize(ppen->m_dWidth);
      auto brush = pbrush ? pbrush : ppen->m_pdraw2dbrush.m_p;
      if (!brush || brush->m_ebrush == ::draw2d::e_brush_solid)
      {
         _001ColorSelect(brush ? brush->m_color : ppen->m_color);
         if (pbrush)
            view->FillShape(&m_bshape);
         else
            view->StrokeShape(&m_bshape);
      }
      else if (brush->m_ebrush == ::draw2d::e_brush_linear_gradient_point_color)
      {
         BGradientLinear gradient(BPoint(brush->m_point1.x, brush->m_point1.y),
                                  BPoint(brush->m_point2.x, brush->m_point2.y));
         gradient.AddColor(as_rgb_color(brush->m_color1), 0);
         gradient.AddColor(as_rgb_color(brush->m_color2), 255);
         if (pbrush)
            view->FillShape(&m_bshape, gradient);
         else
            view->StrokeShape(&m_bshape, gradient);
      }
      else if (brush->m_ebrush == ::draw2d::e_brush_radial_gradient_color)
      {
         if (brush->m_size.cx != brush->m_size.cy)
            throw ::interface_only("Elliptical gradient pending");
         BGradientRadial gradient(BPoint(brush->m_point.x, brush->m_point.y), brush->m_size.cx / 2);
         gradient.AddColor(as_rgb_color(brush->m_color1), 0);
         gradient.AddColor(as_rgb_color(brush->m_color2), 255);
         if (pbrush)
            view->FillShape(&m_bshape, gradient);
         else
            view->StrokeShape(&m_bshape, gradient);
      }
      else if (brush->m_ebrush == ::draw2d::e_brush_box_gradient && pbrush)
      {
         auto bounds = m_bshape.Bounds();
         int left = int(std::floor(bounds.left)), top = int(std::floor(bounds.top));
         int width = int(std::ceil(bounds.right)) - left + 1;
         int height = int(std::ceil(bounds.bottom)) - top + 1;
         if (width <= 0 || height <= 0)
            return;
         BBitmap pixels(BRect(0, 0, width - 1, height - 1), B_RGBA32);
         if (pixels.InitCheck() != B_OK)
            throw ::exception(error_failed);
         double halfWidth = brush->m_size.cx / 2, halfHeight = brush->m_size.cy / 2;
         double radius = std::fmax(0., std::fmin(brush->m_dRadius, std::fmin(halfWidth, halfHeight)));
         double cx = brush->m_point.x + halfWidth, cy = brush->m_point.y + halfHeight;
         auto inner = as_rgb_color(brush->m_color1), outer = as_rgb_color(brush->m_color2);
         for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
            {
               double dx = std::fabs(left + x + .5 - cx) - (halfWidth - radius);
               double dy = std::fabs(top + y + .5 - cy) - (halfHeight - radius);
               double distance = std::hypot(std::fmax(dx, 0.), std::fmax(dy, 0.))
                  + std::fmin(std::fmax(dx, dy), 0.) - radius;
               double t = std::fmax(0., std::fmin(1., 1. + distance / std::fmax(radius, 1.)));
               double a = inner.alpha * (1. - t), b = outer.alpha * t, alpha = a + b;
               auto *pixel = static_cast<unsigned char *>(pixels.Bits()) + y * pixels.BytesPerRow() + x * 4;
               pixel[0] = alpha > 0 ? (inner.blue * a + outer.blue * b) / alpha : 0;
               pixel[1] = alpha > 0 ? (inner.green * a + outer.green * b) / alpha : 0;
               pixel[2] = alpha > 0 ? (inner.red * a + outer.red * b) / alpha : 0;
               pixel[3] = alpha;
            }
         auto transform = view->Transform();
         view->SetTransform(BAffineTransform());
         view->PushState();
         view->SetTransform(transform);
         view->ClipToShape(&m_bshape);
         view->DrawBitmap(&pixels, BPoint(left, top));
         view->PopState();
         view->SetTransform(transform);
      }
      else
         throw ::interface_only("Haiku brush type pending");
   }

   void graphics::fill(::draw2d::path *pdraw2dpath)
   {
      fill(pdraw2dpath, m_pdraw2dbrush);
   }

   void graphics::fill(::draw2d::path *pdraw2dpath, ::draw2d::brush *pdraw2dbrush)
   {
      if (!pdraw2dbrush || pdraw2dbrush->m_ebrush == ::draw2d::e_brush_null)
      {
         return;
      }

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }



      prepare_path(pdraw2dpath);

      _paint_shape(pdraw2dbrush, nullptr, pdraw2dpath->m_efillmode == ::draw2d::e_fill_mode_alternate);
   }


   void graphics::draw(::draw2d::path *pdraw2dpath)
   {
      draw(pdraw2dpath, m_pdraw2dpen);
   }

   void graphics::draw(::draw2d::path *pdraw2dpath, ::draw2d::pen *pdraw2dpen)
   {

      if (!pdraw2dpen || pdraw2dpen->m_epen == ::draw2d::e_pen_null)
         return;

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }



      prepare_path(pdraw2dpath);

      if (pdraw2dpen->m_pdraw2dbrush)
      {
         _paint_shape(nullptr, pdraw2dpen, false);
      }
      else
      {
         graphics_lock lock(target_bitmap());
         _001ColorSelect(pdraw2dpen->m_color);
         auto pbview = lock.m_pbitmap->m_pbview;
         pbview->MovePenTo(BPoint(0, 0));
         pbview->SetPenSize(pdraw2dpen->m_dWidth);
         pbview->StrokeShape(&m_bshape);
      }
   }

   void graphics::move_shape(double x, double y)
   {
      BPoint p(x, y);
      if (m_bBeginFigure)
         m_bshape.MoveTo(p);
      else if (m_bshape.CurrentPosition() != p)
         m_bshape.LineTo(p);
      m_bBeginFigure = false;
   }


   void graphics::arc(::f64 x, ::f64 y, ::f64 w, ::f64 h, ::f64_angle start, ::f64_angle extends)
   {
      if (!m_pdraw2dpen || m_pdraw2dpen->m_epen == ::draw2d::e_pen_null)
         return;
      m_bshape.Clear();
      m_bBeginFigure = true;
      _arc_shape(x, y, x + w, y + h, start.radian(), extends.radian());
      _paint_shape(nullptr, m_pdraw2dpen, false);
   }

   void graphics::_arc_shape(double l, double t, double r, double b, double start, double extent)
   {
      double cx = (l + r) / 2, cy = (t + b) / 2, rx = (r - l) / 2, ry = (b - t) / 2;
      int steps = int(std::ceil(std::fabs(extent) / (MATH_PI / 2)));
      if (steps < 1)
         return;
      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      double step = extent / steps;
      move_shape(cx + rx * std::cos(start), cy + ry * std::sin(start));
      for (int i = 0; i < steps; i++)
      {
         double a = start + i * step, z = a + step, k = 4. / 3. * std::tan(step / 4);
         BPoint points[3] = {
            BPoint(cx + rx * (std::cos(a) - k * std::sin(a)), cy + ry * (std::sin(a) + k * std::cos(a))),
            BPoint(cx + rx * (std::cos(z) + k * std::sin(z)), cy + ry * (std::sin(z) - k * std::cos(z))),
            BPoint(cx + rx * std::cos(z), cy + ry * std::sin(z))
         };
         m_bshape.BezierTo(points);
      }
   }

   bool graphics::_set(const ::draw2d::enum_item &eitem)
   {
      if (eitem == ::draw2d::e_item_begin_figure || eitem == ::draw2d::e_item_end_figure)
         m_bBeginFigure = true;
      else if (eitem== ::draw2d::e_item_close_figure)
      {
         m_bshape.Close();
         m_bBeginFigure = true;
      }
      else
         return false;
      return true;
   }

   bool graphics::_set(const ::f64_line &line)
   {
      move_shape(line.m_p1.x, line.m_p1.y);
      m_bshape.LineTo(BPoint(line.m_p2.x, line.m_p2.y));
      return true;
   }

   bool graphics::_set(const ::f64_lines &lines)
   {
      for (::collection::index i = 1; i < lines.get_count(); i++)
      {
         move_shape(lines[i - 1].x, lines[i - 1].y);
         m_bshape.LineTo(BPoint(lines[i].x, lines[i].y));
      }
      return true;
   }

   bool graphics::_set(const ::f64_polygon_base &polygon)
   {
      for (::collection::index i = 1; i < polygon.get_count(); i++)
      {
         move_shape(polygon[i - 1].x, polygon[i - 1].y);
         m_bshape.LineTo(BPoint(polygon[i].x, polygon[i].y));
      }
      m_bshape.Close();
      m_bBeginFigure = true;
      return true;
   }

   bool graphics::_set(const ::f64_rectangle &rectangle)
   {
      m_bBeginFigure = true;
      move_shape(rectangle.left, rectangle.top);
      m_bshape.LineTo(BPoint(rectangle.right, rectangle.top));
      m_bshape.LineTo(BPoint(rectangle.right, rectangle.bottom));
      m_bshape.LineTo(BPoint(rectangle.left, rectangle.bottom));
      m_bshape.Close();
      m_bBeginFigure = true;
      return true;
   }

   bool graphics::_set(const ::f64_ellipse &ellipse)
   {
      m_bBeginFigure = true;
      _arc_shape(ellipse.left, ellipse.top, ellipse.right, ellipse.bottom, 0, 2 * MATH_PI);
      m_bshape.Close();
      m_bBeginFigure = true;
      return true;
   }

   bool graphics::_set(const ::f64_arc &arc)
   {
      _arc_shape(arc.left, arc.top, arc.right, arc.bottom, arc.m_angleBeg.radian(), arc.m_angleExt.radian());
      return true;
   }


   ::i32 graphics::save_graphics_context()
   {
      graphics_lock lock(target_bitmap());
      auto * pbitmap = lock.m_pbitmap;
      auto transform = pbitmap->m_pbview->Transform();
      pbitmap->m_btransforma.add(transform);
      BRegion region;
      pbitmap->m_pbview->GetClippingRegion(&region);
      pbitmap->m_bregionaClip.add(region);
      // Haiku composes transforms with parent states; CA2 matrices are absolute.
      pbitmap->m_pbview->SetTransform(BAffineTransform());
      pbitmap->m_pbview->ConstrainClippingRegion(nullptr);
      pbitmap->m_pbview->PushState();
      pbitmap->m_pbview->SetTransform(transform);
      pbitmap->m_pbview->ConstrainClippingRegion(&region);
      return ++lock.m_pbitmap->m_iSavedState;
   }


   void graphics::restore_graphics_context(::i32 state)
   {
      graphics_lock lock(target_bitmap());
      auto *pbitmap = lock.m_pbitmap;
      if (state < 1 || state > pbitmap->m_iSavedState)
         throw ::exception(error_bad_argument);
      while (pbitmap->m_iSavedState >= state)
      {
         pbitmap->m_pbview->PopState();
         pbitmap->m_pbview->SetTransform(pbitmap->m_btransforma.pop());
         pbitmap->m_pbview->ConstrainClippingRegion(&pbitmap->m_bregionaClip.last());
         pbitmap->m_bregionaClip.erase_last();
         --pbitmap->m_iSavedState;
      }
   }


   void graphics::_set(const ::geometry2d::matrix &matrix)
   {

      if (!m_pdraw2dbitmap)
      {

         return;

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      graphics_lock lock(target_bitmap());

      lock.m_pbitmap->m_pbview->SetTransform(BAffineTransform(matrix.a1, matrix.a2, matrix.b1, matrix.b2, matrix.c1, matrix.c2));

   }


   void graphics::intersect_clip(const ::f64_rectangle &rectangle)
   {
      graphics_lock lock(target_bitmap());

      if (m_bTargetRectangleModified)
      {
         defer_on_target_rectangle_update();
      }


      auto pbview = lock.m_pbitmap->m_pbview;
      auto tr = pbview->Transform();
      auto rect = as_brect(rectangle);
      BPoint pts[4] = {rect.LeftTop(), rect.RightTop(), rect.RightBottom(), rect.LeftBottom()};
      tr.Apply(pts, 4);
      rect = BRect(pts[0], pts[0]);
      for (int i = 1; i < 4; i++)
      {
         rect.left = std::fmin(rect.left, pts[i].x);
         rect.top = std::fmin(rect.top, pts[i].y);
         rect.right = std::fmax(rect.right, pts[i].x);
         rect.bottom = std::fmax(rect.bottom, pts[i].y);
      }
      //pbview->SetTransform(BAffineTransform()); /* clip diagnostic */
      //pbview->SetTransform(tr);
   }

   void graphics::reset_clip()
   {
      if (!m_pdraw2dbitmap)
         return;
      graphics_lock lock(target_bitmap());
      lock.m_pbitmap->m_pbview->ConstrainClippingRegion(nullptr);
   }

   void graphics::_draw_raw(const ::f64_rectangle &rectangleTarget, ::image::image *pimageSource,
                            const ::image::image_drawing_options &imagedrawingoptions, const ::f64_point &point)
   {
      _stretch_raw(rectangleTarget, pimageSource, imagedrawingoptions, ::f64_rectangle(point, rectangleTarget.size()));
   }

   void graphics::_stretch_raw(const ::f64_rectangle &rectangleTarget, ::image::image *pimageSource,
                               const ::image::image_drawing_options &imagedrawingoptions, const ::f64_rectangle &rectangleSource)
   {
      if (!pimageSource)
         throw ::exception(error_null_pointer);
      auto b = pimageSource->get_bitmap_as_source(this);
      auto *native = dynamic_cast<bitmap *>(b.m_p);
      if (!native)
         throw ::exception(error_wrong_state);
      BBitmap copy(native->m_pbbitmap->Bounds(), B_RGBA32);
      if (copy.InitCheck() != B_OK)
         throw ::exception(error_failed);
      {
         graphics_lock lock(native);
         native->m_pbview->Sync();
         int w = native->m_size.cx, h = native->m_size.cy;
         for (int y = 0; y < h; y++)
         {
            auto *s = static_cast<const uint8_t *>(native->m_pbbitmap->Bits()) + y * native->m_pbbitmap->
                      BytesPerRow();
            auto *t = static_cast<uint8_t *>(copy.Bits()) + y * copy.BytesPerRow();
            memory_copy(t, s, w * 4);
            for (int x = 0; x < w; x++)
               t[x * 4 + 3] = uint8_t(
                  std::fmax(0., std::fmin(255., t[x * 4 + 3] * imagedrawingoptions.opacity().f64_opacity())));
         }
      }
      graphics_lock lock(target_bitmap());
      _001ColorSelect(::argb(255, 255, 255, 255));
      static int traceBlit=0;if(traceBlit++<5)fprintf(stderr,"BLIT dst=%g,%g,%g,%g target=%g,%g matrix=%g,%g\n",rectangleTarget.left,rectangleTarget.top,rectangleTarget.right,rectangleTarget.bottom,m_pointTarget.x,m_pointTarget.y,m_matrix.c1,m_matrix.c2);
      lock.m_pbitmap->m_pbview->DrawBitmap(&copy, as_brect(rectangleSource), as_brect(rectangleTarget), B_FILTER_BITMAP_BILINEAR);
   }

   void graphics::_add_shape(const ::f64_rectangle &polygon)
   {
      if (!m_bBuildingClip)
      {
         m_bshape.Clear();
         m_bBeginFigure = true;
         m_bBuildingClip = true;
      }
      _set(polygon);
   }

   void graphics::_add_shape(const ::f64_ellipse &polygon)
   {
      if (!m_bBuildingClip)
      {
         m_bshape.Clear();
         m_bBeginFigure = true;
         m_bBuildingClip = true;
      }
      _set(polygon);
   }

   void graphics::_add_shape(const ::f64_polygon_base &polygon)
   {
      if (!m_bBuildingClip)
      {
         m_bshape.Clear();
         m_bBeginFigure = true;
         m_bBuildingClip = true;
      }
      _set(polygon);
   }

   void graphics::_intersect_clip()
   {
      graphics_lock lock(target_bitmap()); /* shape clip diagnostic */
      m_bshape.Clear();
      m_bBuildingClip = false;
   }


} // namespace draw2d_haiku
