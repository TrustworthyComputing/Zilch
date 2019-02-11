MOV r0 r0 100       ; r0 = 100
MOV r0 r0 0         ; r1 = 0
__loop__
ADD r0 r0 5         ; r0 += 5
ADD r1 r1 1         ; r1 += 1
CMPE r0 r1 7        ; flag = (r1 == 7)
CJMP r0 r0 __end__  ; if (flag) then jump to __end__
JMP r0 r0 __loop__  ; jump to __loop__
__end__
ANSWER r0 r0 r0    ; return r0 // 35
