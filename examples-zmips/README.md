## Some comments about TinyRAM programs

### Knowledge of factorization
Prover claims he/she knows the prime factors of a verifier-chosen number (e.g. 15), without revealing the factors to the verifier.
```
secread $r1, $r1, 1        ; $r1 is filled with a private value from auxiliary tape (e.g. 3)
secread $r2, $r2, 1        ; $r2 is filled with a private value from auxiliary tape (e.g. 5)
move $r11, $r0, 1       ; $r11 = 1
mult $r3, $r1, $r2      ; $r3 = $r1 * $r2
pubread $r5, $r5, 0        ; $r5 is filled with a public value from primary tape (e.g. 15)
beq $r3, $r5, __end__   ; if (r3 == 15) then jump to __end__ (pc = 7)
move $r11, $r0, 0       ; $r11 = 0
__end__
answer $r11, $r11, $r11 ; return $r11 // $return (r1 * $r2 == 15)
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
secread $r0, $r0, 1        ; r0 is filled with a private value from auxiliary tape (e.g. p = 17)
secread $r1, $r1, 1        ; r1 is filled with a private value from auxiliary tape (e.g. q = 11)
sub $r3, $r0, 1         ; p - 1 = 16
sub $r4, $r1, 1         ; q - 1 = 10
mult $r5, $r3, $r4      ; phi(n) = (p - 1) * (q - 1) = 160
secread $r7, $r7, 1        ; r7 is filled with a private value from auxiliary tape (e.g. d = 23)
pubread $r8, $r8, 0        ; public e = 7
mult $r9, $r7, $r8      ; compute d * e
umod $r11, $r9, $r5     ; compute (d * e) mod phi(n)
answer $r11, $r11, $r11 ; answer should be 1
```
