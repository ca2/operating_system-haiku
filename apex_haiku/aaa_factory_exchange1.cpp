#include "platform.h"


extern "C"
void node_haiku_factory(::factory::factory * pfactory)
{

   add_factory_item < node_haiku::node, ::acme::node >();

}



