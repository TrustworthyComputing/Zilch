__loop__
    READ r0 r0 0        ; consume next word from public tape and store it to r0
    READ r1 r1 1        ; consume next word from auxiliary tape and store it to r1

    ADD r2 r0 r2        ; add them together
    ADD r2 r1 r2        ; r2 = r0 + r1 + r2

    ADD r3 r3  1        ; r3++
    CMPE r3 r3 3        ; flag = (r3 == 3)
CNJMP r0 r0 __loop__    ; if (!flag) then jump to __loop__

ANSWER r0 r0 r2         ; result should be 312
