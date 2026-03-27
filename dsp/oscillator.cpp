#include "./oscillator.h"

void
Oscillator::prepare(float sampleRate)
{
  sampleRate_ = sampleRate;
  filter_.prepare(sampleRate_);
  antiAliasFilter_.prepare(sampleRate_ * 2.0f);
}

void
Oscillator::process(uintptr_t leftPtr, uintptr_t rightPtr, size_t blockSize)
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

  std::vector<float> overSampled(blockSize * 2);

  for (size_t i = 0; i < blockSize * 2; ++i) {
    float sample = 1 - 2 * phase_;
    sample += polyBlep(1.0f / sampleRate_ * freq_, phase_);
    overSampled[i] = sample;

    phase_ += 1.0f / (sampleRate_ * 2) * freq_;
    if (phase_ >= 1.0f)
      phase_ -= 1.0f;
  }

  antiAliasFilter_.applyFilter(
    overSampled.data(), overSampled.data(), blockSize * 2, 20000, 0.707, LP);

  for (size_t i = 0; i < blockSize; ++i) {
    left[i] = overSampled[i * 2];
    right[i] = left[i];
  }

  filter_.applyFilter(left, right, blockSize, cutoffFreq_, q_, filterType_);
}

float
Oscillator::polyBlep(float dt, float t)
{
  if (t < dt) {
    t /= dt;
    return t + t - t * t - 1.0f;
  } else if (t > 1.0f - dt) {
    t = (t - 1.0f) / dt;
    return t * t + t + t + 1.0;
  }
  return 0.0f;
}

void
Oscillator::setIsPlaying(bool isPlaying)
{
  isPlaying_ = isPlaying;
}

void
Oscillator::setFreq(float freq)
{
  freq_ = freq;
}

void
Oscillator::setCutoffFreq(float freq)
{
  cutoffFreq_ = freq;
}

void
Oscillator::setQ(float q)
{
  q_ = q;
}

void
Oscillator::setFilterType(FilterType filterType)
{
  filterType_ = filterType;
}

void
Oscillator::resetFilter()
{
  filter_.reset();
}
