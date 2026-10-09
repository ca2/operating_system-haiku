#include "platform.h"
#include "directory_context.h"
#include "acme/filesystem/filesystem/listing.h"


namespace acme_haiku
{


   directory_context::directory_context()
   {


   }


   directory_context::~directory_context()
   {


   }


   void directory_context::initialize(::particle * pparticle)
   {

      ::acme_posix::directory_context::initialize(pparticle);

   }


   void directory_context::init_system()
   {

      ::acme_posix::directory_context::init_system();

   }


   void directory_context::init_context()
   {

      ::directory_context::init_context();

   }


   ::file::path directory_context::image()
   {
      m_pathImage = home() / "Image";
      return m_pathImage;
   }

   ::file::path directory_context::music()
   {
      m_pathMusic = home() / "Music";
      return m_pathMusic;
   }

   ::file::path directory_context::video()
   {
      m_pathVideo = home() / "Video";
      return m_pathVideo;
   }

   ::file::listing_base & directory_context::root_ones(::file::listing_base & listing)
   {

      auto pathHome = home();
      if (pathHome.has_character() && pathHome != "/")
      {

         pathHome.set_existent_folder();
         listing.insert_at(listing.size(), pathHome);
         listing.m_straTitle.add("Home");

      }

      ::file::path pathRoot = "/";
      pathRoot.set_existent_folder();
      listing.insert_at(listing.size(), pathRoot);
      listing.m_straTitle.add("File System");

      return listing;

   }


} // namespace acme_haiku



