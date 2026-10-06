#include "platform.h"


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         out::out()
         {

            m_poseq = NULL;

         }


         out::~out()
         {

            close();

         }


         ::estatus out::open()
         {

            // Set up the ALSA sequencer client.
            if ( m_poseq != NULL )
            {

               return ::success;

            }

            string clientName = "";

            int result = snd_seq_open(&m_poseq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);

            if ( result < 0 )
            {

               m_poseq = NULL;

               return ::error_failed;

            }
            else
            {

               // Set client name, use current name if given string is empty.
               if ( clientName != "" )
               {

                  m_strClientName = clientName;

               }

               snd_seq_set_client_name( m_poseq, m_strClientName.c_str() );

            }

            // Increment port count.
            //m_numPorts++;

            return ::success;

         }


         ::estatus out::close()
         {

            if(m_poseq != NULL)
            {

               snd_seq_close(m_poseq);

               m_poseq = NULL;

            }

            return ::success;

         }


         ::estatus out::send_short_message(::music::midi::e_message emessage, int iChannel, int iData1, int iData2)
         {

            //return midiOutShortMsg(m_hmidiout, MIDIMSG(((int) etype) >> 4, iChannel, iData1, iData2));
            return ::success;

         }



      } // namespace alsa


   } // namespace midi


} // namespace music




