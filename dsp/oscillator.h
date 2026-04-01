#pragma once

#include "./filters.h"

class Oscillator
{
public:
  void prepare(float sampleRate);
  void process(float* channel, size_t blockSize);

  void setFreq(float freq);

private:
  float sampleRate_ = 44'100.0f;
  float freq_ = 110.0f;
  float phase_ = 0.0f;

  StateVariableFilter antiAliasFilter_;

  float polyBlep(float dt, float t);
};
