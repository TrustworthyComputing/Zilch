; ./stark-tinyram examples-tinyram/knowledge_of_1024bit_factorization.asm -t13 -s120 -P./examples-tinyram/knowledge_of_1024bit_factorization.pubtape -A./examples-tinyram/knowledge_of_1024bit_factorization.auxtape

; Read p and store it to memory (21 16-bit blocks)
MOV r1 r1 0
__read_p__
READ r0 r0 1                ; consume next word from auxtape
ADD r2 r1 1000              ; calc index to store it
STOREW r0 r0 r2             ; store it
ADD r1 r1 1                 ; i++
CMPE r0 r1 21               ; flag = (r1 == 21)
CJMP r0 r0 __read_p_end__   ; if (flag) then jump to __innerr_for_end__
JMP r0 r0 __read_p__
__read_p_end__

; Now read q (21 16-bit blocks)
MOV r1 r1 0
__read_q__
READ r0 r0 1                ; consume next word from auxtape
ADD r2 r1 2000              ; calc index to store it
STOREW r0 r0 r2             ; store it
ADD r1 r1 1                 ; i++
CMPE r0 r1 21               ; flag = (r1 == 21)
CJMP r0 r0 __read_q_end__   ; if (flag) then jump to __innerr_for_end__
JMP r0 r0 __read_q__
__read_q_end__

; Multiply p and q block per block and check with results from primary tape
MOV r2 r2 0                 ; ii = 0
__for_all_pi__
MOV r3 r3 20
SUB r3 r3 r2                ; i = 21 - 1 - ii

; load p16[i] to r6
ADD r12 r3 1000
LOADW r6 r6 r12             ; p16[i]

MOV r4 r4 0                 ; jj = 0
__for_all_qi__
MOV r5 r5 20
SUB r5 r5 r4                ; j = 21 - 1 - jj

; load q16[j] to r7
ADD r12 r5 2000
LOADW r7 r7 r12             ; q16[j]
MULL r8 r6 r7               ; mult = p16[i] * q16[j]

READ r9 r9 0                ; consume next word from primary tape
CMPE r0 r8 r9               ; and check if it is equal with the block multiplication
CJMP r0 r0 __are_equal__    ; if (r8 == r9) then jump to __are_equal__
JMP r0 r0 __not_equal__     ; else jump to end and return 0
__are_equal__

ADD r4 r4 1                 ; jj++ 
CMPE r0 r4 21               ; flag = (jj == 21) 
CJMP r0 r0 __for_all_qi_end__   ; if (flag) then jump to __for_all_qi_end__
JMP r0 r0 __for_all_qi__        ; else jump to the start 
__for_all_qi_end__

ADD r2 r2 1                 ; ii++ 
CMPE r0 r2 21               ; flag = (ii == 21) 
CJMP r0 r0 __for_all_pi_end__   ; if (flag) then jump to __for_all_pi_end__

JMP r0 r0 __for_all_pi__    ; else jump to the start
__for_all_pi_end__

ANSWER r0 r0 1 ; All sub-results were equal
__not_equal__
ANSWER r0 r0 0 ; At least one sub-result was not equal
