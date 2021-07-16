import matplotlib.pyplot as plt
import numpy as np

x, y = np.loadtxt('data.csv', delimiter=',', unpack=True)
plt.plot(x, y)

plt.xlabel('Number of processes')
plt.ylabel('Time')
plt.title('Time vs number of processes')
plt.savefig('bucket.png')
