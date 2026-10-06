#pragma once


#define MIDIMSG(status,channel,data1,data2) ( (uint32_t)((status<<4) | channel | (data1<<8) | (data2<<16)) )


namespace music
{


   namespace midi
   {


      namespace alsa
      {

      //::estatus ::music::translate(::estatus smfrc);


      }

   } // namespace midi_alsa


} // namespace music
