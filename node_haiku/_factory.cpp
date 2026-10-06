#include "platform.h"
#include "node.h"


//::user::enum_desktop get_edesktop();

__FACTORY_IMPORT void aura_haiku_factory(::factory::factory * pfactory);

__FACTORY_EXPORT void node_haiku_factory(::factory::factory * pfactory)
{


   aura_haiku_factory(pfactory);


   pfactory->add_factory_item<::node_haiku::node, ::platform::node>();


}

