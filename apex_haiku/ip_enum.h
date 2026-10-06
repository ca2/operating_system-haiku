#pragma once

/*
Module : IPENUM.H
Purpose: Interface for an apex API class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (ca) 1998 by PJ Naughter.


*/


#include "apex/networking/ip_enum.h"


namespace apex_haiku
{


   class CLASS_DECL_APEX_HAIKU ip_enum :
      virtual public ::networking::ip_enum
   {
   public:


      bool     m_bWinsockInitialized;


      ip_enum();
      ~ip_enum() override;


      //      virtual ::count enumerate(ip_array & ipa);
      ::count enumerate(__pointer_array(::networking::address) & ipa) override;

   };


} // namespace apex_haiku



