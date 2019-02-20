READ r1 r1 1        ; r1 is filled with a private value from auxiliary tape (e.g. 3)
READ r2 r2 1        ; r2 is filled with a private value from auxiliary tape (e.g. 5)
MOV r11 r0 1        ; r11 = 1
MULL r3 r1 r2       ; r3 = r1 * r2
CMPE r0 r3 15       ; flag = (r3 == 15)
CJMP r0 r0 __end__  ; if (flag) then jump to __end__ (PC = 7)
MOV r11 r0 0        ; r11 = 0
__end__
ANSWER r0 r0 r11    ; return r11 // return (r1 * r2 == 15)
