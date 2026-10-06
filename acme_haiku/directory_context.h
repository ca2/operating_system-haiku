#pragma once


#include "acme_posix/directory_context.h"


namespace acme_haiku
{


   class CLASS_DECL_ACME_HAIKU directory_context :
      virtual public ::acme_posix::directory_context
   {
   public:


      directory_context();
      ~directory_context() override;


      void initialize(::particle * pparticle) override;


      void init_system() override;
      void init_context() override;



      ::file::listing_base & root_ones(::file::listing_base & listing) override;


   };


} // namespace acme_haiku



