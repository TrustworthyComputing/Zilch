import numpy as np
import matplotlib.pyplot as plt
import sys

mydpi = 500

# pltsize = (4, 3)
pltsize = (6.4, 2.2)

factorial = [ 9.00, 10.17, 15.18, 14.33, 15.08, 53.50, 50.23, 50.38, 55.42, 52.31, 57.67, 118.33, 110.00, 109.85, 113.08, 112.31 ]

N = len(factorial)
index = np.arange(N) # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

l1 = ax.plot(index, factorial, linestyle='solid', color='black', markerfacecolor='xkcd:sky blue', marker='D', linewidth=0.5, markersize=6)


ax.set_yscale('log')
ax.set_ylim([1, 1000])
ax.set_ylabel('Ratio of Zilch\nover Java 11')


ax.set_xticks(index)
ax.set_xlabel('Factorial')
ax.set_xticklabels(['$1$', '$2$', '$3$', '$4$', '$5$', '$6$', '$7$', '$8$', '$9$', '$10$', '$11$', '$12$', '$13$', '$14$', '$15$', '$16$'])

# plt.show()

plt.tight_layout()
plt.savefig('./charts/baseline_factorial.png', dpi=mydpi, bbox_inches='tight', pad_inches=0.03)