#include <cmath>
#include <emscripten/bind.h>
#include <numbers>

class Oscillator
{
public:
  void prepare(float sampleRate) { sampleRate_ = sampleRate; }

  void process(uintptr_t leftPtr, uintptr_t rightPtr)
  {
    float* left  = reinterpret_cast<float*>(leftPtr);
    float* right = reinterpret_cast<float*>(rightPtr);

    if (!isPlaying_) {
      for (size_t i = 0; i < blockSize_; ++i) {
        left[i]  = 0.0f;
        right[i] = 0.0f;
      }
      return;
    }

    for (size_t i = 0; i < blockSize_; ++i) {
      float sample = std::sin(tau_ * freq_ * phase_);
      left[i]  = sample;
      right[i] = sample;
      phase_ += 1.0f / sampleRate_;
      if (phase_ >= 1.0f)
        phase_ -= 1.0f;
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
  emscripten::class_<Oscillator>("Oscillator")
    .constructor()
    .function("prepare", &Oscillator::prepare)
    .function("setIsPlaying", &Oscillator::setIsPlaying)
    .function("process", &Oscillator::process);
}
