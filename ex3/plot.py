import matplotlib.pyplot as plt

data = []

while True:
	try:
		a = float(input())
	except EOFError:
		break
	else:
		data.append(a)

fig = plt.figure(figsize=(10, 8))
plt.plot(data)
plt.savefig("sample.png")
plt.close(fig)
