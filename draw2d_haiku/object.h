#pragma once


#include "aura/graphics/draw2d/object.h"


namespace draw2d_haiku
{


   class CLASS_DECL_DRAW2D_HAIKU object :
      virtual public ::draw2d::object
   {
      public:


         object();
         ~object() override;

//#ifdef DEBUG
//         void assert_ok() const override;
//         void dump(dump_context & dumpcontext) const override;
//#endif


   };


} // namespace aura



