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
  void prepare(float sampleRate, size_t blockSize);

  void applyFilter(float* channel,
                   float cutoff,
                   float q,
                   FilterType filterType);

  void reset();

private:
  float sampleRate_ = 44100.0f;
  size_t blockSize_ = 128;

  std::vector<float> xhPrev_{ 2, 0.0f };

  void setCoefs(const FilterType filterType,
                const float k,
                const float q,
                float& b0,
                float& b1,
                float& b2,
                float& a1,
                float& a2);
};

class StateVariableFilter
{
public:
  void prepare(float sampleRate, size_t blockSize);

  void applyFilter(float* channel,
                   float cutoff,
                   float q,
                   FilterType filterType);

  void reset();

private:
  float sampleRate_ = 44100.0f;
  size_t blockSize_ = 128;
  float outputLpPrev_ = 0.0f;
  float outputBpPrev_ = 0.0f;
};

class AllPassFilter
{
public:
  void prepare(float sampleRate, size_t blockSize);

  void applyFilter(float* channel, float cutoff);

  void reset();

private:
  float sampleRate_ = 44100.0f;
  size_t blockSize_ = 128;
  std::vector<float> xhPrev_{ 2, 0.0f };
};
