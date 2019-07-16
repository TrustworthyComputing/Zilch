#!/usr/bin/python

import matplotlib.pyplot as plt
from matplotlib import colors
import numpy as np
import sys
import string


benchmark = "specksimon"

mydpi = 300
figname = benchmark+'.png'
# pltsize = (6.2, 1.96) # default (8, 6)
pltsize = (8, 2)
nbits = ["$2^{-60}$", "$2^{-80}$", "$2^{-100}$", "$2^{-120}$"]

data = {
    'specksimon': { 
        'speck32' : [1.543, 2.128, 2.162, 2.207], 'simon32' : [3.655, 4.908, 4.958, 5.002],
        'speck64' : [1.810, 2.692, 2.693, 2.713], 'simon64' : [4.525, 6.284, 6.203, 6.339]
        }
}

# for key, value in data[benchmark].items():
#     for i in range(len(value)):
#         value[i] /= 1000
#     data[benchmark][key] = value


speck32 = data[benchmark]['speck32']
simon32 = data[benchmark]['simon32']
speck64 = data[benchmark]['speck64']
simon64 = data[benchmark]['simon64']

N = len(speck32)
index = np.arange(N)  # the x locations for the groups
# width = 0.42       # the width of the bars
width = 0.225       # the width of the bars

fig, ax = plt.subplots(figsize=pltsize)
ax.margins(0.02, 0.02) 
# rects1 = ax.bar(index, speck32, width, color='xkcd:light pink', hatch='xxx', edgecolor='black', linewidth=1)
# rects2 = ax.bar(index + width, simon32, width, color='xkcd:very light blue', hatch='...', edgecolor='black', linewidth=1)

rects1 = ax.bar(index - width, speck32, width, color='xkcd:light pink', hatch='xxx', edgecolor='black', linewidth=1)
rects3 = ax.bar(index, speck64, width, color='xkcd:very light green', hatch='////', edgecolor='black', linewidth=1)
rects2 = ax.bar(index + width, simon32, width, color='xkcd:very light blue', hatch='...', edgecolor='black', linewidth=1)
rects4 = ax.bar(index + 2*width, simon64, width, color='xkcd:ecru', hatch='---', edgecolor='black', linewidth=1)

ax.set_yscale('log')
ax.set_ylim([0.01, 1000])
ax.set_ylabel("time (sec.)")
ax.set_xlabel("Soundness Error ($2^{-\lambda}$)")
ax.set_xticks(index + width / 2)
ax.set_xticklabels(nbits)
ax.legend((rects1[0], rects3[0], rects2[0], rects4[0]), ("Speck32/64", "Speck64/128", "Simon32/64", "Simon64/128"), fontsize=9, ncol=4, loc='upper left')

# Block size 32 - Key size 64
# Block size 64 - Key size 128

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        if height > 10:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.2f' % (height), ha='center', va='bottom', fontsize=8.5)
        else:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.3f' % (height), ha='center', va='bottom', fontsize=8.5)

autolabel(rects1)
autolabel(rects2)
autolabel(rects3)
autolabel(rects4)

# plt.show()

plt.tight_layout()
plt.savefig("./charts/"+figname,dpi=mydpi, bbox_inches="tight", pad_inches=0.03)
