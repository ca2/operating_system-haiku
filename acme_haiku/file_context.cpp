#include "platform.h"
#include "file_context.h"
#include "directory_context.h"


namespace acme_haiku
{


   file_context::file_context()
   {

   }


   file_context::~file_context()
   {

   }


   void file_context::initialize(::particle * pparticle)
   {

      ::acme_posix::file_context::initialize(pparticle);

   }


   void file_context::init_system()
   {

      ::file_context::init_system();

   }


   void file_context::init_context()
   {

      ::file_context::init_context();


   }


   //::file::path file_context::dropbox_info_network_payload()
   //{

     // ::file::path pathJson;

      //pathJson = directory()->home() / ".dropbox/info.json";

      //return pathJson;

   //}


} // namespace acme_haiku



