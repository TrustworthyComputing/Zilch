# IndigoZK [![License MIT][badge-license]](LICENSE)
## An extension of libSTARK's TinyRAM implementation.

### Overview

The libSTARK library implements _scalable_ and _transparent argument of knowledge_ (STARK) systems. These systems can be executed with, or without, _zero knowledge_ (ZK), and may be designed as either _interactive_ or _non-interactive_ protocols. The theoretical constructions which this library implements are described in detail in the [zk-STARK paper](https://eprint.iacr.org/2018/046). Briefly, the main properties of (zk)-STARK systems are:
+ **universality**: the system can be applied to any computation specified by an algebraic intermediate representation (AIR) or by a permuted algebraic intermediate representation (PAIR) as defined in the [zkSTARK paper](https://eprint.iacr.org/2018/046). The former (AIR) is relevant to "memory efficient" computations and the latter (PAIR) for computations that require random access memory (RAM).
+ **transparency**: all messages sent by the verifier, including queries to the proof oracles, are public random coins (i.e., the protocol is "Arthur-Merlin").
+ **scalability**: as an asymptotic function of the number of cycles (T) required by the computation whose integrity is being proved, both of the following conditions hold:
  + **prover scalability**: prover running time scales _quasi-linearly_ in T, i.e., like T poly log T.
  + **verifier scalability**: verifier running time scales _poly-logarithmically_ in T, i.e., like poly log T.
+ **"post-quantum security"**: the cryptographic primitives that underlie the security of the system are either the existence of a family of collision resistant hash functions (for an interactive system) or common access to a random function (the "random oracle" model for a non-interactive setting); in particular, quantum computers are not known to break system security at time of writing.

### Disclaimer

The code is **academic grade**, meant for academic peer review and evaluation. It very likely contains _multiple serious security flaws_, and should not be relied upon for any other purpose.

### Dependencies
#### Hardware acceleration
* sse4 (https://en.wikipedia.org/wiki/SSE4)
  * CLMUL (https://en.wikipedia.org/wiki/CLMUL_instruction_set) for basic arithmetics in characteristic 2 fields.
  * SIMD (https://en.wikipedia.org/wiki/SIMD) instructions for FFT acceleration.
* aes (https://en.wikipedia.org/wiki/AES_instruction_set) to accelerate Merkle tree construction and verification.

#### Compiler
The code was tested with gcc version 7.3.0 (https://gcc.gnu.org), using c++11 standard. But should probably work with most common versions of gcc.
##### Compilation on macOS
In order to compile on macOS please use this thread:
https://github.com/elibensasson/libSTARK/issues/2

#### Libraries (all should be available in standard Linux distribution package managers)
* OpenMP (https://en.wikipedia.org/wiki/OpenMP) for parallelization.
* googletest (https://github.com/google/googletest) for **unit-tests compilation only**

## How to run the code
#### Compilation:
```
git clone https://github.com/elibensasson/libSTARK.git
cd libSTARK
make -j8
```

# STARK for TinyRAM programs
## Arguments format:
```
./stark-tinyram <TinyRAM assembly file path> -t<trace length log_2> [-s<security parameter]> [-p<0,1>] [-T<tapeFile>]
```
## Prefixes
```
TinyRAM assembly file path: Path to the TinyRAM assembly code (required)
-t: trace length log_2 (required)
-s: security parameter (optional)
-P: path to the primary tape file (file path containing public inputs) (optional)
-A: path to the auxiliary tape file (file path containing private inputs) (optional)

The below flags enable verification over the network; if neither is enabled, the execution will be locally. Verifier acts as the server and thus should be executed first.
-r: verifier-address:port-number (optional) (default = 'localhost:1234')
-V: enables execution of the verifier, listening on port-number (optional)
-R: enables execution of the prover, transmitting to verifier-address:port-number (optional)
```
see examples below on how to use the prefixes.

## Labels
In our implementation, both the prefix and the suffix of a the label should be `__`. For instance `__labelName__`.


## Enhanced READ instructions
* `READ ri rj A` : Consume the next word from the [A]-th tape (0 or 1) if it has remaining words and store it in `ri`. A can be either a register or an immediate value. For consuming the next word from the public tape A must be `0`, while for consuming from the private tape `A` must be `1`. In `READ` instruction, register `rj` is not used.

* `RAREAD ri B A` : `RAREAD` stands for `Random Access Read`. `RAREAD` consumes the word in offset `B` from tape `A`. Both `A` and `B` can be either registers or immediate values.


## Macros
We have also added macros which are translated to a bunch of instructions automatically.

1. __@INC register__ Increments `register` by one. (No use of extra registers)
1. __@DEC register__ Decrements `register` by one. (No use of extra registers)
1. __@MIN dest first-register second-register__ Uses `r0` as an intermediate register and puts to `dest` the minimum value of `first-register` and `second-register`.
A simple example is [min_test.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/min_test.asm).
1. __@MAX dest first-register second-register__ Uses `r0` as an intermediate register and puts to `dest` the maximum value of `first-register` and `second-register`.
1. __@SWAP first-register second-register__ Uses `r0` as an intermediate register and swaps values between `first-register` and `second-register`.
1. __@READ_AND_STORE_ARRAY len index tape__ Uses `r0`, `r1` and `r2`.
Reads `len` words from `tape` (0 or 1) and stores them in index `idx`. First word is stored at `idx`, second in `idx+1`, `len`-word is stored at `idx+len-1`. Labels are auto-increment.
For instance, `@READ_AND_STORE_ARRAY 3 1000 0` produces the following code:
```
MOV r1 r1 0
__macro_0__
    READ r0 r0 0
    ADD r2 r1 1000
    STOREW r0 r0 r2
    ADD r1 r1 1
    CMPE r1 r1 3
CNJMP r1 r1 __macro_0__
```

A simple example using `@MIN`, `@INC` and `@DEC` is [min_test.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/min_test.asm).
Another simple example using `@SWAP` is [swap_test.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/swap_test.asm).
An example using `@READ_AND_STORE_ARRAY` is presented in [read_test_with_macros.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/read_test_with_macros.asm).



__Notice:__ Currently, some of the implemented macros use `r0`, `r1` and `r2` registers, thus any value that is stored in those registers before the macros will be lost.
The programmer is responsible of either moving those values to other registers or storing and loading them from the memory.


## Over the Network Verification:
The default behavior (without flags `-r`, `-V`, `-R`) of the `stark-tinyram` executable results in a local execution. 
In order to enable over the network verification first the verifier should be executed (`-V` flag) and then the prover (`-R` flag).
The verifier acts as a server waiting for the prover to connect, executes and prints and returns its decision to the prover.

For instance, a simple read from tapes example over the network:

First run the verifier listening on port `2324`:
```
./stark-tinyram examples-tinyram/read_test.asm -t10 -s120 -P./examples-tinyram/read_test.pubtape -A./examples-tinyram/read_test.auxtape -V -rlocalhost:2324
```
And then the prover to connect to port `2324`:
```
./stark-tinyram examples-tinyram/read_test.asm -t10 -s120 -P./examples-tinyram/read_test.pubtape -A./examples-tinyram/read_test.auxtape -R -rlocalhost:2324
```


### Example (Collatz Conjecture):
```
./stark-tinyram examples-tinyram/collatz.asm -t10 -s120
```
The above execution results in execution of STARK simulation over the collatz program, using at most 1023 (which is 2<sup>10</sup>-1) machine steps, and soundness error at most 2<sup>-120</sup>.


### An example for testing labels and read instruction:
Perform a loop 3 times, each time reading one word from the primary tape and another one from the auxiliary tape.
Primary tape is filled with `1, 2, 3, 4, ...`, while aux tape contains `101, 102, 103, 104, ...`.
In simple terms, the below TinyRAM program adds 1+101+2+102+3+103 and prints it through the `ANSWER` instruction. 
```
__loop__
    READ r0 r0 0        ; consume next word from public tape and store it to r0
    READ r1 r1 1        ; consume next word from auxiliary tape and store it to r1

    ADD r2 r0 r2        ; add them together
    ADD r2 r1 r2        ; r2 = r0 + r1 + r2

    ADD r3 r3  1        ; r3++
    CMPE r3 r3 3        ; flag = (r3 == 3)
CNJMP r0 r0 __loop__    ; if (!flag) then jump to __loop__

ANSWER r0 r0 r2         ; result should be 312
```
In order to execute the above program, simply run `./stark-tinyram examples-tinyram/read_test.asm -t10 -s120 -P./examples-tinyram/read_test.pubtape -A./examples-tinyram/read_test.auxtape`.

We have also implemented the same example using macros in [read_test_with_macros.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/read_test_with_macros.asm).
This specific example is not a great use-case of using macros since it expands to three loops instead of just one, but is implemented for comparison.


### A more interesting example (Knowledge of Factorization):
A more interesting example would be to prove the knowledge of the factors of a number (e.g. 15) without disclosing them to the verifier.
As we already mentioned, all the private inputs (i.e. the inputs that only the prover has knowledge of) are placed in the auxiliary tape.
```
./stark-tinyram examples-tinyram/knowledge_of_factorization.asm -t10 -s120 -P./examples-tinyram/knowledge_of_factorization.pubtape -A./examples-tinyram/knowledge_of_factorization.auxtape
```

#### TinyRAM code for the Knowledge of Factorization example:
```
READ r1 r1 1        ; r1 is filled with a private value from auxiliary tape (e.g. 3)
READ r2 r2 1        ; r2 is filled with a private value from auxiliary tape (e.g. 5)
MOV r11 r0 1        ; r11 = 1
MULL r3 r1 r2       ; r3 = r1 * r2
READ r5 r5 0        ; r5 is filled with a public value from primary tape (e.g. 15)
CMPE r0 r3 r5       ; flag = (r3 == 15)
CJMP r0 r0 __end__  ; if (flag) then jump to __end__ (PC = 7)
MOV r11 r0 0        ; r11 = 0
__end__
ANSWER r0 r0 r11    ; return r11 // return (r1 * r2 == 15)
```

Also in file [knowledge_of_bignum_factorization.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/knowledge_of_bignum_factorization.asm) we have implemented the knowledge of factorization example for big numbers (e.g., 1024 bit arithmetic) based on block multiplication. Private inputs are located [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/knowledge_of_bignum_factorization.auxtape) while public inputs is [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-tinyram/knowledge_of_bignum_factorization.pubtape).
To execute this example simply run `./stark-tinyram examples-tinyram/knowledge_of_bignum_factorization.asm -t14 -s120 -P./examples-tinyram/knowledge_of_bignum_factorization.pubtape -A./examples-tinyram/knowledge_of_bignum_factorization.auxtape`. The numbers in the auxiliary and public tape are the blocks of [RSA-100](https://en.wikipedia.org/wiki/RSA_numbers) number.


### Another interesting example (Knowledge of RSA Private Key):
Prover claims he/she posseses the private RSA key of a verifier-chosen public key without revealing anything about the key to the verifier.
```
./stark-tinyram examples-tinyram/knowledge_of_RSA_private_key.asm -t10 -s120 -P./examples-tinyram/knowledge_of_RSA_private_key.pubtape -A./examples-tinyram/knowledge_of_RSA_private_key.auxtape
```


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

#### TinyRAM code for the knowledge of RSA private key example:
```
READ r0 r0 1        ; r0 is filled with a private value from auxiliary tape (e.g. p = 17)
READ r1 r1 1        ; r1 is filled with a private value from auxiliary tape (e.g. q = 11)
SUB r3 r0 1         ; p - 1 = 16
SUB r4 r1 1         ; q - 1 = 10
MULL r5 r3 r4       ; phi(n) = (p - 1) * (q - 1) = 160
READ r7 r7 1        ; r7 is filled with a private value from auxiliary tape (e.g. d = 23)
READ r8 r8 0        ; public e = 7
MULL r9 r7 r8       ; compute d * e
UMOD r11 r9 r5      ; compute (d * e) mod phi(n)
ANSWER r0 r0 r11    ; Answer should be 1
```

## Execution results
In the simulation the Prover and Verifier interact, the Prover generates a proof and the Verifier verifies it.
During the executions the specifications of generated BAIR and APR, measurements, and Verifier's decision, are printed to the standard output.

## Unit-tests
To use the unit-test the gtest development package should be installed: `apt install libgtest-dev`.

Compilation:
```
make -j8 tests
```
Execution:
```
./bin/algebralib-tests/algebralib_tests
./bin/libstark-tests/libstark-tests
./bin/stark-tinyram-tests/stark-tinyram-tests
```

### Academic literature (partial list, reverse chronological order)

1. Scalable perfect zero knowledge IOP systems [[BCGV](https://link.springer.com/chapter/10.1007%2F978-3-662-49099-0_2), [BCFGRS](https://link.springer.com/chapter/10.1007%2F978-3-319-70503-3_6)].
1. A STARK without ZK [[SCI](https://link.springer.com/chapter/10.1007%2F978-3-319-56617-7_19)].
1. Survey of alternative (non-STARK) proof systems [[WB](https://dl.acm.org/citation.cfm?doid=2641562)].
1. Interactive Oracle Proofs [[BCS](https://link.springer.com/chapter/10.1007%2F978-3-662-53644-5_2), [RRR](https://dl.acm.org/citation.cfm?doid=2897518.2897652)].
1. PCPs with scalable (quasi-linear) proof size [[BS](http://epubs.siam.org/doi/10.1137/050646445), [D](https://dl.acm.org/citation.cfm?doid=1236457.1236459)].
1. ZK-PCPs [[K](https://dl.acm.org/citation.cfm?doid=129712.129782), [KPT](https://dl.acm.org/citation.cfm?doid=258533.258643), [IMS](https://link.springer.com/chapter/10.1007%2F978-3-642-28914-9_9)].
1. Probabilistically checkable proofs (PCPs) [[AS](https://dl.acm.org/citation.cfm?doid=273865.273901), [ALMSS](https://dl.acm.org/citation.cfm?doid=278298.278306)] and PCPs of Proximity (PCPPs) [[BGHSV](http://epubs.siam.org/doi/10.1137/S0097539705446810), [DR](http://epubs.siam.org/doi/10.1137/S0097539705446962)].
1. Scalable (poly-logarithmic) verification of computations [[BFLS](https://dl.acm.org/citation.cfm?doid=103418.103428),
[BGHSV](http://ieeexplore.ieee.org/document/1443079/)].
1. Interactive and ZK proof systems [[BM](https://www.sciencedirect.com/science/article/pii/0022000088900281?via%3Dihub), [GMR](https://dl.acm.org/citation.cfm?doid=22145.22178), [BFL](https://link.springer.com/article/10.1007%2FBF01200056)].

[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
