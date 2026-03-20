#include <cmath>
#include <cstddef>
#include <emscripten/bind.h>
#include <numbers>

class Oscillator
{
public:
  void process(float* output)
  {
    if (!isPlaying_)
      return;

    for (size_t i = 0; i < blockSize_; ++i) {
      output[i] = std::sin(tau_ * freq_ * phase_);
      phase_ += i / sampleRate_;
      if (phase_ >= tau_)
        phase_ = 0.0f;
    }
  }

  void setIsPlaying(bool isPlaying) { isPlaying_ = isPlaying; }

private:
  float sampleRate_ = 44'100.0f;
  float freq_ = 220.0f;
  float phase_ = 0;
  size_t blockSize_ = 128;
  bool isPlaying_ = false;
  float tau_ = 2.0f * std::numbers::pi_v<float>;
};

EMSCRIPTEN_BINDINGS(dsp_module)
{
  class_<Oscillator>("Oscillator")
    .constructor()
    .function("setIsPlaying", &Oscillator::setIsPlaying)
    .function("process", &Oscillator::process);
}
