#include "platform.h"
#include "apex/platform/launcher.h"
#include "launcher.h"


namespace apex_haiku
{


   void launcher::start()
   {

      if (!ensure_executable())
      {

         throw exception(error_failed);

      }

      string strPath(get_executable_path());

      string strDir(file_path_folder(strPath));


      /*string strPath

      SHELLEXECUTEINFOA infoa;

      __memset(&infoa, 0, sizeof(infoa));

      infoa.cbSize         = sizeof(infoa);
      infoa.pFile         = strPath;

      infoa.pDirectory    = strDir;*/


      //execlp(strPath, strPath, (char *) 0);



      //return true;

   }


} // namespace apex_haiku



