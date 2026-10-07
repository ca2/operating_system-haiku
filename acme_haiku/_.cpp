#include "platform.h"


//#include "acme/library.h"


#include <unistd.h>
#include <limits.h>


namespace acme_haiku
{


   ::u32 get_current_directory(string& str)
   {

      char path[PATH_MAX];
      if (!::getcwd(path, sizeof(path))) throw ::exception(error_failed, "Cannot get current directory");
      str = path;

      return str.length();

   }


} // namespace acme_haiku



