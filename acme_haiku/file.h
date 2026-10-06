/*****<3ThomasBorregaardSørensen!!Created by camilo*/
// From acme_posix/acme_file by camilo on 2021-11-22 00:07 AM <3ThomasBorregaardSørensen!!
// From acme_apple/acme_file.h
// on 2021-08-09
// 05:02 BRT
#pragma once


#include "acme_darwin/acme_file.h"


namespace acme_haiku
{


   class CLASS_DECL_ACME acme_file :
      virtual public ::acme_darwin::acme_file
   {
   public:


      acme_file();
      ~acme_file() noexcept override ;


      //::file::path module() override;


   };


} // namespace acme_haiku



