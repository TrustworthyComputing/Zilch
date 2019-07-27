#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
import sys
import string

benchmark = 'fibonacci'
mydpi = 300
figname = benchmark+'.png'
pltsize = (6.4, 2.2)

nbits = ['$W = 8$, $\lambda = 60$','$W = 16$, $\lambda = 60$', '$W = 32$, $\lambda = 60$', '$W = 8$, $\lambda = 120$', '$W = 16$, $\lambda = 120$', '$W = 32$, $\lambda = 120$']

data = {
    'fibonacci_s60' : { '8bits' : [0.080329, 0.131446, 0.221845, 0.413722, 0.771309], '16bits' : [0.120351, 0.201161, 0.364322, 0.447337, 0.886423], '32bits' : [0.160044, 0.248045, 0.467424, 0.779899, 1.589399] },
    
    'fibonacci_s120' : { '8bits' : [0.136667, 0.22681, 0.347104, 0.656814, 1.140248], '16bits' : [0.193466, 0.299735, 0.502554, 0.7021, 1.345031], '32bits' : [0.279497, 0.407047, 0.645234, 1.160738, 2.14223] }
}

x_axis_labels = {
# 'fibonacci' : ("Fibonacci(N), Timesteps required(T)", ['N = 4,\nT = 22', 'N = 8,\nT = 46', 'N = 16,\nT = 94', 'N = 32,\nT = 190', 'N = 64,\nT = 382'])
    'fibonacci' : ("Fibonacci(N)", ['N = 4', 'N = 8', 'N = 16', 'N = 32', 'N = 64'])
}

data8b_s60 = data[benchmark+"_s60"]['8bits']
data16b_s60 = data[benchmark+"_s60"]['16bits']
data32b_s60 = data[benchmark+"_s60"]['32bits']

data8b_s120 = data[benchmark+"_s120"]['8bits']
data16b_s120 = data[benchmark+"_s120"]['16bits']
data32b_s120 = data[benchmark+"_s120"]['32bits']


N = len(data8b_s120)
index = np.arange(N)  # the x locations for the groups
width = 0.3       # the width of the bars

fig, ax = plt.subplots(figsize=pltsize)

ax.margins(0.02, 0.02) 

rects1_s120 = ax.bar(index - width, data8b_s120, width, color='xkcd:light pink', hatch='xxx', edgecolor='black', linewidth=1)
rects2_s120 = ax.bar(index, data16b_s120, width, color='xkcd:very light green', hatch='////', edgecolor='black', linewidth=1)
rects3_s120 = ax.bar(index + width, data32b_s120, width, color='xkcd:very light blue', hatch='...', edgecolor='black', linewidth=1)
rects1_s60 = ax.bar(index - width, data8b_s60, width, color='xkcd:light pink', edgecolor='black', linewidth=1)
rects2_s60 = ax.bar(index, data16b_s60, width, color='xkcd:very light green', edgecolor='black', linewidth=1)
rects3_s60 = ax.bar(index + width, data32b_s60, width, color='xkcd:very light blue', edgecolor='black', linewidth=1)

ax.set_yscale('log')
if benchmark == "fibonacci":
    ax.set_ylim([0.1, 10])
    ax.set_ylabel("time (sec.)")
ax.set_xticks(index)
ax.set_xlabel(x_axis_labels[benchmark][0])
ax.set_xticklabels(x_axis_labels[benchmark][1])
ax.legend((rects1_s60[0], rects2_s60[0], rects3_s60[0], rects1_s120[0], rects2_s120[0], rects3_s120[0]), nbits, fontsize=9, ncol=2, loc='upper left')

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        if height > 10:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.1f' % (height), ha='center', va='bottom', fontsize=9)
        else:
            ax.text(rect.get_x() + rect.get_width()/2., 1.1*height, '%2.2f' % (height), ha='center', va='bottom', fontsize=9)

autolabel(rects1_s120)
autolabel(rects2_s120)
autolabel(rects3_s120)

# plt.show()

plt.tight_layout()
plt.savefig("./charts/"+figname,dpi=mydpi, bbox_inches="tight", pad_inches=0.03)
