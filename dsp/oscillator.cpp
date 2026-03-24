#include "./filters.cpp"
#include <cmath>
#include <numbers>

class Oscillator
{
public:
  void prepare(float sampleRate)
  {
    sampleRate_ = sampleRate;
    filter_.prepare(sampleRate_);
  }

  void process(uintptr_t leftPtr, uintptr_t rightPtr, size_t blockSize)
  {
    float* left = reinterpret_cast<float*>(leftPtr);
    float* right = reinterpret_cast<float*>(rightPtr);

    if (!isPlaying_) {
      for (size_t i = 0; i < blockSize; ++i) {
        left[i] = 0.0f;
        right[i] = 0.0f;
      }
      return;
    }

    for (size_t i = 0; i < blockSize; ++i) {
      float sample = 1 - 2 * phase_;
      left[i] = sample;
      right[i] = sample;

      phase_ += 1.0f / sampleRate_ * freq_;
      if (phase_ >= 1.0f)
        phase_ -= 1.0f;
    }

    filter_.applyFilter(left, right, blockSize, 500.0f);
  }

  void setIsPlaying(bool isPlaying) { isPlaying_ = isPlaying; }
  void setFreq(float freq) { freq_ = freq; }

private:
  float sampleRate_ = 44'100.0f;
  float freq_ = 220.0f;
  float phase_ = 0;
  bool isPlaying_ = false;

  float xhPrev_ = 0.0f;

  float pi = std::numbers::pi_v<float>;
  float tau_ = 2.0f * pi;

  CanonicalFilter filter_;
};
