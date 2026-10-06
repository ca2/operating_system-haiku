#include "platform.h"
#include "node.h"
//#include "interaction_impl.h"


__FACTORY_IMPORT void apex_haiku_factory(::factory::factory * pfactory);


__FACTORY_IMPORT void aura_posix_factory(::factory::factory * pfactory);


__FACTORY_EXPORT void aura_haiku_factory(::factory::factory * pfactory)
{

   apex_haiku_factory( pfactory);

   aura_posix_factory(pfactory);

   pfactory->add_factory_item < ::aura_haiku::node, ::platform::node >();
   //pfactory->add_factory_item < ::aura_haiku::interaction_impl, ::user::interaction_impl >();

}



