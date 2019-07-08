#!/usr/bin/python

import matplotlib.pyplot as plt
from matplotlib import colors
import numpy as np
import sys
import string

if len(sys.argv) != 2:
    print "Usage:\t python " + sys.argv[0] + " [nqueens|factorial|fibonacci]"
    sys.exit(1)
if sys.argv[1] == "nqueens" or sys.argv[1] == "factorial" or sys.argv[1] == "fibonacci":
    benchmark = sys.argv[1]
else:
    print "Usage:\t python " + sys.argv[0] + " [nqueens|factorial|fibonacci]"
    sys.exit(1)

mydpi = 300
figname = benchmark+'.png'
pltsize = (6, 1.9) # default (8, 6)
nbits = [4, 8, 16, 24]

data = {
    'nqueens' :     { 'beta16' : [910.39, 2059.405, 4439.46, 12514.24],              'beta24' : [3269.625, 7288.04, 15830.21, 44812.925]         },
    'factorial' :   { 'beta16' : [10.04, 28.86333333, 58.64166667, 150.3016667],   'beta24' : [21.285, 67.97166667, 128.1783333, 327.3716667] },
    'fibonacci' :   { 'beta16' : [10.07, 29.11833333, 51.31166667, 453.8833333],       'beta24' : [26.55333333, 84.7, 134.5683333, 408.7133333]   }
}

for key, value in data[benchmark].items():
    for i in range(len(value)):
        if benchmark == "factorial" or benchmark == "fibonacci":
            value[i] /= 100
        elif benchmark == "nqueens":
            value[i] /= 10000
    data[benchmark][key] = value

beta16 = data[benchmark]['beta16']
beta24 = data[benchmark]['beta24']


N = len(beta16)
index = np.arange(N)  # the x locations for the groups
width = 0.42       # the width of the bars

fig, ax = plt.subplots(figsize=pltsize)
ax.margins(0.04, 0.04) 
rects1 = ax.bar(index, beta16, width, color='xkcd:light pink', hatch='xxx', edgecolor='black', linewidth=1)
rects2 = ax.bar(index + width, beta24, width, color='xkcd:very light blue', hatch='...', edgecolor='black', linewidth=1)

ax.set_yscale('log')
if benchmark == "nqueens":
    ax.set_ylim([0.05, 60])
    ax.set_ylabel("time (sec.) x $10^4$")
elif benchmark == "factorial" or benchmark == "fibonacci":
    ax.set_ylim([0.05, 30])
    ax.set_ylabel("time (sec.) x $10^2$")
ax.set_xlabel("Security Parameter ($\lambda$) bits")
ax.set_xticks(index + width / 2)
ax.set_xticklabels(nbits)
ax.legend((rects1[0], rects2[0]), ("$\\beta$ = 16", "$\\beta$ = 24"), fontsize=8)

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        if height > 10:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.2f' % (height), ha='center', va='bottom', fontsize=8)
        else:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.3f' % (height), ha='center', va='bottom', fontsize=8)

autolabel(rects1)
autolabel(rects2)

# plt.show()

plt.tight_layout()
plt.savefig("./charts/"+figname,dpi=mydpi, bbox_inches="tight", pad_inches=0.03)
