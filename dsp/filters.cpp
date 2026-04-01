#include "filters.h"
#include "./constants.h"
#include <cmath>

// -------------------------
// Canonical Filter
// --------------------------

void
CanonicalFilter::prepare(float sampleRate, size_t blockSize)
{
  sampleRate_ = sampleRate;
  blockSize_ = blockSize;
}

void
CanonicalFilter::applyFilter(float* channel,
                             float cutoff,
                             float q,
                             FilterType filterType)
{

  float k = std::tan(PI * cutoff / sampleRate_);

  float b0, b1, b2, a1, a2;
  setCoefs(filterType, k, q, b0, b1, b2, a1, a2);

  for (size_t i = 0; i < blockSize_; ++i) {
    // apply filter formula
    float xh = channel[i] - a1 * xhPrev_[0] - a2 * xhPrev_[1];
    channel[i] = b0 * xh + b1 * xhPrev_[0] + b2 * xhPrev_[1];

    // save history
    xhPrev_[1] = xhPrev_[0];
    xhPrev_[0] = xh;
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

// -------------------------
// State Varaible Filter
// --------------------------

void
StateVariableFilter::prepare(float sampleRate, size_t blockSize)
{
  sampleRate_ = sampleRate;
  blockSize_ = blockSize;
}

void
StateVariableFilter::applyFilter(float* channel,
                                 float cutoff,
                                 float q,
                                 FilterType filterType)
{
  float outputHp = 0.0f;
  float outputBp = 0.0f;
  float outputLp = 0.0f;

  q = std::max(0.51f, q);
  float q1 = 1.0f / q;
  float f1 = 2.0f * std::sin(PI * cutoff / sampleRate_);
  f1 = std::max(0.0f, std::min(2 - q1, f1));

  for (size_t i = 0; i < blockSize_; ++i) {
    outputHp = channel[i] - outputLpPrev_ - q1 * outputBpPrev_;
    outputBp = f1 * outputHp + outputBpPrev_;
    outputLp = f1 * outputBp + outputLpPrev_;

    outputBpPrev_ = outputBp;
    outputLpPrev_ = outputLp;

    if (filterType == HP) {
      channel[i] = outputHp;
    } else if (filterType == BP) {
      channel[i] = outputBp;
    } else {
      channel[i] = outputLp;
    }
  }
}

void
StateVariableFilter::reset()
{
  outputBpPrev_ = 0.0f;
  outputLpPrev_ = 0.0f;
}

void
AllPassFilter::prepare(float sampleRate, size_t blockSize)
{
  sampleRate_ = sampleRate;
  blockSize_ = blockSize;
}

void
AllPassFilter::applyFilter(float* channel, float cutoff)
{
  float c = (std::tan(PI * cutoff / sampleRate_) - 1.0f) /
            (std::tan(PI * cutoff / sampleRate_) + 1.0f);

  for (size_t i = 0; i < blockSize_; ++i) {
    float xh = channel[i] - c * xhPrev_;
    channel[i] = c * xh + xhPrev_;
  }
}

void
AllPassFilter::reset()
{
  xhPrev_ = 0.0f;
}
