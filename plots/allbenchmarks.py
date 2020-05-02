import numpy as np
import matplotlib.pyplot as plt
import sys

benchmark = ''
if len(sys.argv) > 1:
    if sys.argv[1] == 'prover' or sys.argv[1] == 'verifier' or sys.argv[1] == 'communication':
        benchmark = sys.argv[1]
else:
    benchmark = 'prover'
    # benchmark = 'verifier'
    # benchmark = 'communication'

mydpi = 300

pltsize = (4, 3)
# pltsize = (6.4, 2.2)


legend = ['Factorial', 'Fibonacci', 'Collatz', 'PIR', 'Speck32', 'Simon32', 'Matrix Mult.']


factorial = {   'prover' : [ 0.093065, 0.119385, 0.178415, 0.260860, 0.478281, 0.903474, 2.161618, 4.565379, 9.022720 ],
                'verifier' : [ 0.030576, 0.029649, 0.028560, 0.028785, 0.029414, 0.033472, 0.029839, 0.029726, 0.030151 ],
                'communication' : [ 212.062500, 215.125000, 222.109375, 226.484375, 236.593750, 241.281250, 249.359375, 262.203125, 266.390625 ] } # KBytes,

fibonacci = {   'prover' : [ 0.073386, 0.091637, 0.145771, 0.247560, 0.468014, 0.888938, 1.975108, 5.247415, 11.333514 ],
                'verifier' : [ 0.021240, 0.021883, 0.022224, 0.022313, 0.022263, 0.022756, 0.023259, 0.023393, 0.023205 ],
                'communication' : [ 209.625000, 207.921875, 221.031250, 220.421875, 231.984375, 233.218750, 248.312500, 247.390625, 261.656250 ] }

#                          [ 2, 4, 9, 115, 55, 3133 ]
collatz = { 'prover' : [ 0.067946, 0.143712, 0.170728, 0.282284, 0.470858, 0.904538, 2.132010, 5.237204, 11.452509 ],
            'verifier' : [ 0.021580, 0.021345, 0.022497, 0.023755, 0.023232, 0.023259, 0.023286, 0.023637, 0.023829 ],
            'communication' : [ 207.921875, 209.625000, 221.031250, 220.421875, 231.984375, 233.218750, 248.312500, 247.390625, 261.656250 ] }

speck = {   'prover' : [ None, None, None, None, None, 1.037857, 3.655111, 12.106470, 27.546865 ],
            'verifier' : [ None, None, None, None, None, 0.027119, 0.048041, 0.077191, 0.086886 ],
            'communication' : [ None, None, None, None, None, 235.484375, 251.851300, 259.203125, 273.468750 ] }

simon = {   'prover' : [ None, None, None, None, None, None, 3.329412, 14.039877, 29.818561 ],
            'verifier' : [ None, None, None, None, None, None, 0.038435, 0.068230, 0.079094 ],
            'communication' : [ None, None, None, None, None, None, 252.890625, 261.812500, 265.156250 ] }

mmult = {   'prover' : [ 0.078125, 0.170655, 0.285565, 0.604151, 2.144369, 4.373802, 9.979514, 20.415620, 41.748632 ],
            'verifier' : [ 0.028389, 0.039623, 0.037785, 0.053820, 0.069142, 0.068868, 0.085635, 0.116191, 0.125473 ],
            'communication' : [ 262.062500, 268.296875, 281.406250, 281.671875, 304.968750, 303.734375, 319.140625, 320.062500, 333.406250 ] }

pir = {   'prover' : [ 0.064583, 0.091351, 0.156915, 0.247922, 0.466817, 0.900133, 2.053873, 5.408473, 11.541519 ],
            'verifier' : [ 0.021196, 0.021694, 0.022259, 0.021678, 0.023074, 0.022746, 0.022842, 0.022164, 0.023204 ],
            'communication' : [ 208.796875, 210.500000, 221.906250, 221.296875, 234.093750, 232.859375, 248.265625, 249.187500, 262.531250 ] }

for i in range(len(factorial['communication'])):
    if factorial['communication'][i] is not None:
        factorial['communication'][i] /= 100
    if fibonacci['communication'][i] is not None:
        fibonacci['communication'][i] /= 100
    if collatz['communication'][i] is not None:
        collatz['communication'][i] /= 100
    if speck['communication'][i] is not None:
        speck['communication'][i] /= 100
    if simon['communication'][i] is not None:
        simon['communication'][i] /= 100
    if mmult['communication'][i] is not None:
        mmult['communication'][i] /= 100
    if pir['communication'][i] is not None:
        pir['communication'][i] /= 100

N = len(factorial[benchmark])
index = np.arange(N)  # the x locations for the groups

fig, ax = plt.subplots(figsize=pltsize)

l7 = ax.plot(index, mmult[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:very light blue', marker='>', linewidth=0.5, markersize=6)
l6 = ax.plot(index, simon[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:green', marker='<', linewidth=0.5, markersize=6)
l5 = ax.plot(index, speck[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:sky blue', marker='D', linewidth=0.5, markersize=6)
l4 = ax.plot(index, pir[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light green', marker='s', linewidth=0.5, markersize=6)
l3 = ax.plot(index, collatz[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:light orange', marker='d', linewidth=0.5, markersize=6)
l2 = ax.plot(index, fibonacci[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:butter yellow', marker='^', linewidth=0.5, markersize=6)
l1 = ax.plot(index, factorial[benchmark], linestyle='solid', color='black', markerfacecolor='xkcd:salmon', marker='o', linewidth=0.5, markersize=6)


if benchmark == 'prover':
    ax.set_yscale('log')
    ax.set_ylim([0.05, 100])
    ax.set_ylabel('time (sec.)')
    ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=1, loc='upper left')
if benchmark == 'verifier':
    ax.set_yscale('log')
    ax.set_ylim([0.01, 0.2])
    ax.set_ylabel('time (sec.)')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=2, loc='upper left')
elif benchmark == 'communication':
    ax.set_yticks(np.arange(2, 4, 0.4))
    ax.set_ylabel('size (KB) x $10^2$')
    # ax.legend((l1[0], l2[0], l3[0], l4[0], l5[0], l6[0], l7[0]), legend, fontsize=9, ncol=2, loc='upper left')
ax.set_xticks(index)
ax.set_xlabel('Number of instructions')
ax.set_xticklabels(['$2^4$', '$2^5$', '$2^6$', '$2^7$', '$2^8$', '$2^9$', '$2^{10}$', '$2^{11}$', '$2^{12}$'])


# plt.show()

plt.tight_layout()
plt.savefig('./charts/allbenchmarks_'+benchmark+'.png', dpi=mydpi, bbox_inches='tight', pad_inches=0.03)