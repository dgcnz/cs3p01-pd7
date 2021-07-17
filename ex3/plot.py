import matplotlib.pyplot as plt

labels = []
data = []

while True:
	try:
		x = input()
		y = float(input())
	except EOFError:
		break
	else:
		labels.append(x)
		data.append(y)

fig = plt.figure(figsize=(10, 8))
plt.plot(labels, data)
plt.title("Number of threads vs. execution time")
plt.ylabel("Execution time (ms)")
plt.xlabel("Number of threads")
plt.savefig("time_vs_p.png")
plt.close(fig)
