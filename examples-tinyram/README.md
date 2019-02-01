## Some comments about TinyRAM programs

### Knowledge of factorization
```
SECREAD r1          ; r1 is filled with a private value from auxiliary tape (e.g. 3)
SECREAD r2          ; r2 is filled with a private value from auxiliary tape (e.g. 5)
MOV r11 r0 1        ; r11 = 1
MULL r3 r1 r2       ; r3 = r1 * r2
CMPE r3 r0 15       ; flag = (r3 == 15)
CJMP r0 r0 7        ; if (flag) then PC = 7
MOV r11 r0 0        ; r11 = 0
ANSWER r0 r0 r11    ; return r11 // return (r1 * r2 == 15)
```

### Knowledge of RSA private key
```
p = 61, q = 53
n = p * q = 3233
λ(3233) = lcm(60, 52) = 780
choose random e, 1 < e < 780. ie. e = 17
d * e mod(λ(n)) = 1
d = 413

Public-key:
    n = 3233, e = 17
Private-key:
    n = 3233, d = 413, λ(n) = 780
    d * e mod(λ(n)) = 1
```

ZKP methodology:
```
public  : n = 3233, e = 17
private : d = 413, λ(n) = 780
compute : d * e mod(λ(n)) = 1

7021 mod 780 = 1
```

```
MOV r0 r0 3233      ; n = 3233
MOV r1 r0 17        ; e = 17
MOV r2 r0 413       ; d = 413
MOV r3 r0 780       ; λ(n) = 780
MULL r5 r1 r2       ; r5 = d * e
UMOD r7 r5 r3       ; r7 = d * e mod( λ(n) )
ANSWER r7 r7 r7     ; r7 should be 1
```
