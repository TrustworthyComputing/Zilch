# RSA-100 has 100 decimal digits (330 bits)
# 
# RSA-100 = 37975227936943673922808872755445627854565536638199 * 40094690950920881030683735292761468389214899724061
# RSA-100 = 1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139

import math

def bytes16of(num):
    num_bits = int(math.floor(math.log(num, 2)) + 1)
    num_bytes = int(math.floor(num_bits / 16) + 1)
    return num_bytes
    
def split_to_array_of_16(num):
    num_bytes = bytes16of(num)
    print(str(num) + " has " + str(num_bytes) + " 16-bits")
    num32 = [0] * num_bytes
    for i in range(num_bytes):
        j = num_bytes-1-i
        num32[j] = (num >> (16 * i)) & 0xFFFF
        print("p[" + str(j) + "] = " + str(num32[j]))
    print
    return num32
    
def recreate_bignum(num32):
    num_bytes = len(num32)
    num_ver = 0
    for i in range(num_bytes):
        num_ver = (num_ver << 16) + num32[i]
    return num_ver
        

p = 37975227936943673922808872755445627854565536638199
p16 = split_to_array_of_16(p)
# print(p16)

q = 40094690950920881030683735292761468389214899724061
q16 = split_to_array_of_16(q)
# print(q16)

# Multiply blocks
L1 = len(p16)
print(L1, "L1")
L2 = len(q16)

num_ver = 0
for ii in range(L1):
    i = L1-1-ii
    for jj in range(L2):
        j = L2-1-jj
        mult = p16[i] * q16[j]
        print p16[i]
        num_ver = num_ver + (mult << (16 * (ii + jj)))
        

rsa1024 = 1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139
print(num_ver, num_ver == rsa1024)

# Verify
p_ver = recreate_bignum(p16)
print(p_ver, p_ver == p)

q_ver = recreate_bignum(q16)
print(q_ver, q_ver == q)
