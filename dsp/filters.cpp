#include <cmath>
#include <numbers>

class CanonicalFilter
{
public:
  void prepare(float sampleRate) { sampleRate_ = sampleRate; }

  void applyFilter(float* left, float* right, size_t blockSize, float cutoff)
  {
    float x_h[blockSize];

    float k = std::tan(pi * cutoff / sampleRate_);

    float b_0 = k / (k + 1);
    float b_1 = b_0;
    float a_1 = (k - 1) / (k + 1);

    for (size_t i = 0; i < blockSize; ++i) {
      x_h[i] = left[i] - a_1 * xhPrev_;
      left[i] = b_0 * x_h[i] + b_1 * xhPrev_;
      right[i] = left[i];
      xhPrev_ = x_h[i];
    }
  }

private:
  float pi = std::numbers::pi_v<float>;
  float sampleRate_;
  float xhPrev_;
};
