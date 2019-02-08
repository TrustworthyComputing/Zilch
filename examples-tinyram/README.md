## Some comments about TinyRAM programs

### Knowledge of factorization
Prover claims he/she knows the prime factors of a verifier-chosen number (e.g. 15), without revealing the factors to the verifier.
```
SECREAD r1          ; r1 is filled with a private value from auxiliary tape (e.g. 3)
SECREAD r2          ; r2 is filled with a private value from auxiliary tape (e.g. 5)
MOV r11 r0 1        ; r11 = 1
MULL r3 r1 r2       ; r3 = r1 * r2
CMPE r0 r3 15       ; flag = (r3 == 15)
CJMP r0 r0 7        ; if (flag) then PC = 7
MOV r11 r0 0        ; r11 = 0
ANSWER r0 r0 r11    ; return r11 // return (r1 * r2 == 15)
```

### Knowledge of RSA private key
Prover claims he/she posseses the private RSA key of a verifier-chosen public key without revealing anything about the key to the verifier.

RSA example:
```
p = 17, q = 11
n = p * q = 187
phi(187) = (p-1)*(q-1) = 160
choose random e, 1 < e < 160. ie. e = 7
d * e mod(phi(n)) = 1
d = 23

Public-key:
    n = 187, e = 17
Private-key:
    d = 23, p = 17, q = 11, and thus phi(n) = 160

Proof of correctness:
    d * e mod(phi(n)) = 1
    (23 * 7) mod 160 = 1 ==> 161 mod 160 = 1
```

In TinyRAM:
```
SECREAD r0          ; r0 is filled with a private value from auxiliary tape (e.g. p = 17)
SECREAD r1          ; r1 is filled with a private value from auxiliary tape (e.g. q = 11)
SUB r3 r0 1         ; p - 1 = 16
SUB r4 r1 1         ; q - 1 = 10
MULL r5 r3 r4       ; phi(n) = (p - 1) * (q - 1) = 160
SECREAD r7          ; r7 is filled with a private value from auxiliary tape (e.g. d = 23)
MOV r8 r8 7         ; public e = 7
MULL r9 r7 r8       ; compute d * e
UMOD r11 r9 r5      ; compute (d * e) mod phi(n)
ANSWER r0 r0 r11    ; Answer should be 1
```
