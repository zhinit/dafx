import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt

sample_rate = 44_100
freq = 440
file_length = 2.0
total_samples = int(sample_rate * file_length)

time = np.arange(total_samples) / sample_rate

# sine wave
# should go oscilate frequency times in 2 pi
sin_samples = np.sin(2 * np.pi * freq * time)
plt.plot(time[:1000], sin_samples[:1000])
plt.show()
sf.write("sine.wav", sin_samples, sample_rate)

# saw
# has slope of -2 to go from 1 to -1 each frequency
phase = (freq * time) % 1.0
saw_samples = 1.0 - 2.0 * phase
plt.plot(time[:1000], saw_samples[:1000])
plt.show()
sf.write("saw.wav", saw_samples, sample_rate)

# triangle
# use abs but
# shift down and multiply by 2
# shift to the left and multiply by 2
tri_samples = 2.0 * np.abs(2.0 * phase - 1.0) - 1.0
plt.plot(time[:1000], tri_samples[:1000])
plt.show()
sf.write("tri.wav", tri_samples, sample_rate)

# square
phase = (freq * time) % 1.0
square_samples = 2 * np.round(phase) - 1.0
plt.plot(time[:1000], square_samples[:1000])
plt.show()
sf.write("square.wav", square_samples, sample_rate)
