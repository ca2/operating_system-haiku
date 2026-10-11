#pragma once
#include "audio/audio/wave/buffered_wave_out.h"
#include <SoundPlayer.h>
#include <Locker.h>
namespace audio_haiku {
class out : virtual public ::wave::buffered_wave_out {
public:
  BSoundPlayer *m_player = nullptr;
  BLocker m_queueLock;
  ::memory m_queue;
  memsize m_read = 0, m_write = 0, m_queued = 0;
  int m_frameBytes = 0, m_nativeBufferBytes = 0;
  ::u8 m_silence = 0;
  ::f64 m_outputGain = 1.0;
  ~out() override;
  ::string default_audio_device() override;
  int device_open(int precision, ::u32 rate, unsigned char channels) override;
  int device_close() override;
  int device_pause() override;
  int device_resume() override;
  memsize device_write(const void *data, memsize bytes) override;
  memsize device_queued_bytes() override;
  int device_minimum_queue_frames() override;
  ::string device_error_message(int error) override;
  static void fill(void *cookie, void *buffer, size_t size,
                   const media_raw_audio_format &format);
};
} // namespace audio_haiku
