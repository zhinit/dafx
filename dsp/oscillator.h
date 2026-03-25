#pragma once

#include "./filters.h"
#include <cstdint>

class Oscillator
{
public:
  void prepare(float sampleRate);

  void process(uintptr_t leftPtr, uintptr_t rightPtr, size_t blockSize);

  void setIsPlaying(bool isPlaying);
  void setFreq(float freq);
  void setCutoffFreq(float freq);

private:
  float sampleRate_ = 44'100.0f;
  float freq_ = 220.0f;
  float phase_ = 0;
  bool isPlaying_ = false;

  CanonicalFilter filter_;
  float cutoffFreq_ = 350.0f;
};
