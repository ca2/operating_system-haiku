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
      // directory_context::home() reads this cache, while the POSIX home()
      // method resolves HOME directly. Keep both layers in agreement.
      m_pathHome = home();
      if (m_pathHome.is_empty())
         throw ::exception(error_failed, "Haiku home directory is unavailable");


   }


   void directory_system::init_system()
   {

      ::directory_system::init_system();
      // Haiku does not define standard media directories. Provide ca2's
      // desktop defaults without replacing or moving any existing content.
      auto userHome = home();
      create(userHome / "Image");
      create(userHome / "Music");
      create(userHome / "Video");

   }


   ::file::path directory_system::roaming()
   {

      return home() / ".config";

   }


} // namespace acme_haiku



