; ./hyperion examples-zmips/knowledge_of_bignum_factorization.asm -t14 -s120 -P./examples-zmips/knowledge_of_bignum_factorization.pubtape -A./examples-zmips/knowledge_of_bignum_factorization.auxtape

; Read the number of blocks from public tape 
READ r1 r1 0                            ; the number of blocks of each number (l1)
MULL r13 r1 2                           ; the number of blocks of the product

; Initialize memory to store blocks of product with zero
MOV r5 r5 0
__init_mult_mem__
    ADD r2 r5 3000                      ; calc index to store it
    STOREW r0 r0 r2                     ; init mem with zero
    ADD r5 r5 1                         ; i++
    CMPE r5 r5 r13                      ; flag = (r5 == 42)
CNJMP r5 r5 __init_mult_mem__           ; if (!flag) then jump to __init_mult_mem__

; Read p and store it to memory (21 16-bit blocks)
MOV r5 r5 0
__read_p__
    READ r0 r0 1                        ; consume next word from auxtape
    ADD r2 r5 1000                      ; calc index to store it
    STOREW r0 r0 r2                     ; store it
    ADD r5 r5 1                         ; i++
    CMPE r5 r5 r1                       ; flag = (r5 == 21)
CNJMP r5 r5 __read_p__                  ; if (!flag) then jump to __read_p__

; Now read q (21 16-bit blocks)
MOV r5 r5 0
__read_q__
    READ r0 r0 1                        ; consume next word from auxtape
    ADD r2 r5 2000                      ; calc index to store it
    STOREW r0 r0 r2                     ; store it
    ADD r5 r5 1                         ; i++
    CMPE r5 r5 r1                       ; flag = (r5 == 21)
CNJMP r5 r5 __read_q__                  ; if (!flag) then jump to __read_q__

; Multiply p and q block per block and check with results from primary tape
MOV r2 r2 0                             ; ii = 0
__for_all_pi__
    SUB r3 r1 1                         ; r3 gets l1 - 1
    SUB r3 r3 r2                        ; i = 21 - 1 - ii

    ; load p16[i] to r6
    ADD r12 r3 1000
    LOADW r6 r6 r12                     ; p16[i]

    MOV r4 r4 0                         ; jj = 0
    __for_all_qi__
        SUB r5 r1 1                     ; r5 gets l1 - 1
        SUB r5 r5 r4                    ; j = 21 - 1 - jj

        ; load q16[j] to r7
        ADD r12 r5 2000
        LOADW r7 r7 r12                 ; q16[j]
        MULL r8 r6 r7                   ; mult = p16[i] * q16[j]
        SHR r7 r8 8                     ; hi = (mult >> BITS_PER_BLOCK)
        AND r8 r8 255                   ; lo = mult & ((1<<BITS_PER_BLOCK) - 1)

        ADD r12 r3 r5                   ; i + j
        ADD r12 r12 3000                ; i + j + 3000 // calc index to store it
        ; num_blocks[i + j] += hi
        LOADW r10 r10 r12               ; load num_blocks[i + j]
        ADD r10 r10 r7                  ; num_blocks[i + j] += hi
        STOREW r10 r10 r12              ; store num_blocks[i + j]
        ; num_blocks[i + j + 1] += lo
        ADD r12 r12 1                   ; i + j + 1 + 3000 // calc next index 
        LOADW r10 r10 r12               ; load num_blocks[i + j + 1]
        ADD r10 r10 r8                  ; num_blocks[i + j + 1] += lo
        STOREW r10 r10 r12              ; store num_blocks[i + j + 1]

        ADD r4 r4 1                     ; jj++ 
        CMPE r4 r4 r1                   ; flag = (jj == 21) 
    CNJMP r4 r4 __for_all_qi__          ; if (!flag) then jump to __for_all_qi__

    ADD r2 r2 1                         ; ii++ 
    CMPE r2 r2 r1                       ; flag = (ii == 21) 
CNJMP r2 r2 __for_all_pi__              ; if (!flag) then jump to __for_all_pi__

SUB r3 r13 1                            ; i = (2 * 21) - 1
__check_overflows__
    ADD r2 r3 3000                      ; calc index to load from
    LOADW r10 r10 r2                    ; load num_blocks[i]
    SUB r3 r3 1                         ; i--
    CMPG r10 r10 255                    ; flag = ( num_blocks[i] > ((1 << BITS_PER_BLOCK)-1) )
    CNJMP r5 r5 __no_overflow__         ; if (!flag) then jump to __no_overflow__
        SHR r8 r10 8                    ; carry = num_blocks[i] >> BITS_PER_BLOCK
        AND r10 r10 255                 ; num_blocks[i] &= ((1 << BITS_PER_BLOCK)-1)
        SUB r2 r2 1                     ; r2-- // r2 = 3000 + i - 1
        LOADW r9 r9 r2                  ; load num_blocks[i - 1]
        ADD r9 r9 r8                    ; num_blocks[i-1] += carry
        STOREW r9 r9 r2                 ; store num_blocks[i + j + 1]
    __no_overflow__
    ; Check equality between computed block and the next from public tape
    READ r4 r4 0                        ; consume next word from primary tape
    CMPE r5 r4 r10                      ; and check if it is equal with the block multiplication
    CNJMP r5 r5 __not_equal__           ; if (r4 != r10) then jump to __not_equal__
    CMPE r3 r3 1                        ; flag = (r3 == 1)
CNJMP r3 r3 __check_overflows__         ; if (!flag) then jump to __check_overflows__

ANSWER r0 r0 1                          ; All sub-results were equal
__not_equal__
ANSWER r0 r0 0                          ; At least one sub-result was not equal
