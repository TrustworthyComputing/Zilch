import numpy as np
import matplotlib.pyplot as plt
import sys

benchmark = ''
if len(sys.argv) > 1:
    if sys.argv[1] == 'prover' or sys.argv[1] == 'verifier': # or sys.argv[1] == 'communication':
        benchmark = sys.argv[1]
else:
    benchmark = 'prover'
    # benchmark = 'verifier'
    # benchmark = 'communication'

mydpi = 300

# pltsize = (4, 3)
pltsize = (7, 3)


legend = ['Hyperion', 'Bulletproofs', 'Hyrax' ]

hyperion =  {   'prover' :   [ 1.231022*60 ],
                'verifier' : [ 1.558190 ]
                }

bulletproofs = { 'prover' :  [ None ],
                'verifier' : [ None ]
                }
                
hyrax = {       'prover' :   [ 27.859260 ],
                'verifier' : [ 0.840967 ]
                }


N = len(hyperion[benchmark])
index = np.arange(N)  # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

l1 = ax.plot(index, hyperion[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='s', linewidth=0.5, markersize=6)
l2 = ax.plot(index, bulletproofs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light red', marker='D', linewidth=0.5, markersize=6)
l3 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light blue', marker='>', linewidth=0.5, markersize=6)
# l5 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='o', linewidth=0.5, markersize=6)


ax.set_yscale('log')
if benchmark == 'prover':
    ax.set_ylim([10, 1000])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0]), legend, fontsize=9, ncol=1, loc='upper left')
if benchmark == 'verifier':
    ax.set_ylim([0.1, 10])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0]), legend, fontsize=9, ncol=1, loc='upper left')
# elif benchmark == 'communication':
#     ax.set_ylim([200, 400])
#     ax.set_ylabel('Communication complexity (KB)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=2, loc='upper left')
ax.set_xticks(index)
ax.set_xlabel('Matrix size')
ax.set_xticklabels(['1 Block'])


# plt.show()

plt.tight_layout()
plt.savefig('./charts/comparisons_sha256_'+benchmark+'.png', dpi=mydpi, bbox_inches='tight', pad_inches=0.03)
