#include "platform.h"
DECLARE_FACTORY(node_haiku);
DECLARE_FACTORY(windowing_haiku);
IMPLEMENT_FACTORY(operating_ambient_haiku){node_haiku_factory(pfactory);windowing_haiku_factory(pfactory);}
