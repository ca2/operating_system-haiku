#pragma once


#include <alsa/asoundlib.h>


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         class message_out :
            virtual public ::music::midi::message_out
         {
         public:


            uint64_t                m_ui64Start;
            snd_rawmidi_t *         m_prawmidi;
            int64_t                 m_iWrite;
            memory_file             m_file;


            message_out(midi * pmidi, string strDevice);
            virtual ~message_out();

            virtual void start() override;

            virtual void note_on(int iChannel, unsigned char uchNote, unsigned char uchVelocity) override;
            virtual void note_off(int iChannel, unsigned char uchNote, unsigned char uchVelocity) override;
            virtual void program_change(int iChannel, unsigned char uchProgram) override;
            virtual void control_change(int iChannel, unsigned char uchController, unsigned char uchValue) override;
            virtual void pitch_bend(int iChannel, unsigned short ushBend) override;
            virtual void sysex(const ::block & block) override;

            virtual void step();

            virtual void general_midi_reset() override;

            virtual void reset_all_controllers() override;

            virtual void add_short_message(byte * pmessage, int iSize);

            virtual bool is_ok() override;

         };


      } // namespace alsa


   } // namespace midi


} // namespace music



