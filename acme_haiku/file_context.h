#pragma once


#include "acme_posix/file_context.h"


namespace acme_haiku
{


   class CLASS_DECL_ACME_HAIKU file_context :
      virtual public ::acme_posix::file_context
   {
   public:


      file_context();
      ~file_context() override;


      void initialize(::particle * pparticle) override;


      void init_system() override;
      void init_context() override;


      //::file::path dropbox_info_network_payload() override;


   };


} // namespace acme_haiku



