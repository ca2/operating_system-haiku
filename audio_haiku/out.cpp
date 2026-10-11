#include "platform.h"
#include "out.h"
#include <Autolock.h>
#include <MediaRoster.h>
#include <errno.h>
#include <string.h>
namespace audio_haiku {
// Haiku errno values are negative; buffered_wave_out expects positive error codes.
static int device_error(int error){return error<0?-error:error;}

out::~out(){device_close();}
::string out::default_audio_device(){return "Haiku default audio output";}
int out::device_open(int precision,::u32 rate,unsigned char channels){
 if(!rate || !channels || channels>8 || (precision!=8 && precision!=16 && precision!=32))return device_error(EINVAL);
 device_close();
 status_t rosterStatus;
 auto *roster=BMediaRoster::Roster(&rosterStatus);
 if(!roster || rosterStatus!=B_OK)return device_error(rosterStatus?rosterStatus:ENODEV);
 media_node output;
 auto outputStatus=roster->GetAudioOutput(&output);
 if(outputStatus==B_BAD_INDEX || outputStatus==B_NAME_NOT_FOUND)return device_error(ENODEV);
 if(outputStatus!=B_OK)return outputStatus<0?-outputStatus:outputStatus;
 roster->ReleaseNode(output);
 media_raw_audio_format format=media_raw_audio_format::wildcard;
 format.frame_rate=rate;format.channel_count=channels;
 format.format=precision==8?media_raw_audio_format::B_AUDIO_UCHAR:precision==16?media_raw_audio_format::B_AUDIO_SHORT:media_raw_audio_format::B_AUDIO_INT;
 format.byte_order=B_MEDIA_HOST_ENDIAN;format.buffer_size=4096;
 m_frameBytes=channels*precision/8;m_silence=precision==8?128:0;
 m_player=new BSoundPlayer(&format,"ca2 audio output",fill,nullptr,this);
 auto status=m_player->InitCheck();
 if(status!=B_OK){delete m_player;m_player=nullptr;return status<0?-status:status;}
 auto actual=m_player->Format();
 if(actual.channel_count!=channels || actual.format!=format.format || actual.frame_rate!=rate){device_close();return device_error(EINVAL);}
 m_nativeBufferBytes=actual.buffer_size;
 m_queue.set_size(maximum(memsize(m_nativeBufferBytes)*8,memsize(m_frameBytes)*32768));
 m_read=m_write=m_queued=0;
 // Haiku can retain a mixer's input gain between clients with the same name.
 // Initialize this stream explicitly instead of inheriting a stale mute.
 m_outputGain=global_volume() * minimum(1.0, maximum(0.0, m_dVolume));
 m_player->SetVolume(float(m_outputGain));
 m_player->SetHasData(true);
 return 0;
}
int out::device_close(){auto *player=m_player;m_player=nullptr;if(player){player->Stop(true,true);delete player;}BAutolock lock(&m_queueLock);m_queue.set_size(0);m_read=m_write=m_queued=0;return 0;}
int out::device_pause(){if(!m_player)return device_error(ENODEV);m_player->Stop(true,false);return 0;}
int out::device_resume(){if(!m_player)return device_error(ENODEV);auto status=m_player->Start();return status<0?-status:status;}
memsize out::device_write(const void *data,memsize bytes){
 if(bytes<0 || (!data && bytes>0))return -device_error(EINVAL);
 if(bytes==0)return 0;
 BAutolock lock(&m_queueLock);
 if(!m_player || m_queue.is_empty())return -device_error(ENODEV);
 auto count=minimum(bytes,m_queue.size()-m_queued);count-=count%m_frameBytes;
 auto first=minimum(count,m_queue.size()-m_write);
 ::memory_copy(m_queue.data()+m_write,data,first);
 ::memory_copy(m_queue.data(),static_cast<const ::u8*>(data)+first,count-first);
 m_write=(m_write+count)%m_queue.size();m_queued+=count;return count;
}
memsize out::device_queued_bytes(){BAutolock lock(&m_queueLock);return m_queued;}
int out::device_minimum_queue_frames(){return m_frameBytes?maximum(1024,m_nativeBufferBytes/m_frameBytes*4):1024;}
::string out::device_error_message(int error){if(error==device_error(ENODEV))return "Haiku has no registered default audio output. Check Media preferences.";return ::string(strerror(-error));}
void out::fill(void *cookie,void *buffer,size_t size,const media_raw_audio_format&){
 auto *self=static_cast<out*>(cookie);
 ::memory_set(buffer,self->m_silence,size);
 BAutolock lock(&self->m_queueLock);
 if(self->m_queue.is_empty())return;
 auto count=minimum(memsize(size),self->m_queued);
 auto first=minimum(count,self->m_queue.size()-self->m_read);
 ::memory_copy(buffer,self->m_queue.data()+self->m_read,first);
 ::memory_copy(static_cast<::u8*>(buffer)+first,self->m_queue.data(),count-first);
 self->m_read=(self->m_read+count)%self->m_queue.size();self->m_queued-=count;
 // Mixer gains have a floor on Haiku; zero must still mean complete silence.
 if(self->m_outputGain==0.0)::memory_set(buffer,self->m_silence,size);
}
}


