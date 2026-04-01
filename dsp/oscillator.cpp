#include "./oscillator.h"

void
Oscillator::prepare(float sampleRate, size_t blockSize)
{
  sampleRate_ = sampleRate;
  blockSize_ = blockSize;
  antiAliasFilter_.prepare(sampleRate_ * 2.0f, blockSize_ * 2);
  overSampledBuffer.resize(blockSize_ * 2, 0.0f);
}

void
Oscillator::process(float* channel)
{

  for (size_t i = 0; i < blockSize_ * 2; ++i) {
    float sample = 1 - 2 * phase_;
    sample += polyBlep(1.0f / sampleRate_ * freq_, phase_);
    overSampledBuffer[i] = sample;

    phase_ += 1.0f / (sampleRate_ * 2) * freq_;
    if (phase_ >= 1.0f)
      phase_ -= 1.0f;
  }

  antiAliasFilter_.applyFilter(overSampledBuffer.data(), 20000, 0.707, LP);

  for (size_t i = 0; i < blockSize_; ++i) {
    channel[i] = overSampledBuffer[i * 2];
  }
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
Oscillator::setFreq(float freq)
{
  freq_ = freq;
}
