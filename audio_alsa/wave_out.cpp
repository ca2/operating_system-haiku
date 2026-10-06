#include "platform.h"
#include "aura/message.h"


long long timestamp2ns(snd_htimestamp_t t)
{

   long long nsec;

   nsec = t.tv_sec * 1000000000;
   nsec += t.tv_nsec;

   return nsec;

}


long long timediff(snd_htimestamp_t t1, snd_htimestamp_t t2)
{

   long long nsec1, nsec2;

   nsec1 = timestamp2ns(t1);
   nsec2 = timestamp2ns(t2);

   return nsec1 - nsec2;

}


namespace multimedia
{


   namespace audio_alsa
   {


      wave_out::wave_out()
      {

         m_bDirectOutput      = false;
         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_estatusWave            = success;
         m_bWrite             = false;
         m_bStarted           = false;

         m_pstatus            = NULL;

         snd_pcm_status_malloc (&m_pstatus);

      }


      wave_out::~wave_out()
      {

         snd_pcm_status_free (m_pstatus);

      }


      void wave_out::install_message_routing(::channel * pchannel)
      {

         ::wave::out::install_message_routing(pchannel);

      }


      estatus wave_out::init_thread()
      {

         if(!::wave::out::init_thread())
         {

            return false;

         }

         return true;

      }


      void wave_out::term_thread()
      {

         ::wave::out::term_thread();

         thread::term_thread();

      }


      ::estatus wave_out::out_open_ex(thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::wave::e_purpose epurpose)
      {

         sync_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_open_ex");

         if(m_ppcm != NULL && m_estate != state_initial)
         {

            return success;

         }

//         m_dwPeriodTime =  20 * 1000; /* period time in us */

//         m_iBufferCountEffective = 4;

         if(epurpose == ::wave::purpose_playback)
         {

  //          m_dwPeriodTime =  6 * 1000; /* period time in us */

            //m_iBufferCountEffective = 16;

            m_frameCount = uiSamplesPerSec / 20;

            m_iBufferCount = 8;

            printf("::wave::purpose_playback %d\n", m_frameCount);

         }
         else
         {

            //m_frameCount = uiSamplesPerSec / 30;

            m_frameCount = 1024;

            m_iBufferCount = 4;

            printf("::wave::* %d\n", m_frameCount);

         }

//         m_iBufferCount = m_iBufferCountEffective;

         m_pthreadCallback = pthreadCallback;

         ASSERT(m_ppcm == NULL);

         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag        = 0;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = 0;

         if((m_estatusWave = this->snd_pcm_open(SND_PCM_STREAM_PLAYBACK)) != success)
         {

            return m_estatusWave;

         }

//         if(m_iBufferCount < m_iBufferCountEffective)
//         {
//
//            m_iBufferCountEffective = m_iBufferCount;
//
//         }

         //UINT uiBufferSize = snd_pcm_frames_to_bytes(m_ppcm, m_framesPeriodSize);

//         m_uiBufferTime = m_framesBuffer * 1000 * 1000 / uiSamplesPerSec;

         printf("snd_pcm_frames_to_bytes %d\n", m_frameCount);

         UINT uiBufferSize = snd_pcm_frames_to_bytes(m_ppcm, m_frameCount);

         out_get_buffer()->PCMOutOpen(this, uiBufferSize, m_iBufferCount, 128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(m_pwaveformat->nChannels, m_iBufferCount, m_frameCount);

//         m_pprebuffer->SetMinL1BufferCount(out_get_buffer()->GetBufferCount());

         int err;

         if((err = snd_pcm_sw_params_current(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("unable to determine current m_pswparams for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         snd_pcm_sframes_t framesThreshold = (m_iBufferCount - 1) * m_frameCount;

         // start the transfer when the buffer is almost full:
         if((err = snd_pcm_sw_params_set_start_threshold(m_ppcm, m_pswparams, framesThreshold)) < 0)
         {

            TRACE("unable to set start threshold mode for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         // allow the transfer when at least m_framesPeriodSize samples can be processed
         if((err = snd_pcm_sw_params_set_avail_min(m_ppcm, m_pswparams, m_frameCount)) < 0)
         {

            TRACE("unable to set avail min for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         // write the parameters to the playback device
         if((err = snd_pcm_sw_params(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("unable to set sw params for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         m_estate = state_opened;

         m_epurpose = epurpose;

         return success;

      }


      ::estatus wave_out::out_close()
      {

         sync_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_close");

         if(m_estate == state_playing)
         {

            out_stop();

         }

         if(m_estate != state_opened)
         {

            return success;

         }

         ::estatus mmr;

         mmr = this->snd_pcm_close();

         m_ppcm = NULL;

         ::wave::out::out_close();

         m_estate = state_initial;

         return success;

      }


      ::estatus wave_out::out_stop()
      {

         sync_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_stop");

         if(m_estate != state_playing && m_estate != state_paused)
         {

            return error_failed;

         }

         //m_pprebuffer->stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_drain(m_ppcm));

         if(m_estatusWave == success)
         {

            m_estate = state_opened;

         }

         return m_estatusWave;

      }


      ::estatus wave_out::out_pause()
      {

         sync_lock sl(mutex());

         ASSERT(m_estate == state_playing);

         TRACE("multimedia::audio_alsa::out_pause");

         if(m_estate != state_playing)
         {

            return error_failed;

         }

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_pause(m_ppcm, 1));

         ASSERT(m_estatusWave == success);

         if(m_estatusWave == success)
         {

            m_estate = state_paused;

         }

         return m_estatusWave;

      }


      ::estatus wave_out::out_restart()
      {

         sync_lock sl(mutex());

         ASSERT(m_estate == state_paused);

         TRACE("multimedia::audio_alsa::out_restart");

         if(m_estate != state_paused)
            return error_failed;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_pause(m_ppcm, 0));

         ASSERT(m_estatusWave == success);

         if(m_estatusWave == success)
         {
            m_estate = state_playing;
         }

         return m_estatusWave;

      }


      imedia_time wave_out::out_get_position_millis()
      {

         sync_lock sl(mutex());

         imedia_time time = 0;

         if(m_ppcm != NULL)
         {

            if(snd_pcm_status(m_ppcm, m_pstatus) == 0)
            {

               snd_htimestamp_t t;

               snd_pcm_status_get_htstamp (m_pstatus, &t);

               time = t.tv_sec * 1000 + t.tv_nsec / (1000 * 1000);

            }

         }

         if(time > 0)
         {

            output_debug_string("test");

         }

         return time;

      }


      imedia_position wave_out::out_get_position()
      {

         return out_get_position_millis();

      }


      void wave_out::out_on_playback_end()
      {

         TRACE("multimedia::audio_alsa::out_on_playback_end");

         ::wave::out::out_on_playback_end();

      }


      snd_pcm_t * wave_out::out_get_safe_PCM()
      {

         if(this == NULL)
         {

            return NULL;

         }

         return m_ppcm;

      }



      bool wave_out::alsa_should_play()
      {

         if(!::thread_get_run())
         {

            return false;

         }

         if(m_ppcm == NULL)
         {

            TRACE("alsa_out_buffer_ready m_ppcm == NULL");

            return false;

         }

         if(m_estate != ::wave::out::state_playing && m_estate != wave::out::state_stopping)
         {

            TRACE("alsa_out_buffer_ready failure: !playing && !stopping");

            return false;

         }

         return true;

      }


      void wave_out::out_filled(index iBuffer)
      {

         sync_lock sl(mutex());

         if(m_ppcm == NULL)
         {

            return;

         }

         snd_pcm_sframes_t iFramesToWrite = m_frameCount;

         int iBytesToWrite = snd_pcm_frames_to_bytes(m_ppcm, iFramesToWrite);

         ::estatus mmr = success;

         snd_pcm_sframes_t iFrameFreeCount = 0;

         if(false)
         {

            while(::thread_get_run())
            {

               iFrameFreeCount = snd_pcm_avail_update(m_ppcm);

               if(iFrameFreeCount == -EAGAIN)
               {

                  continue;

               }
               else if(iFrameFreeCount < 0)
               {

                  const char * pszError = snd_strerror(iFrameFreeCount);

                  TRACE("ALSA wave_out snd_pcm_avail error: %s (%d)\n", pszError, iFrameFreeCount);

                  iFrameFreeCount = defer_underrun_recovery(iFrameFreeCount);

                  if(iFrameFreeCount >= 0)
                  {

                     TRACE("ALSA wave_out snd_pcm_avail underrun recovery success (snd_pcm_avail)");

                     break;

                  }

                  TRACE("ALSA wave_out snd_pcm_avail minimum byte count %d\n", iFramesToWrite);

                  m_estate = state_opened;

                  m_estatusWave = error_failed;

                  TRACE("ALSA wave_out snd_pcm_avail error: %s\n", snd_strerror(iFrameFreeCount));

                  return;

               }
               else if(iFrameFreeCount >= iFramesToWrite)
               {

                  break;

               }

               usleep((iFramesToWrite - iFrameFreeCount) * 1'500'000 / m_pwaveformat->nSamplesPerSec);

            }

         }

         byte * pdata;

         memory m;

         if(iBuffer >= 0)
         {

            pdata = (byte *) out_get_buffer_data(iBuffer);

         }
         else
         {

            m.set_size(iBytesToWrite);

            m.zero();

            pdata = (byte *) m.get_data();

         }

         int iZero = 0;

         int iFramesJustWritten = 0;


         while (iBytesToWrite > 0)
         {

            iFramesJustWritten = snd_pcm_writei(m_ppcm, pdata, iFramesToWrite);

            iFramesToWrite -= iFramesJustWritten;

            m_bStarted = true;

            if(iFramesJustWritten == -EINTR)
            {

               TRACE("snd_pcm_writei -EINTR\n");

               continue;

            }
            else if(iFramesJustWritten == -EAGAIN)
            {

               //TRACE("snd_pcm_writei -EAGAIN\n");

               sl.unlock();

               snd_pcm_wait(m_ppcm, 100);

               sl.lock();

               continue;

            }
            else if(iFramesJustWritten < 0)
            {

               TRACE("snd_pcm_writei Underrun\n");

               TRACE("ALSA wave_out snd_pcm_writei error: %s (%d)\n", snd_strerror(iFramesJustWritten), iFramesJustWritten);

               iFramesJustWritten = defer_underrun_recovery(iFramesJustWritten);

               if(iFramesJustWritten < 0 && iFramesJustWritten != -EAGAIN)
               {

                  m_estate = state_opened;

                  m_estatusWave = error_failed;

                  TRACE("ALSA wave_out snd_pcm_writei couldn't recover from error: %s\n", snd_strerror(iFramesJustWritten));

                  return;

               }

               iFramesJustWritten = 0;

               continue;

            }

            int iBytesJustWritten = snd_pcm_frames_to_bytes(m_ppcm, iFramesJustWritten);

            if(iBuffer >= 0)
            {

               m_pprebuffer->m_position += iFramesJustWritten;

            }

            pdata += iBytesJustWritten;

            iBytesToWrite -= iBytesJustWritten;

            if(iBytesToWrite > 0)
            {

               sl.unlock();

               snd_pcm_wait(m_ppcm, 100);

               sl.lock();

            }

         }

         m_iBufferedCount++;

         sl.unlock();

         if(iBuffer >= 0)
         {

            m_psynththread->on_free(iBuffer);

         }

      }


      imedia_time wave_out::out_get_position_millis_for_synch()
      {

         if (m_pprebuffer.is_null())
         {

            return 0;

         }

         int64_t dwPosition = m_pprebuffer->m_position;

         dwPosition *= 1000;

         if (m_pwaveformat->nSamplesPerSec <= 0)
            return 0;

         dwPosition /= m_pwaveformat->nSamplesPerSec;

         return dwPosition;

      }


      ::estatus wave_out::out_start(const imedia_position & position)
      {

         sync_lock sl(mutex());

         if(m_estate == state_playing)
         {

            return success;

         }

         if(m_estate != state_opened && m_estate != state_stopped)
         {

            return error_failed;

         }

         int err = 0;

         if ((err = snd_pcm_prepare (m_ppcm)) < 0)
         {

            TRACE ("out_start: Cannot prepare audio interface for use (%s)\n",snd_strerror (err));

            return error_failed;

         }

         TRACE("out_start: snd_pcm_prepare OK");

         m_bStarted = false;

         m_estatusWave = ::wave::out::out_start(position);

         if(failed(m_estatusWave))
         {

            TRACE("out_start: ::wave::out::out_start FAILED");

            return m_estatusWave;

         }

//         m_pthreadWriter = fork([&]()
//         {
//
//            alsa_write_thread();
//
//         });

         TRACE("out_start: ::wave::out::out_start OK");

         return success;

      }


      int wave_out::defer_underrun_recovery(int err)
      {

         return snd_pcm_recover(m_ppcm, err, 0);

//         if (err == -EPIPE)
//         {
//
//            TRACE("underrun_recovery, going to snd_pcm_prepare: %s\n", snd_strerror(err));
//
//
//
//            if (err < 0)
//            {
//
//               TRACE("Can't recover from underrun, snd_pcm_prepare failed: %s\n", snd_strerror(err));
//
//            }
//
//         }
//         else if (err == -ESTRPIPE)
//         {
//
//            TRACE("underrun_recovery, snd_pcm_prepare returned -ESTRPIPE: %s\n", snd_strerror(err));
//
//            while ((err = snd_pcm_resume(m_ppcm)) == -EAGAIN)
//            {
//
//               TRACE("underrun_recovery, snd_pcm_resume returned -EAGAIN: %s\n", snd_strerror(err));
//
//               sleep(1); /* wait until the suspend flag is released */
//
//            }
//
//            if (err < 0)
//            {
//
//               err = snd_pcm_prepare(m_ppcm);
//
//               if (err < 0)
//               {
//
//                  TRACE("Can't recovery from suspend, snd_pcm_prepare failed: %s\n", snd_strerror(err));
//
//               }
//
//            }
//
//         }
//
//         return err;

      }


   } // namespace audio_alsa


} // namespace multimedia



