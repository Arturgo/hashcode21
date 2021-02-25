import numpy as np
import matplotlib.pyplot as plt

nbVoitures = int(input())

times = []
for iVoiture in range(nbVoitures):
	time = int(input())
	times.append(time)

times = sorted(times)

fig = plt.figure()
ax = fig.add_subplot(111)

ax.hist(times, 50, color='green', alpha=0.8)
plt.show()
