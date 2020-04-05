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

mydpi = 300

pltsize = (4, 3)
# pltsize = (7, 3)


legend = ['Zilch', 'Bulletproofs', 'Hyrax' ]

zilch_regs =  {   'prover' :      [ 2.273, 1.934, 2.5984, 5.2095, 17.4122, 18.6738, 20.1869, 1.4822 * 60, 2.4466 *60],
                'verifier' :    [ 0.1512, 0.1613, 0.2875, 0.5740, 1.1459, 1.3131, 1.5348, 3.0563, 3.5284]
                }

bulletproofs = { 'prover' : [ 0.099388, 0.177597, 0.325088, 0.602870, 0.634588, 1.116599, 1.184699, 2.124449, 2.249473],
                'verifier' : [ 0.024325, 0.036660, 0.059968, 0.088143, 0.099940, 0.153849, 0.169196, 0.270128, 0.299970],
                }
                
hyrax = {       'prover' : [ 0.0637, 0.11044, 0.1402, 0.17622, 0.20312, 0.3328, 0.3916, 0.5011, 0.6227],
                'verifier' : [ 0.0521, 0.0781, 0.0849, 0.0898, 0.0902, 0.1237, 0.1245, 0.1318, 0.1398],
                }


N = len(zilch_regs[benchmark])
index = np.arange(N)  # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

l1 = ax.plot(index, zilch_regs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='s', linewidth=0.5, markersize=6)
l2 = ax.plot(index, bulletproofs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light red', marker='D', linewidth=0.5, markersize=6)
l3 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light blue', marker='>', linewidth=0.5, markersize=6)
# l5 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='o', linewidth=0.5, markersize=6)


ax.set_yscale('log')
if benchmark == 'prover':
    ax.set_ylim([0.01, 10000])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0]), legend, fontsize=9, ncol=1, loc='upper left')
if benchmark == 'verifier':
    ax.set_ylim([0.01, 100])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0]), legend, fontsize=9, ncol=1, loc='upper left')
# elif benchmark == 'communication':
#     ax.set_ylim([200, 400])
#     ax.set_ylabel('Communication complexity (KB)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=2, loc='upper left')
ax.set_xticks(index)
ax.set_xlabel('Matrix size')
ax.set_xticklabels(['4', '5', '6', '7', '8', '9', '10', '11', '12'])


# plt.show()

plt.tight_layout()
plt.savefig('./charts/comparisons_mmult_'+benchmark+'.png', dpi=mydpi, bbox_inches='tight', pad_inches=0.03)
