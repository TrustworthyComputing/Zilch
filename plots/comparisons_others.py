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


# legend = ['Zilch-1-reg' ,'Zilch-Regs', 'Zilch-Tape', 'Zilch-Tape-NoSeek', 'Bulletproofs', 'Hyrax' ]
legend = ['Zilch-Regs', 'Zilch-Tape', 'Bulletproofs', 'Hyrax' ]

# zilch_1reg = {   'prover' :  [ 0.452401, 2.033594, 2.359311, 8.246474, 5.814676, 13.701671, 16.125765, 39.336746, 53.658344, 4.750684*60, 4.959441*60, 5.177292*60, 5.680984*60 ],
#                 'verifier' :    [ 0.080788, 0.137124, 0.242234, 0.466610, 0.494386, 0.875259, 0.975965, 1.682557, 1.791617, 3.239871, 3.380108, 3.542254, 3.714149 ],
#                 }

zilch_regs =  {   'prover' :      [ 2.273, 1.934, 2.5984, 5.2095, 17.4122, 18.6738, 20.1869, 1.4822 * 60, 2.4466 *60, 10.1226*60, 12.4469*60, 13.1648*60, 50.7941*60 ],
                'verifier' :    [ 0.1512, 0.1613, 0.2875, 0.5740, 1.1459, 1.3131, 1.5348, 3.0563, 3.5284, 7.0708, 7.9601, 9.0186, 18.1823 ],
                # 'communication' : [ 304.968, 304.968, 305.4843, 359.4375, 389.1406, 424.5781, 459.1406, 535.5312, 584.9687, 658.1250, 716.7500, 775.3750, 875.2343 ] # KBytes,
                }

zilch_tape ={ 'prover' :     [ 1.215575, 1.942465, 4.219186, 6.624356, 8.730812, 13.346925, 15.114755, None, None, None, None, None, None ],
                'verifier' :    [ 0.055115, 0.046980, 0.062355, 0.075279, 0.086876, 0.066085, 0.072600, None, None, None, None, None, None ],
                }

# zilch_tape_noseek = { 'prover' : [ 1.057642, 1.748901, 2.317338, 4.066957, 3.153779, 6.069818, 6.719411, 13.786633, 13.965558, None, None, None, None ],
#                         'verifier' :[ 0.040105, 0.039939, 0.056032, 0.057815, 0.049102, 0.043351, 0.056936, 0.057217, 0.062261, None, None, None, None ],
#                 }
# 
# zilch_tape_indexes = { 'prover' : [ None, None, None, None, None, None, None, None, None, None, None, None, None ],
#                         'verifier' :[ None, None, None, None, None, None, None, None, None, None, None, None, None ],
#                 }
                
bulletproofs = { 'prover' : [ 0.099388, 0.177597, 0.325088, 0.602870, 0.634588, 1.116599, 1.184699, 2.124449, 2.249473, 4.026145, 4.110821, 4.287803, 4.389603 ],
                'verifier' : [ 0.024325, 0.036660, 0.059968, 0.088143, 0.099940, 0.153849, 0.169196, 0.270128, 0.299970, 0.431761, 0.463101, 0.549591, 0.67921 ],
                }
                
hyrax = {       'prover' : [ 0.0637, 0.11044, 0.1402, 0.17622, 0.20312, 0.3328, 0.3916, 0.5011, 0.6227, 0.8312, 0.9365, 1.0868, 1.2370 ],
                'verifier' : [ 0.0521, 0.0781, 0.0849, 0.0898, 0.0902, 0.1237, 0.1245, 0.1318, 0.1398, 0.1493, 0.1523, 0.1537, 0.1560 ],
                }


N = len(zilch_regs[benchmark])
index = np.arange(N)  # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

# l1 = ax.plot(index, zilch_1reg[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:yellow', marker='<', linewidth=0.5, markersize=6)
# l2 = ax.plot(index, zilch_regs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light blue', marker='s', linewidth=0.5, markersize=6)
# l3 = ax.plot(index, zilch_tape[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light red', marker='D', linewidth=0.5, markersize=6)
# l4 = ax.plot(index, zilch_tape_noseek[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light orange', marker='>', linewidth=0.5, markersize=6)
# l5 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='o', linewidth=0.5, markersize=6)
l1 = ax.plot(index, zilch_regs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:yellow', marker='<', linewidth=0.5, markersize=6)
l2 = ax.plot(index, zilch_tape[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light blue', marker='s', linewidth=0.5, markersize=6)
l3 = ax.plot(index, bulletproofs[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light red', marker='D', linewidth=0.5, markersize=6)
l4 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light orange', marker='>', linewidth=0.5, markersize=6)
# l5 = ax.plot(index, hyrax[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='o', linewidth=0.5, markersize=6)


ax.set_yscale('log')
if benchmark == 'prover':
    ax.set_ylim([0.01, 10000])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0], l4[0]), legend, fontsize=9, ncol=1, loc='upper left')
if benchmark == 'verifier':
    ax.set_ylim([0.01, 100])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0]), legend, fontsize=9, ncol=1, loc='upper left')
    ax.legend((l1[0], l2[0], l3[0], l4[0]), legend, fontsize=9, ncol=1, loc='upper left')
# elif benchmark == 'communication':
#     ax.set_ylim([200, 400])
#     ax.set_ylabel('Communication complexity (KB)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=2, loc='upper left')
ax.set_xticks(index)
ax.set_xlabel('Matrix size')
ax.set_xticklabels(['4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16'])


# plt.show()

plt.tight_layout()
plt.savefig('./charts/comparisons_all'+benchmark+'.png', dpi=mydpi, bbox_inches='tight', pad_inches=0.03)
