# RSA-100 has 100 decimal digits (330 bits)
# 
# RSA-100 = 37975227936943673922808872755445627854565536638199 * 40094690950920881030683735292761468389214899724061
# RSA-100 = 1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139

BITS_PER_BLOCK = 8

import math

def bytesof(x, bitsnum):
    bits_of_x = int(math.floor(math.log(x, 2)) + 1)
    return int(math.floor(bits_of_x / bitsnum) + 1)
    
def split_to_array_of(num, bitsnum):
    blocks = bytesof(num, bitsnum)
    print(str(num) + " has " + str(blocks) + " " + str(BITS_PER_BLOCK) + "-bit blocks")
    num32 = [0] * blocks
    for i in range(blocks):
        j = blocks-1-i
        num32[j] = (num >> (bitsnum * i)) & ((1<<bitsnum) - 1)
        print("p[" + str(j) + "] = " + str(num32[j]))
    print
    return num32
    
def recreate_num(x_array, bitsnum):
    blocks = len(x_array)
    x = 0
    for i in range(blocks):
        x = (x << bitsnum) + x_array[i]
    return x
        

p = 37975227936943673922808872755445627854565536638199
p_blocks = split_to_array_of(p, BITS_PER_BLOCK)
# print(p_blocks)

q = 40094690950920881030683735292761468389214899724061
q_blocks = split_to_array_of(q, BITS_PER_BLOCK)
# print(q_blocks)

# Multiply blocks
l1 = len(p_blocks)
l2 = len(q_blocks)

x_ver = 0
for ii in range(l1):
    i = l1-1-ii
    for jj in range(l2):
        j = l2-1-jj
        mult = p_blocks[i] * q_blocks[j]
        x_ver = x_ver + (mult << (BITS_PER_BLOCK * (ii + jj)))
        

# Verify
rsa1024 = 1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139
print(x_ver, x_ver == rsa1024)

p_ver = recreate_num(p_blocks, BITS_PER_BLOCK)
print(p_ver, p_ver == p)

q_ver = recreate_num(q_blocks, BITS_PER_BLOCK)
print(q_ver, q_ver == q)