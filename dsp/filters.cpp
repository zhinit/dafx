#include "filters.h"

#include "./constants.h"
#include <cmath>

void
CanonicalFilter::prepare(float sampleRate)
{
  sampleRate_ = sampleRate;
}

void
CanonicalFilter::applyFilter(float* left,
                             float* right,
                             size_t blockSize,
                             float cutoff,
                             float q,
                             FilterType filterType)
{
  std::vector<float> xh(blockSize, 0.0f);

  float k = std::tan(PI * cutoff / sampleRate_);

  float b0, b1, b2, a1, a2;
  setCoefs(filterType, k, q, b0, b1, b2, a1, a2);

  for (size_t i = 0; i < blockSize; ++i) {
    // apply filter formula
    xh[i] = left[i] - a1 * xhPrev_[0] - a2 * xhPrev_[1];
    left[i] = b0 * xh[i] + b1 * xhPrev_[0] + b2 * xhPrev_[1];

    // for now lets just assume mono CHANGE LATER
    right[i] = left[i];

    // save history
    xhPrev_[1] = xhPrev_[0];
    xhPrev_[0] = xh[i];
  }
}

void
CanonicalFilter::reset()
{
  xhPrev_[0] = 0.0f;
  xhPrev_[1] = 0.0f;
}

void
CanonicalFilter::setCoefs(const FilterType filterType,
                          const float k,
                          const float q,
                          float& b0,
                          float& b1,
                          float& b2,
                          float& a1,
                          float& a2)
{
  float denom = k * k * q + k + q;
  a1 = (2.0f * q * (k * k - 1.0f)) / denom;
  a2 = (k * k * q - k + q) / denom;
  switch (filterType) {
    case LP:
      b0 = (k * k * q) / denom;
      b1 = 2.0f * b0;
      b2 = b0;
      break;
    case HP:
      b0 = q / denom;
      b1 = -2.0f * b0;
      b2 = b0;
      break;
    case BP:
      b0 = k / denom;
      b1 = 0.0f;
      b2 = -b0;
      break;
    case BR:
      b0 = (q * (1.0f + k * k)) / denom;
      b1 = (2.0f * q * (k * k - 1.0f)) / denom;
      b2 = b0;
      break;
    case AP:
      b0 = (k * k * q - k + q) / denom;
      b1 = (2.0f * q * (k * k - 1.0f)) / denom;
      b2 = 1.0f;
      break;
  }
}
