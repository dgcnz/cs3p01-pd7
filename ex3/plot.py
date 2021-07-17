import matplotlib.pyplot as plt

data = []

while True:
	try:
		y = float(input())
	except EOFError:
		break
	else:
		data.append(y)

labels = [str(2 ** i) for i in range(len(data))]

fig = plt.figure(figsize=(10, 8))
plt.plot(labels, data)
plt.title("Number of threads vs. execution time")
plt.ylabel("Execution time (ms)")
plt.xlabel("Number of threads")
plt.savefig("time_vs_p.png")
plt.close(fig)
