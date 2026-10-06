#include "platform.h"

__FACTORY_EXPORT void audio_alsa_factory()
{

   add_factory_item < ::multimedia::audio_alsa::wave_in  , ::wave::in                  > ();

   add_factory_item < ::multimedia::audio_alsa::wave_out ,::wave::out                 > ();

}



