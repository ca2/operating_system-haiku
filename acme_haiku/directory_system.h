#pragma once


#include "acme_posix/directory_system.h"


namespace acme_haiku
{


   class CLASS_DECL_ACME_HAIKU directory_system :
      virtual public ::acme_posix::directory_system
   {
   public:


      directory_system();
      ~directory_system() override;


      void initialize(::particle * pparticle) override;


      void init_system() override;


      ::file::path roaming() override;


   };


} // namespace acme_haiku



