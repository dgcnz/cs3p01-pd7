import matplotlib.pyplot as plt
import numpy as np
import csv

p = []
total = []
comp = []
comm = []

with open('data.csv') as f:
    rows = list(csv.reader(f, delimiter=','))
    p = [int(row[0]) for row in rows]
    total = [float(row[1]) for row in rows]
    comp = [float(row[2]) for row in rows]
    comm = [float(row[3]) for row in rows]

print(p, total, comp, comm)
plt.plot(p, total, label='Total time')
plt.plot(p, comp, label='Computation time')
plt.plot(p, comm, label='Communication time')

plt.xlabel('Number of processes')
plt.ylabel('Time')
plt.title('Time vs number of processes')
plt.legend()
plt.savefig('bucket.png')
