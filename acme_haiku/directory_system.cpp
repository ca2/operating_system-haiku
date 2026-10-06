#include "platform.h"
#include "directory_system.h"


namespace acme_haiku
{


   directory_system::directory_system()
   {

   }


   directory_system::~directory_system()
   {

   }


   void directory_system::initialize(::particle * pparticle)
   {

      //auto estatus =

      ::directory_system::initialize(pparticle);


   }


   void directory_system::init_system()
   {

      ::directory_system::init_system();

   }


   ::file::path directory_system::roaming()
   {

      return home() / ".config";

   }


} // namespace acme_haiku



