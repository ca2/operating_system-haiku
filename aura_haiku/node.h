//
// Created by camilo on 21/01/2021. <33TBS!!
//

//
// Created by camilo on 21/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "apex_haiku/node.h"
#include "aura_posix/node.h"


namespace aura_haiku
{


   class CLASS_DECL_AURA_HAIKU node :
           virtual public ::apex_haiku::node,
           virtual public ::aura_posix::node
   {
   public:


      node();

      ~node() override;


      string get_user_name() override;


      void initialize(::particle * pparticle) override;


      //virtual bool _os_calc_app_dark_mode() override;

      //virtual bool _os_calc_system_dark_mode() override;

      //virtual ::color::color get_default_color(::u64 u) override;

      //virtual void set_console_colors(::u32 dwScreenColors, ::u32 dwPopupColors, ::u32 dwWindowAlpha) override;

      //virtual ::e_status set_system_dark_mode1(bool bSet = true);

      //virtual ::e_status set_app_dark_mode1(bool bSet = true);

      //virtual double get_time_zone() override;

      //virtual ::e_status get_system_time(system_time_t * psystemtime) override;

      void browse_for_folder(::file::path & pathFolder) override;

      //virtual ::e_status register_dll(const ::file::path & pathDll) override;

      //virtual ::e_status start() override;


   };


} // namespace aura_haiku



