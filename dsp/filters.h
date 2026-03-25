#pragma once

#include <cstddef>

class CanonicalFilter
{
public:
  void prepare(float sampleRate);

  void applyFilter(float* left, float* right, size_t blockSize, float cutoff);

private:
  float sampleRate_;
  float xhPrev_;
};
