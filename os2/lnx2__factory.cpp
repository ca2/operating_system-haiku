#include "platform.h"


namespace win2
{


   factory_exchange::factory_exchange()
   {

      System.factory().creatable < ::win2::application         >  (System.type_info < ::cubebase::application > (), 1);

   }


   factory_exchange::~factory_exchange()
   {
   }

} // namespace win2


extern "C"
void ca2_factory()
{

   win2::factory_exchange factoryexchange();

}

