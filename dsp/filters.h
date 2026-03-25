#pragma once

#include <cstddef>

class CanonicalFilter
{
public:
  void prepare(float sampleRate);

  void applyFilter(float* left, float* right, size_t blockSize, float cutoff);

private:
  float sampleRate_ = 44100.0f;
  float xhPrev_ = 0.0f;
};
