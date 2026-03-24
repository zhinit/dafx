#include "../oscillator.cpp"
#include <iostream>

int
main()
{
  Oscillator saw;
  float left[8];
  float right[8];

  saw.prepare(4.0f);
  saw.setIsPlaying(true);
  saw.setFreq(1.0f);
  saw.process(
    reinterpret_cast<uintptr_t>(left), reinterpret_cast<uintptr_t>(right), 8);

  for (size_t i = 0; i < 8; ++i)
    std::cout << left[i] << " ";
  std::cout << '\n';
  return 0;
}
