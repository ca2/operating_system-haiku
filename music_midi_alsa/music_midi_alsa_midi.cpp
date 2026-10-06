#include "platform.h"


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         midi::midi()
         {

            m_strName = "alsa";

         }


         midi::~midi()
         {

         }


         ::estatus midi::enumerate_midi_devices()
         {

            return ::success;

   /*         UINT devs = midiInGetNumDevs();

            LOG("midiIn devices: %u", devs);

            for (UINT dev = 0; dev < devs; dev++)
            {

               MIDIINCAPSW caps = {};

               MMRESULT mmr = midiInGetDevCapsW(dev, &caps, sizeof(caps));

               if (MMSYSERR_NOERROR != mmr)
               {

                  return translate_os_result(mmr, "enumerate_midi_devices", "midiInGetDevCapsW");

               }

               m_uiaIn.add(dev);

               mmsystem_LogMidiInCaps(dev, caps);

               mmsystem_GetMidiInDeviceInterface(dev);

            }

            devs = midiOutGetNumDevs();

            LOG("midiOut devices: %u", devs);

            for (UINT dev = 0; dev < devs; dev++)
            {

               MIDIOUTCAPSW caps = {};

               MMRESULT mmr = midiOutGetDevCapsW(dev, &caps, sizeof(caps));

               if (MMSYSERR_NOERROR != mmr)
               {

                  return translate_os_result(mmr, "enumerate_midi_devices", "midiOutGetDevCaps");

               }

               mmsystem_LogMidiOutCaps(dev, caps);

               mmsystem_GetMidiOutDeviceInterface(dev);

            }

            return ::success;*/

         }




   //      void midi::mmsystem_GetMidiInDeviceInterface(UINT i)
     //    {

            // query the size of the device interface string
   /*         HMIDIIN h = reinterpret_cast<HMIDIIN>(i);
            ULONG size = 0;
            MMRESULT mmr = midiInMessage(
               h,
               DRV_QUERYDEVICEINTERFACESIZE,
               reinterpret_cast<DWORD_PTR>(&size),
               0
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiInMessage(DRV_QUERYDEVICEINTERFACESIZE) failed: mmr = 0x%08x", mmr);
               return;
            }

            if (0 == size) {
               LOG("No device interface");
               return;
            }
            if (size % sizeof(WCHAR)) {
               LOG("Device interface length in bytes (%u) should be a multiple of the size of a WCHAR!", size);
               return;
            }

            wstring wstr;
            wstr.alloc(size);




            mmr = midiInMessage(
               h,
               DRV_QUERYDEVICEINTERFACE,
               reinterpret_cast<DWORD_PTR>((wchar_t *) wstr),
               size
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiInMessage(DRV_QUERYDEVICEINTERFACE) failed: mmr = 0x%08x", mmr);
               return;
            }

            LOG("    Device interface: \"%S\"", wstr);*/
       //  }

         //void midi::mmsystem_GetMidiOutDeviceInterface(UINT i)
         //{

            // query the size of the device interface string
   /*         HMIDIOUT h = reinterpret_cast<HMIDIOUT>(i);
            ULONG size = 0;
            MMRESULT mmr = midiOutMessage(
               h,
               DRV_QUERYDEVICEINTERFACESIZE,
               reinterpret_cast<DWORD_PTR>(&size),
               0
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiOutMessage(DRV_QUERYDEVICEINTERFACESIZE) failed: mmr = 0x%08x", mmr);
               return;
            }

            if (0 == size) {
               LOG("No device interface");
               return;
            }
            if (size % sizeof(WCHAR)) {
               LOG("Device interface length in bytes (%u) should be a multiple of the size of a WCHAR!", size);
               return;
            }

            wstring wstr;
            wstr.alloc(size);

            mmr = midiOutMessage(
               h,
               DRV_QUERYDEVICEINTERFACE,
               reinterpret_cast<DWORD_PTR>((wchar_t *) wstr),
               size
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiOutMessage(DRV_QUERYDEVICEINTERFACE) failed: mmr = 0x%08x", mmr);
               return;
            }

            LOG("    Device interface: \"%S\"", wstr);*/
         //}

         //void midi::mmsystem_LogMidiInCaps(UINT i, MIDIINCAPSW caps)
         //{
   /*         LOG(
               "-- %u: %S --\n"
               "    Device ID: %u\n"
               "    Manufacturer identifier: %u\n"
               "    Product identifier: %u\n"
               "    Driver version: %u.%u\n"
               "    Product name: %S\n"
               "    Support: 0x%x"
               ,
               i,
               caps.szPname ? caps.szPname : L"(no name)",
               i,
               caps.wMid,
               caps.wPid,
               caps.vDriverVersion / 256, caps.vDriverVersion % 256,
               caps.szPname ? caps.szPname : L"(no name)",
               caps.dwSupport
               );
            string strName;
            if (caps.szPname != NULL)
            {
               strName.Format("%s (%d)", caps.szPname, i);
            }
            else
            {
               strName.Format("(no name midi In device) (%d)", i);
            }
            m_straIn.add(strName);*/
         //}

         //void midi::mmsystem_LogMidiOutCaps(UINT i, MIDIOUTCAPSW caps)
         //{
   /*         LOG(
               "-- %u: %S --\n"
               "    Device ID: %u\n"
               "    Manufacturer identifier: %u\n"
               "    Product identifier: %u\n"
               "    Driver version: %u.%u\n"
               "    Product name: %S\n"
               "    Technology: %u (%S)\n"
               "    Voices: %u\n"
               "    Notes: %u\n"
               "    Channel mask: 0x%x\n"
               "    Support: 0x%x"
               "%S%S%S%S"
               ,
               i,
               caps.szPname ? caps.szPname : L"(no name)",
               i,
               caps.wMid,
               caps.wPid,
               caps.vDriverVersion / 256, caps.vDriverVersion % 256,
               caps.szPname ? caps.szPname : L"(no name)",
               caps.wTechnology,
               caps.wTechnology == MOD_MIDIPORT ? L"MOD_MIDIPORT" :
               caps.wTechnology == MOD_SYNTH ? L"MOD_SYNTH" :
               caps.wTechnology == MOD_SQSYNTH ? L"MOD_SQSYNTH" :
               caps.wTechnology == MOD_FMSYNTH ? L"MOD_FMSYNTH" :
               caps.wTechnology == MOD_MAPPER ? L"MOD_MAPPER" :
               caps.wTechnology == MOD_WAVETABLE ? L"MOD_WAVETABLE" :
               caps.wTechnology == MOD_SWSYNTH ? L"MOD_SWSYNTH" :
               L"Unrecognized",
               caps.wVoices,
               caps.wNotes,
               caps.wChannelMask,
               caps.dwSupport,
               (caps.dwSupport & MIDICAPS_CACHE) ? L"\n        MIDICAPS_CACHE" : L"",
               (caps.dwSupport & MIDICAPS_LRVOLUME) ? L"\n        MIDICAPS_LRVOLUME" : L"",
               (caps.dwSupport & MIDICAPS_STREAM) ? L"\n        MIDICAPS_STREAM" : L"",
               (caps.dwSupport & MIDICAPS_VOLUME) ? L"\n        MIDICAPS_VOLUME" : L""
               );
            string strName;
            if (caps.szPname != NULL)
            {
               strName.Format("%S (%d)", caps.szPname, i);
            }
            else
            {
               strName.Format("(no name midi Out device) (%d)", i);
            }
            m_straOut.add(strName);*/
         //}
      __pointer(::music::midi::message_out) midi::get_message_out(const string& strDevice)
      {


         string strEngine = device_engine(strDevice);

         if (strEngine.compare_ci("real_time") == 0)
         {

            return m_pmidiRealTime->get_message_out(strDevice);

         }


         return new message_out(this, strDevice);

      }


      __pointer(::music::midi::sequencer) midi::create_midi_sequencer(sequence * psequence, const string& strDevice)
      {


         if (strDevice.compare_ci("alsa") == 0)
         {

            return __new(::music::midi::sequencer(psequence, get_message_out(strDevice)));

         }

         auto psequencer = ::music::midi::midi::create_midi_sequencer(psequence, strDevice);

         if(psequencer)
         {

            return psequencer;

         }

         return __new(::music::midi::sequencer(psequence, get_message_out(strDevice)));

      }


      ::estatus midi::translate_os_result(string & strMessage, string & strOsMessage, ::music::midi::object * pobject, int64_t iOsResult, const string & strContext, const string & strText)
         {

            ::estatus mmrc = iOsResult == 0 ? ::success : ::error_failed;

            if (mmrc == ::success)
            {

               return ::success;

            }

            return mmrc;

         }


         bool midi::Initialize()
         {


            if(!::music::midi::midi::Initialize())
            {

               return false;

            }

         	int card, err;

            card = -1;

            if ((err = snd_card_next(&card)) < 0)
            {

               TRACE("cannot determine card number: %s", snd_strerror(err));

               return false;

            }

            if (card < 0)
            {

               TRACE("no sound card found");

               return false;

            }


            do
            {

               list_midi_out_card_devices(card);


               if ((err = snd_card_next(&card)) < 0)
               {

                  TRACE("cannot determine card number: %s", snd_strerror(err));

                  break;

               }

            } while (card >= 0);

            return true;

         }


         bool midi::list_midi_out_card_devices(int card)
         {

            snd_ctl_t *ctl;
            char name[32];
            int device;
            int err;

            sprintf(name, "hw:%d", card);

            if ((err = snd_ctl_open(&ctl, name, 0)) < 0)
            {

               TRACE("cannot open control for card %d: %s", card, snd_strerror(err));

               return false;
            }

            device = -1;

            for (;;)
            {

                           list_midi_out_devices(ctl, card, device);


               if ((err = snd_ctl_rawmidi_next_device(ctl, &device)) < 0)
               {

                  TRACE("cannot determine device number: %s", snd_strerror(err));

                  break;

               }

               if (device < 0)
               {

                  break;

               }


            }

            snd_ctl_close(ctl);

            return true;

         }


         bool midi::list_midi_out_devices(snd_ctl_t *ctl, int card, int device)
         {

            snd_rawmidi_info_t *info;
            const char *name;
            const char *sub_name;
            int subs;
            int sub;
            int err;

            snd_rawmidi_info_alloca(&info);

            snd_rawmidi_info_set_device(info, device);

            snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);

            err = snd_ctl_rawmidi_info(ctl, info);

            if (err >= 0)
            {

               subs = snd_rawmidi_info_get_subdevices_count(info);

            }
            else
            {

               subs = 0;

            }

            if (!subs)
            {

               return false;

            }

            for (sub = 0; sub < subs; ++sub)
            {

               snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);

               snd_rawmidi_info_set_subdevice(info, sub);

               err = snd_ctl_rawmidi_info(ctl, info);

               if (err < 0)
               {

                  TRACE("cannot get rawmidi information %d:%d:%d: %s\n", card, device, sub, snd_strerror(err));

                  return false;

               }

               name = snd_rawmidi_info_get_name(info);

               sub_name = snd_rawmidi_info_get_subdevice_name(info);

               if (sub == 0 && sub_name[0] == '\0')
               {

                  string strDev;

                  strDev.Format("hw:%d,%d", card, device);

                  printf("hw:%d,%d    %s", card, device, name);

                  add_midi_out_device(name, "alsa:" + strDev);

                  break;

               }
               else
               {

                  string strDev;

                  strDev.Format("hw:%d,%d,%d", card, device, sub);

                  printf("hw:%d,%d,%d  %s\n",  card, device, sub, sub_name);

                  add_midi_out_device(sub_name, "alsa:" + strDev);



               }

            }

            return true;

         }


      } // namespace alsa


   } // namespace midi


} // namespace music



