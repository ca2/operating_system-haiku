#include "platform.h"
#include "out.h"
#include "in.h"
IMPLEMENT_FACTORY(audio_haiku) {
  pfactory->add_factory_item<audio_haiku::out, ::wave::out>();
  pfactory->add_factory_item<audio_haiku::in, ::wave::in>();
}
