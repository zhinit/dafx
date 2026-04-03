#include "./audio_processor.h"

void
AudioProcessor::prepare(float sampleRate, size_t numChannels, size_t blockSize)
{
  sampleRate_ = sampleRate;
  numChannels_ = numChannels;
  blockSize_ = blockSize;

  for (size_t ch = 0; ch < numChannels_; ++ch) {
    Oscillator oscillator;
    oscillator.prepare(sampleRate_, blockSize_);
    oscillators_.push_back(oscillator);

    // CanonicalFilter filter;
    // StateVariableFilter filter;
    AllPassFilter filter;
    filter.prepare(sampleRate_, blockSize_);
    filters_.push_back(filter);
  }
}

void
AudioProcessor::process(uintptr_t channelPointers)
{
  float** channels = reinterpret_cast<float**>(channelPointers);

  if (!isPlaying_) {
    for (size_t ch = 0; ch < numChannels_; ch++) {
      for (size_t i = 0; i < blockSize_; ++i) {
        channels[ch][i] = 0.0f;
      }
    }
    return;
  }

  for (size_t ch = 0; ch < numChannels_; ch++) {
    oscillators_[ch].process(channels[ch]);
    // filters_[ch].applyFilter(channels[ch], cutoffFreq_, q_, filterType_);
    filters_[ch].applyFilter(channels[ch], cutoffFreq_);
  }
}

void
AudioProcessor::setIsPlaying(bool isPlaying)
{
  isPlaying_ = isPlaying;
}

void
AudioProcessor::setFreq(float freq)
{
  freq_ = freq;
}

void
AudioProcessor::setCutoffFreq(float freq)
{
  cutoffFreq_ = freq;
}

void
AudioProcessor::setQ(float q)
{
  q_ = q;
}

void
AudioProcessor::setFilterType(FilterType filterType)
{
  filterType_ = filterType;
}

void
AudioProcessor::resetFilter()
{
  for (size_t ch = 0; ch < numChannels_; ++ch) {
    filters_[ch].reset();
  }
}
