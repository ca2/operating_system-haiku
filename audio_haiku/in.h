#pragma once
#include "audio/audio/wave/in.h"
#include <MediaRecorder.h>
namespace audio_haiku {
class in : virtual public ::wave::in {
public:
  BMediaRecorder *m_recorder = nullptr;
  media_node m_inputNode{};
  memsize m_offset = 0;
  ~in() override;
  void in_open(int buffers, int samples) override;
  void in_close() override;
  void in_start() override;
  void in_stop() override;
  void in_reset() override;
  static void record(void *cookie, bigtime_t timestamp, void *data,
                     size_t bytes, const media_format &format);
};
} // namespace audio_haiku
