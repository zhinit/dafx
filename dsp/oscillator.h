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

private:
  float sampleRate_ = 44'100.0f;
  float freq_ = 220.0f;
  float phase_ = 0;
  bool isPlaying_ = false;

  float xhPrev_ = 0.0f;

  CanonicalFilter filter_;
};
