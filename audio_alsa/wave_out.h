#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class  CLASS_DECL_AUDIO_ALSA wave_out :
         virtual public snd_pcm,
         virtual public ::wave::out
      {
      public:


         bool                    m_bWrite;
         int                     m_iBuffer;
         bool                    m_bStarted;
         uint64_t                m_uiStart;
         snd_pcm_status_t *      m_pstatus;


         wave_out();
         virtual ~wave_out();


         void install_message_routing(::channel * pchannel) override;

         virtual imedia_time out_get_position_millis();
         imedia_position out_get_position();

         //virtual ::estatus wave_out_open(::thread * pthreadCallback, ::count iBufferCount, ::count iBufferSampleCount) override;
         virtual ::estatus out_open_ex(::thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::wave::e_purpose epurpose) override;
         virtual ::estatus out_stop() override;
         virtual ::estatus out_close() override;
         virtual ::estatus out_pause() override;
         virtual ::estatus out_restart() override;
         snd_pcm_t * out_get_safe_PCM();

         //virtual void alsa_write_thread();

         virtual void out_on_playback_end() override;
         virtual void out_filled(index iBuffer) override;
         virtual bool alsa_should_play();

         virtual estatus init_thread() override;
         virtual void term_thread() override;

         virtual ::estatus out_start(const imedia_position & position);

         virtual int defer_underrun_recovery(int err);

         imedia_time out_get_position_millis_for_synch();


      };


   } // namespace audio_alsa


} // namespace multimedia



