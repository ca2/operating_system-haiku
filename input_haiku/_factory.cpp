#include "platform.h"
#include "input.h"
IMPLEMENT_FACTORY(input_haiku)
{
   pfactory->add_factory_item<::input_haiku::input, ::input::input>();
}
