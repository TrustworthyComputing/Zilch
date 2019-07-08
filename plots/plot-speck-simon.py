#!/usr/bin/python

import matplotlib.pyplot as plt
from matplotlib import colors
import numpy as np
import sys
import string


benchmark = "specksimon"

mydpi = 300
figname = benchmark+'.png'
pltsize = (6.2, 1.96) # default (8, 6)
nbits = ["$2^{-60}$", "$2^{-80}$", "$2^{-100}$", "$2^{-120}$"]

data = {
    'specksimon': { 'speck' : [1.543437, 2.128665, 2.162636, 2.207159], 'simon' : [3.655611, 4.908996, 4.958283, 5.002398] }
}

# for key, value in data[benchmark].items():
#     for i in range(len(value)):
#         value[i] /= 1000
#     data[benchmark][key] = value


speck = data[benchmark]['speck']
simon = data[benchmark]['simon']

N = len(speck)
index = np.arange(N)  # the x locations for the groups
width = 0.42       # the width of the bars

fig, ax = plt.subplots(figsize=pltsize)
ax.margins(0.04, 0.04) 
rects1 = ax.bar(index, speck, width, color='xkcd:light pink', hatch='xxx', edgecolor='black', linewidth=1)
rects2 = ax.bar(index + width, simon, width, color='xkcd:very light blue', hatch='...', edgecolor='black', linewidth=1)

ax.set_yscale('log')
ax.set_ylim([0.01, 1000])
ax.set_ylabel("time (sec.)")
ax.set_xlabel("Soundness Error ($2^{-\lambda}$)")
ax.set_xticks(index + width / 2)
ax.set_xticklabels(nbits)
ax.legend((rects1[0], rects2[0]), ("Speck32 Cipher", "Simon32 Cipher"), fontsize=9, ncol=2, loc='upper left')


def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        if height > 10:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.2f' % (height), ha='center', va='bottom', fontsize=8.5)
        else:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.3f' % (height), ha='center', va='bottom', fontsize=8.5)

autolabel(rects1)
autolabel(rects2)

# plt.show()

plt.tight_layout()
plt.savefig("./charts/"+figname,dpi=mydpi, bbox_inches="tight", pad_inches=0.03)
