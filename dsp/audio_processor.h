#pragma once

#include "./filters.h"
#include "./oscillator.h"
#include <cstdint>

class AudioProcessor
{
public:
  void prepare(float sampleRate, size_t numChannels, size_t blockSize);

  void process(uintptr_t channelPointers);

  void setIsPlaying(bool isPlaying);
  void setFreq(float freq);
  void setCutoffFreq(float freq);
  void setQ(float q);
  void setFilterType(FilterType filterType);
  void resetFilter();

private:
  float sampleRate_ = 44'100.0f;
  size_t blockSize_ = 128;
  size_t numChannels_ = 2;
  float freq_ = 110.0f;
  float phase_ = 0.0f;
  bool isPlaying_ = false;

  std::vector<Oscillator> oscillators_;

  // std::vector<CanonicalFilter> filters_;
  // std::vector<StateVariableFilter> filters_;
  std::vector<AllPassFilter> filters_;
  float cutoffFreq_ = 350.0f;
  float q_ = 0.707f;
  FilterType filterType_;

  StateVariableFilter antiAliasFilter_;

  float polyBlep(float dt, float t);
};
