#include "platform.h"


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         factory_exchange::factory_exchange()
         {

            //add_factory_item < sequence, ::music::midi::sequence > ();
            //add_factory_item < file, ::music::midi::file > ();
            //add_factory_item < sequence_thread, ::music::midi::sequence_thread > ();
            //add_factory_item < player, ::music::midi::player > ();
            // add_factory_item < ::music::midi_alsa::player::player_interface , ::music::midi::player::player_interface > ();
            add_factory_item < midi, ::music::midi::midi > ();

         }


         factory_exchange::~factory_exchange()
         {
         }


      } // namespace alsa


   } // namespace midi


} // namespace music


extern "C"
void music_midi_alsa_factory()
{

   //::music::midi::alsa::factory_exchange factoryexchange();
   add_factory_item < ::music::midi::alsa::midi, ::music::midi::midi > ();

}









