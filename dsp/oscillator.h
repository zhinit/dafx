#pragma once

#include "./filters.h"

class Oscillator
{
public:
  void prepare(float sampleRate, size_t blockSize);
  void process(float* channel);

  void setFreq(float freq);

private:
  float sampleRate_ = 44'100.0f;
  float blockSize_ = 128;
  float freq_ = 110.0f;
  float phase_ = 0.0f;

  std::vector<float> overSampledBuffer;

  StateVariableFilter antiAliasFilter_;

  float polyBlep(float dt, float t);
};
