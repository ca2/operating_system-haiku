// Created by camilo on 2026-10-07 22:37 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "aura/graphics/draw2d/graphics.h"


#include <Shape.h>


namespace draw2d_haiku 
{


	class bitmap;

	class CLASS_DECL_DRAW2D_HAIKU graphics :
      virtual public ::draw2d::graphics
	{
	public:

      using ::draw2d::graphics::set;
      using ::draw2d::graphics::line;
      using ::draw2d::graphics::draw_ellipse;
      using ::draw2d::graphics::fill_ellipse;
      using ::draw2d::graphics::_set;
      using ::draw2d::graphics::draw;
      using ::draw2d::graphics::fill;
		 
 
		BShape m_bshape;
 
		bool m_bBeginFigure=true;
 
		//::f64_point m_pointBitmapOrigin;
		bool m_bBuildingClip=false;


	   graphics();
	   ~graphics();

		
		void _add_shape(const ::f64_rectangle &) override;
		void _add_shape(const ::f64_ellipse &) override;
		void _add_shape(const ::f64_polygon_base &) override;
		void _intersect_clip() override;
 
		bitmap *target_bitmap();
		void set_alpha_mode(::draw2d::enum_alpha_mode) override;


	   void _001ColorSelect(const ::color::color & color);

 
		void on_acquire_memory_graphics(bool,::image::image *,const ::i32_size &,::draw2d::domain *) override;
		void _create_memory_graphics(const ::i32_size &,::draw2d::domain *) override;
		::i32 save_graphics_context() override;
		void restore_graphics_context(::i32) override;
		void _set(const ::geometry2d::matrix &) override;
		void intersect_clip(const ::f64_rectangle &) override;
		void reset_clip() override;
		void _draw_raw(const ::f64_rectangle &,::image::image *,const ::image::image_drawing_options &,const ::f64_point &) override;
		void _stretch_raw(const ::f64_rectangle &,::image::image *,const ::image::image_drawing_options &,const ::f64_rectangle &) override;
		void draw(::draw2d::path *) override;
		void draw(::draw2d::path *, ::draw2d::pen *) override;
		void fill(::draw2d::path *) override;
		void fill(::draw2d::path *, ::draw2d::brush *) override;
		bool _set(const ::draw2d::enum_item &) override;
		bool _set(const ::f64_line &) override;
		bool _set(const ::f64_lines &) override;
		bool _set(const ::f64_rectangle &) override;
		bool _set(const ::f64_ellipse &) override;
		bool _set(const ::f64_arc &) override;
		bool _set(const ::f64_polygon_base &) override;
		void prepare_path(::draw2d::path *);
		void move_shape(double,double);
		void _arc_shape(double l, double t, double r, double b, double start, double exten);
	   void arc(::f64 x, ::f64 y, ::f64 w, ::f64 h, ::f64_angle start, ::f64_angle extends) override;
		void _paint_shape(::draw2d::brush *pdraw2dbrush,::draw2d::pen *pdraw2dpen,bool alternate);
		void set(::draw2d::bitmap *) override;
		void create_bitmap_graphics(::draw2d::bitmap *,::draw2d::domain *) override;
		void line(double,double,double,double) override;
		void line(double,double,double,double,::draw2d::pen *) override;
		void fill_rectangle(const ::f64_rectangle &) override;
		void fill_rectangle(const ::f64_rectangle &,::draw2d::brush *) override;
		void fill_rectangle(const ::f64_rectangle &,const ::color::color &) override;
		void draw_rectangle(const ::f64_rectangle &) override;
		void draw_rectangle(const ::f64_rectangle &,::draw2d::pen *) override;
		void fill_ellipse(const ::f64_rectangle &) override;
		void draw_ellipse(const ::f64_rectangle &) override;
		void TextOutRaw(double,double,const ::scoped_string &) override;
		::f64_size get_text_extent(const ::scoped_string &) override;
		::f64_size _get_text_extent(const ::scoped_string &) override;


	};


}  // namespace draw2d_haiku



