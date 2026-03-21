import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt

f_s = 44_100
freq = 440
file_length = 2.0
total_samples = int(f_s * file_length)
time = np.arange(total_samples) / f_s

# make a saw as input signal x[n]
phase = (freq * time) % 1.0
x = 1.0 - 2.0 * phase
plt.plot(time[:1000], x[:1000])
plt.show()

# filter the saw
# second order low pass
f_c = 1_000
Q = 1 / (2 ** (0.5))
K = np.tan(np.pi * f_c / f_s)

denom = K**2 * Q + K + Q

b_0 = K**2 * Q / denom
b_1 = 2 * b_0
b_2 = b_0

a_1 = 2 * Q * (K**2 - 1) / denom
a_2 = (K**2 * Q - K + Q) / denom

x_h = np.zeros(total_samples)
y = np.zeros(total_samples)
for n in range(len(x)):
    x_h[n] = x[n] - a_1 * x_h[n - 1] - a_2 * x_h[n - 2]
    y[n] = b_0 * x_h[n] + b_1 * x_h[n - 1] + b_2 * x_h[n - 2]
plt.plot(time[:1000], y[:1000])
plt.show()

sf.write("output_sounds/saw.wav", x, f_s)
sf.write("output_sounds/lp-saw.wav", y, f_s)
