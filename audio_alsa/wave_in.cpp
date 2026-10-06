#include "platform.h"


namespace multimedia
{


   namespace audio_alsa
   {


      wave_in::wave_in()
      {

         m_pencoder = NULL;
         m_estate = state_initial;
         m_bResetting = false;


      }

      wave_in::~wave_in()
      {
      }


      estatus wave_in::init_thread()
      {
         TRACE("wave_in::initialize_instance %X\n", get_os_int());
         //SetMainWnd(NULL);
         //ASSERT(GetMainWnd() == NULL);
         set_thread_priority(::priority_highest);
         //m_evInitialized.SetEvent();
         return true;
      }


      void wave_in::term_thread()
      {

         m_eventExitInstance.SetEvent();

         thread::term_thread();

      }


      ::estatus wave_in::in_open(int32_t iBufferCount, int32_t iBufferSampleCount)
      {

         return success;

         if(m_ppcm != NULL && m_estate != state_initial)
         {

            in_initialize_encoder();

            return success;

         }

         single_lock sLock(mutex(), TRUE);

         ::estatus mmr = success;

         ASSERT(m_ppcm == NULL);

         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag = 0;
         m_pwaveformat->nChannels = 2;
         m_pwaveformat->nSamplesPerSec = 44100;
         m_pwaveformat->wBitsPerSample = sizeof(::wave::WAVEBUFFERDATA) * 8;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;

         return error_failed;

         if(snd_pcm_open(SND_PCM_STREAM_CAPTURE) != 0)
            return error_failed;


         goto Opened;
         //m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         //m_pwaveformat->nChannels = 2;
         //m_pwaveformat->nSamplesPerSec = 44100;
         //m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8;
         //m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         //m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         //m_pwaveformat->cbSize = 0;
         //sp(::multimedia::audio::wave) audiowave = Application.audiowave();
         //m_iBuffer = 0;

         /*if(success == (mmr = waveInOpen(
            &m_hwavein,
            audiowave->m_uiWaveInDevice,
            wave_format(),
            get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 22050;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if(success == (mmr = waveInOpen(
            &m_hwavein,
            WAVE_MAPPER,
            wave_format(),
            (uint32_t) get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 11025;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if(success == (mmr = waveInOpen(
            &m_hwavein,
            WAVE_MAPPER,
            wave_format(),
            (uint32_t) get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;

         if(mmr !=success)
         {
            if(mmr == MMSYSERR_ALLOCATED)
            {
               TRACE("Specified resource is already allocated.");
            }
            else if(mmr == MMSYSERR_BADDEVICEID)
            {
               TRACE("Specified device identifier is out of range.");
            }
            else if(mmr == WAVERR_BADFORMAT)
            {
               TRACE("Attempted to open with an unsupported waveform-audio_alsa format.");
            }
            TRACE("ERROR OPENING WAVE INPUT DEVICE");
            return mmr;
         }*/

Opened:
         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;

         if(m_pwaveformat->nSamplesPerSec == 44100)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            if(iBufferCount > 0)
            {
               uiAllocationSize = iBufferCount * uiBufferSize;
            }
            else
            {
               uiAllocationSize = 8 * uiAnalysisSize;
            }
            uiInterestSize = 200;
            uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 9;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 16 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 600;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 9;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 8 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 600;
            uiSkippedSamplesCount = 1;
         }
         in_get_buffer()->FFTOpen(
            uiAllocationSize,
            uiBufferSize,
            uiAnalysisSize,
            uiInterestSize,
            uiSkippedSamplesCount);

         /*int32_t i, iSize;

         iSize = (int32_t) in_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            if(success != (mmr =  waveInPrepareHeader(m_hwavein, create_new_WAVEHDR(in_get_buffer(), i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
               return mmr;
            }

            in_add_buffer(i);

         }*/

         if(m_pencoder != NULL && !in_initialize_encoder())
         {

            m_estate = state_opened;

            in_close();

            return (::estatus) -1;

         }

         m_estate = state_opened;

         return success;

      }


      ::estatus wave_in::in_close()
      {

         single_lock sLock(mutex(), TRUE);

         ::estatus mmr;

         if(m_estate != state_opened && m_estate != state_stopped)
            return success;

         mmr = in_reset();

         //int32_t i, iSize;

         //iSize = (int32_t) in_get_buffer()->GetBufferCount();

         /*for(i = 0; i < iSize; i++)
         {

            if(success != (mmr = waveInUnprepareHeader(m_hwavein, wave_hdr(i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer");
               //return mmr;
            }

            delete wave_hdr(i);

         }*/

         mmr = snd_pcm_close();

         m_ppcm = NULL;

         m_estate = state_initial;

         return success;

      }


      ::estatus wave_in::in_start()
      {

         return success;

         single_lock sLock(mutex(), TRUE);

         if(m_estate == state_recording)
         {

            return success;

         }

         if(m_estate != state_opened && m_estate != state_stopped)
         {

            return success;

         }

         ::estatus mmr;

         if((mmr = translate_alsa(snd_pcm_start(m_ppcm))) != success)
         {

            TRACE("ERROR starting INPUT DEVICE ");

            return mmr;

         }

         m_estate = state_recording;

         return success;

      }


      ::estatus wave_in::in_stop()
      {

         sync_lock sl(mutex());

         if(m_estate != state_recording)
         {

            return error_failed;

         }

         ::estatus mmr;

         m_estate = state_stopping;

         try
         {

            if(success != (mmr = translate_alsa(snd_pcm_drain(m_ppcm))))
            {

               TRACE("wave_in::in_stop : ERROR OPENING stopping INPUT DEVICE ");

            }

         }
         catch(...)
         {

            TRACE("wave_in::in_stop : Exception OPENING stopping INPUT DEVICE ");

         }

         m_estate = state_stopped;

         m_eventStopped.SetEvent();

         return success;

      }


      ::estatus wave_in::run()
      {

         int iBuffer = 0;
         int iSize;
         int err;

         while(thread_get_run())
         {

            if(in_is_recording() && in_get_buffer()->GetBufferCount() > 0)
            {

               iSize = (int32_t) in_get_buffer()->GetBufferCount();

               if(iBuffer >= iSize)
                  iBuffer = 0;

               if(iBuffer < iSize)
               {

                  if ((err = snd_pcm_readi (m_ppcm, in_get_buffer()->get_buffer_data(iBuffer), in_get_buffer()->m_uiBufferSize)) == in_get_buffer()->m_uiBufferSize)
                  {
                     in_proc(m_ppcm, 0, 0, iBuffer, 0);
                  }
                  else
                  {
                     TRACE("read from audio interface failed (%s)\n", snd_strerror (err));
                  }

               }

            }
            else
            {
                Sleep(5);
            }

         }

         return success;

      }


      void CALLBACK wave_in::in_proc(snd_pcm_t * p, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
      {

         m_iBuffer--;

         uint32_t msSampleTime = Audio.multimedia_get_time();

         int iBuffer = dwParam1;

         in_get_buffer()->get_buffer(iBuffer)->OnMultimediaDone();

         m_listenerset.in_data_proc(this, msSampleTime, iBuffer);

         if(m_pencoder != NULL)
         {
            m_pencoder->EncoderWriteBuffer(in_get_buffer()->get_buffer_data(iBuffer), in_get_buffer()->m_uiBufferSize);
         }

         if(!in_is_resetting() && in_is_recording())
         {
            in_add_buffer(iBuffer);
         }

      }


      ::estatus wave_in::in_reset()
      {
         sync_lock sl(mutex());
         m_bResetting = true;
         if(m_ppcm == NULL)
         {
            return error_failed;
         }

         ::estatus mmr;
         if(m_estate == state_recording)
         {
            if(success != (mmr = in_stop()))
            {
               TRACE("wave_in::Reset error stopping input device");
               return mmr;
            }
         }
         try
         {
            if(success != (mmr = translate_alsa(snd_pcm_drop(m_ppcm))))
            {
               TRACE("wave_in::Reset error resetting input device");
               return mmr;
            }
         }
         catch(...)
         {
         }

         m_estate = state_opened;

         m_bResetting = false;

         return success;

      }


/*

      ::estatus wave_in::in_add_buffer(int32_t iBuffer)
      {

         return in_add_buffer(wave_hdr(iBuffer));

      }


      ::estatus wave_in::in_add_buffer(LPWAVEHDR lpwavehdr)
      {

         ::estatus mmr;

         /*if(success != (mmr = waveInAddBuffer(m_hwavein, lpwavehdr, sizeof(WAVEHDR))))
         {

            TRACE("ERROR OPENING Adding INPUT DEVICE buffer");

         }*/

/*         m_iBuffer++;

         return mmr;

      }
*/

      bool wave_in::in_initialize_encoder()
      {

         if(m_pencoder == NULL)
            return false;

         if(!::wave::in::in_initialize_encoder())
            return false;

         return true;

      }

/*      WAVEFORMATEX * wave_in::wave_format()
      {

         translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }
*/
      snd_pcm_t * wave_in::in_get_safe_PCM()
      {

         if(this == NULL)
            return NULL;

         return m_ppcm;

      }

      void * wave_in::get_os_data()
      {
         return m_ppcm;
      }

/*      LPWAVEHDR wave_in::wave_hdr(int iBuffer)
      {
         return ::multimedia::audio_alsa::get_os_data(in_get_buffer(), iBuffer);
      }*/


   } // namespace audio_alsa


} // namespace multimedia






