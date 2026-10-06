#pragma once


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_ALSA midi :
            virtual public ::music::midi::midi
         {
         public:


            //base_library                 m_library;
            //midi_listener_set             m_listenerptra;
            //uint32_t                      m_uiMidiOutDevice;


            midi();
            virtual ~midi();


            virtual ::estatus enumerate_midi_devices();


            //void RemoveListener(midi_listener * plistener);
            //void AddListener(midi_listener * plistener);
            //uint32_t GetMidiOutDevice();
            //void SetMidiOutDevice(uint32_t uiDevice);
            //bool Initialize();


            //bool IsSoftKaraokeFile(const char * lpszPathName);
            //bool IsXFFile(const char * lpcszPathName);


            //virtual bool veriwell_multimedia_music_midi_factory();


   /*            void mmsystem_LogMidiInCaps(UINT i, MIDIINCAPSW caps);
            void mmsystem_GetMidiInDeviceInterface(UINT i);
            void mmsystem_LogMidiOutCaps(UINT i, MIDIOUTCAPSW caps);
            void mmsystem_GetMidiOutDeviceInterface(UINT i);*/


            virtual __pointer(::music::midi::message_out) get_message_out(const string& strDevice) override;
            virtual __pointer(::music::midi::sequencer) create_midi_sequencer(sequence * psequence, const string& strDevice) override;


            using ::music::midi::midi::translate_os_result;
            virtual ::estatus translate_os_result(string & strMessage, string & strOsMessage, ::music::midi::object * pobject, int64_t iOsResult, const string & strContext, const string & strText);


            virtual bool Initialize();

            virtual bool list_midi_out_card_devices(int card);

            virtual bool list_midi_out_devices(snd_ctl_t *ctl, int card, int device);


         };


      } // namespace alsa


   } // namespace midi


} // namespace music




