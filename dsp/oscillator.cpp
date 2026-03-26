#include "./oscillator.h"

void
Oscillator::prepare(float sampleRate)
{
  sampleRate_ = sampleRate;
  filter_.prepare(sampleRate_);
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

  for (size_t i = 0; i < blockSize; ++i) {
    float sample = 1 - 2 * phase_;
    left[i] = sample;
    right[i] = sample;

    phase_ += 1.0f / sampleRate_ * freq_;
    if (phase_ >= 1.0f)
      phase_ -= 1.0f;
  }

  filter_.applyFilter(left, right, blockSize, cutoffFreq_, q_, filterType_);
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
