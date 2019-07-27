import numpy as np
import matplotlib.pyplot as plt

pltsize = (6.4, 2.2)
mydpi = 300


legend = ['Prover 16-bits', 'Verifier 16-bits', 'Prover 32-bits', 'Verifier 32-bits']


prover16bits = [ 0.040579, 0.071375, 0.099710, 0.223937, 0.227975, 0.949586, 2.506098, 5.320718, 11.200841 ]
prover32bits = [ 0.069892, 0.132708, 0.130252, 0.372982, 0.371100, 1.978278, 4.159766, 8.671282, 19.416889 ]
verifier16bits = [ 0.017787, 0.018508, 0.018223, 0.018918, 0.019446, 0.019730, 0.019128, 0.019950, 0.020411 ]
verifier32bits = [ 0.025006, 0.024876, 0.024652, 0.025637, 0.025933, 0.027333, 0.026378, 0.027331, 0.026701]

N = len(prover16bits)
index = np.arange(N)  # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

# red dashes, blue squares and green triangles
l1 = ax.plot(index, prover16bits, linestyle='solid', color='black', markerfacecolor='xkcd:light orange', marker='o', linewidth=0.5, markersize=7, label='line 1')
l2 = ax.plot(index, verifier16bits, linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='^', linewidth=0.5, markersize=7, label='line 2')
l3 = ax.plot(index, prover32bits, linestyle='solid', color='black', markerfacecolor='xkcd:light blue', marker='s', linewidth=0.5, markersize=7, label='line 3')
l4 = ax.plot(index, verifier32bits, linestyle='solid', color='black', markerfacecolor='xkcd:light yellow', marker='D', linewidth=0.5, markersize=7, label='line 3')

ax.set_yscale('log')
ax.set_ylim([0.01, 100])
ax.set_ylabel("time (sec.)")
ax.set_xticks(index)
ax.set_xlabel('Number of instructions')
ax.set_xticklabels(['16', '32', '64', '128', '256', '512', '1024', '2048', '4096'])
ax.legend((l1[0], l2[0], l3[0], l4[0]), legend, fontsize=9, ncol=2, loc='upper left')


# plt.show()

plt.tight_layout()
plt.savefig("./charts/instructions.png", dpi=mydpi, bbox_inches="tight", pad_inches=0.03)
