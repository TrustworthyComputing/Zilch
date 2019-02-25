@READ_AND_STORE_ARRAY 3 1000 0      ; read 3 values from primary tape and store them at 1000, 1001 and 1002
@READ_AND_STORE_ARRAY 3 2000 1      ; read 3 values from auxiliary tape and store them at 2000, 2001 and 2002

MOV r2 r2 0                         ; sum = 0
MOV r3 r3 0                         ; i = 0
__loop__
    ADD r4 r3 1000
    LOADW r0 r0 r4                  ; load value from mem
    ADD r4 r3 2000
    LOADW r1 r1 r4                  ; load value from mem
    
    ADD r2 r0 r2                    ; add them together
    ADD r2 r1 r2                    ; r2 = r0 + r1 + r2
            
    ADD r3 r3  1                    ; i++
    CMPE r3 r3 3                    ; flag = (i == 3)
CNJMP r3 r3 __loop__                ; if (!flag) then jump to __loop__

ANSWER r2 r2 r2                     ; result should be 312
