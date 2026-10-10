#include "platform.h"
#include "in.h"
#include "audio/audio/wave/buffer.h"
#include "audio/audio/wave/format.h"
#include "audio/audio/decode/encoder.h"
#include <MediaRoster.h>
namespace audio_haiku {
in::~in(){in_close();}
void in::in_open(int buffers,int samples){
 if(m_recorder)return;
 if(buffers<=0 || samples<=0)throw ::exception(error_bad_argument);
 defer_construct_newø(m_pwaveformat);
 auto &wave=m_pwaveformat->m_waveformat;
 if(!wave.nSamplesPerSec)wave.nSamplesPerSec=44100;
 if(!wave.nChannels)wave.nChannels=2;
 if(!wave.wBitsPerSample)wave.wBitsPerSample=16;
 if(wave.wBitsPerSample!=16 || wave.nChannels>8)throw ::exception(error_bad_argument,"Haiku capture requires 16-bit PCM with at most eight channels");
 wave.wFormatTag=1;wave.nBlockAlign=wave.nChannels*2;wave.nAvgBytesPerSec=wave.nSamplesPerSec*wave.nBlockAlign;
 media_format format{};format.type=B_MEDIA_RAW_AUDIO;format.u.raw_audio=media_raw_audio_format::wildcard;
 format.u.raw_audio.frame_rate=wave.nSamplesPerSec;format.u.raw_audio.channel_count=wave.nChannels;
 format.u.raw_audio.format=media_raw_audio_format::B_AUDIO_SHORT;format.u.raw_audio.byte_order=B_MEDIA_HOST_ENDIAN;
 format.u.raw_audio.buffer_size=samples*wave.nBlockAlign;
 auto *recorder=new BMediaRecorder("ca2 audio input",B_MEDIA_RAW_AUDIO);
 auto status=recorder->InitCheck();
 if(status==B_OK)status=recorder->SetHooks(record,nullptr,this);
 if(status==B_OK)status=BMediaRoster::Roster()->GetAudioInput(&m_inputNode);
 if(status==B_OK)status=recorder->Connect(m_inputNode,nullptr,&format);
 if(status!=B_OK){delete recorder;if(m_inputNode.node>0)BMediaRoster::Roster()->ReleaseNode(m_inputNode);m_inputNode={};throw ::exception(error_failed,"Cannot connect Haiku audio input: "+::as_string(status));}
 auto actual=recorder->Format().u.raw_audio;
 if(actual.format!=format.u.raw_audio.format || actual.channel_count!=wave.nChannels || actual.frame_rate!=wave.nSamplesPerSec){recorder->Disconnect();delete recorder;BMediaRoster::Roster()->ReleaseNode(m_inputNode);m_inputNode={};throw ::exception(error_failed,"Haiku audio input negotiated a different PCM format");}
 try{if(!in_get_buffer()->PCMOutOpen(this,samples*wave.nBlockAlign,buffers,128,m_pwaveformat,m_pwaveformat))throw ::exception(error_failed,"Cannot allocate Haiku capture buffers");}catch(...){recorder->Disconnect();delete recorder;BMediaRoster::Roster()->ReleaseNode(m_inputNode);m_inputNode={};throw;}
 m_recorder=recorder;m_iBuffer=0;m_offset=0;m_einstate=::wave::e_in_state_opened;
 if(m_pencoder)in_initialize_encoder();
}
void in::in_close(){auto *recorder=m_recorder;m_recorder=nullptr;if(recorder){recorder->Stop(true);recorder->Disconnect();delete recorder;in_get_buffer()->Close(this);}if(m_inputNode.node>0){BMediaRoster::Roster()->ReleaseNode(m_inputNode);m_inputNode={};}m_einstate=::wave::e_in_state_initial;}
void in::in_start(){if(!m_recorder)throw ::exception(error_wrong_state);auto status=m_recorder->Start();if(status!=B_OK)throw ::exception(error_failed,"Cannot start Haiku audio input");m_einstate=::wave::e_in_state_recording;}
void in::in_stop(){if(m_recorder)m_recorder->Stop(true);m_einstate=::wave::e_in_state_stopped;}
void in::in_reset(){in_stop();m_iBuffer=0;m_offset=0;}
void in::record(void *cookie,bigtime_t timestamp,void *data,size_t bytes,const media_format&){
 auto *self=static_cast<in*>(cookie);
 try {
  auto buffer=self->in_get_buffer();
  auto *source=static_cast<::u8*>(data);
  while(bytes){auto count=minimum(memsize(bytes),buffer->m_uiBufferSize-self->m_offset);::memory_copy(static_cast<::u8*>(buffer->get_buffer_data(self->m_iBuffer))+self->m_offset,source,count);source+=count;bytes-=count;self->m_offset+=count;if(self->m_offset==buffer->m_uiBufferSize){buffer->get_buffer(self->m_iBuffer)->OnMultimediaDone();if(self->m_pencoder)self->m_pencoder->EncoderWriteBuffer(buffer->get_buffer_data(self->m_iBuffer),buffer->m_uiBufferSize);self->m_listenerset.in_data_proc(self,::u32(timestamp/1000),self->m_iBuffer);self->m_iBuffer=(self->m_iBuffer+1)%buffer->GetBufferCount();self->m_offset=0;}}
 } catch(...){self->warning()<<"Haiku audio input callback failed";}
}
}


