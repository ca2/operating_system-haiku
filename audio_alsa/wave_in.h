#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class  CLASS_DECL_AUDIO_ALSA wave_in :
         virtual public snd_pcm,
         virtual public ::wave::in
      {
      public:


         wave_in();
         virtual ~wave_in();


         virtual bool in_initialize_encoder();

         //virtual ::estatus in_add_buffer(int32_t iBuffer);
         //virtual ::estatus in_add_buffer(LPWAVEHDR lpwavehdr);

         snd_pcm_t * in_get_safe_PCM();
         virtual void * get_os_data();

         ::estatus in_open(int32_t iBufferCount, int32_t iBufferSampleCount);
         ::estatus in_close();
         ::estatus in_stop();
         ::estatus in_start();
         ::estatus in_reset();

         //virtual void translate_in_message(::signal_details * pobj);

         virtual ::estatus init_thread() override;
         virtual void term_thread() override;
         //virtual void pre_translate_message(::signal_details * pobj);

         void CALLBACK in_proc(snd_pcm_t * hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

         virtual ::estatus run() override;

         //WAVEFORMATEX * wave_format();
         //LPWAVEHDR wave_hdr(int iBuffer);


      };


   } // namespace audio_alsa


} // namespace multimedia




