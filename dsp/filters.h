#pragma once

#include <cstddef>
#include <vector>

enum FilterType
{
  LP,
  HP,
  BP,
  BR,
  AP
};

class CanonicalFilter
{
public:
  void prepare(float sampleRate);

  void applyFilter(float* left,
                   float* right,
                   size_t blockSize,
                   float cutoff,
                   float q,
                   FilterType filterType);

  void reset();

private:
  float sampleRate_ = 44100.0f;
  std::vector<float> xhPrev_ = { 0.0f, 0.0f };

  void setCoefs(const FilterType filterType,
                const float k,
                const float q,
                float& b0,
                float& b1,
                float& b2,
                float& a1,
                float& a2);
};
