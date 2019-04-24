# The Hyperion Framework [![License MIT][badge-license]](LICENSE)
## A Framework for Building Zero-Knowledge Proofs for any Application

### Overview
Hyperion is a framework that implements _scalable_ and _transparent_ (it does not need any trusted-party setup phase) _argument_ of _knowledge_ (STARKs).
Hyperion consists of two main components: a front-end and a back-end.
The [front-end](https://github.com/jimouris/zilch-compiler) supports our novel, high-level programming language (called _Zilch_) and our custom compiler for translating the Zilch code to _zMIPS_ assembly instructions; zMIPS is our extension to the MIPS ISA to support programming ZKPs.
The front-end is complemented by our custom _code optimizer_ that performs static analysis, including live-range, dead-code, constant- and copy-propagation analyses, and optimizes the assembly code based on those results.
Our high-level programming language syntax resembles C89 for programming convenience, without the need for pointers or system calls, and incorporates a small number of custom Zilch calls for easy creation of ZKPs for any application.
The back-end translates the zMIPS assembly instructions to arithmetic circuits and generates ZKPs for verifying the evaluation of these circuits.
Our back-end builds upon the ZKP constructions of the [libSTARK](https://eprint.iacr.org/2018/046) library, and extends the limited programming model of libSTARK to our more powerful Zilch abstract machine.
The latter offers support for two read-only input sources (viewed as "tapes"), one for public and one for private inputs, which support _both sequential and random access reads_.
Zilch further supports MACRO instructions and custom labels, which enhances the expressivity of our assembly language.
Finally, Hyperion supports over-the-network ZKPs without trusted third parties.


## Compilation and Dependencies:
```
apt install libjsoncpp-dev
apt install libboost-all-dev
```

```
git clone https://github.com/TrustworthyComputing/Hyperion
cd Hyperion
make -j8
```

## Arguments format:
```
$ ./hyperion --asm <zMIPS assembly file path> [--tsteps <trace length log_2>] [--security <security parameter]> [--pubtape <primaryTapeFile>] [--auxtape <auxTapeFile>] [--verifier | --prover] [--address <address:port_number>]
```
## Prefixes
```
--help      : Display this help message
--examples  : Display some usage examples
--show-asm  : Display zMIPS assembly input
--verbose   : Verbose output, print BAIR, ACSP, APR and FRI specifications

--asm       : Path to the zMIPS assembly code (required)
--tsteps    : trace length log_2 (optional, default = 5)
--security  : security parameter (optional, default = 60)
--pubtape   : path to the primary tape file (optional, default = none)
--auxtape   : path to the auxiliary tape file (optional, default = none)

The flags below enable verification over the network; if neither is enabled, the execution will be locally. Verifier acts as the server and thus should be executed first.
--address  : verifier-address:port-number (optional, default = 'localhost:1234')
--verifier : enables execution of the verifier, listening on port-number (optional, default = false)
--prover   : enables execution of the prover, transmitting to verifier-address:port-number (optional, default = false)
```
see examples below on how to use the prefixes.

## Labels
In our implementation, both the prefix and the suffix of a the label should be `__`. For instance `__labelName__`.


## Enhanced READ instructions
* `read $ri, $rj, A` : Consume the next word from the [A]-th tape (0 or 1) if it has remaining words and store it in `$ri`. A can be either a register or an immediate value. For consuming the next word from the public tape A must be `0`, while for consuming from the private tape `A` must be `1`. In `read` instruction, register `$rj` is not used.

* `seek $ri, B, A` : `seek` is a random access `read` command. `seek` consumes the word in offset `B` from tape `A`. Both `A` and `B` can be either registers or immediate values.


## User-defined Macros
In [macros.json](https://github.com/TrustworthyComputing/Hyperion/blob/master/framework/hyperion/src/macros.json) the user can define her own custom zMIPS macro-instructions.

For instance, we have defined `inc`, `dec` and `min` macro-instructions as shown below :
```
"inc": {
    "reg1": "$x",
    "macro": "add $x, $x, 1"
}
```
This means that `inc` uses one register. A zMIPS program can use the inc instruction as
```
move $r0, $r0, 5
inc $r0
answer $r0, $r0, $r0
```
the answer would be 6.

The `min` macro-instruction uses three registers and also labels:
``` 
"min": {
    "reg1": "$x",
    "reg2": "$y",
    "reg3": "$z",
    "uses_label" : "true",
    "macro" : "blt $y, $z, __min_label__
            move $x, $x, $z
            j __end_min_label__
            __min_label__
            move $x, $x, $y
            __end_min_label__"
    }
``` 
```
move $r0, $r0, 5
move $r1, $r1, 9
min $r2, $r0, $r1
answer $r2, $r2, $r2
```
the answer would be 5.


## Over the Network Verification:
The default behavior (without flags `--address`, `--verifier`, `--prover`) of the `hyperion` executable results in a local execution. 
In order to enable over the network verification first the verifier should be executed (`--verifier` flag) and then the prover (`--prover` flag).
The verifier acts as a server waiting for the prover to connect, executes and prints and returns its decision to the prover.

For instance, a simple read from tapes example over the network:

First run the verifier listening on port `2324`:
```
./hyperion --asm ./examples-zmips/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test.pubtape --auxtape ./examples-zmips/read_test.auxtape --verifier --address localhost:2324
```
And then the prover to connect to port `2324`:
```
./hyperion --asm ./examples-zmips/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test.pubtape --auxtape ./examples-zmips/read_test.auxtape --prover --address localhost:2324
```


### Example (Collatz Conjecture):
```
./hyperion --asm ./examples-zmips/collatz.zmips --tsteps 10 --security 120
```
The above execution results in execution of STARK simulation over the collatz program, using at most 1023 (which is 2<sup>10</sup>-1) machine steps, and soundness error at most 2<sup>-120</sup>.


### An example for testing labels and read instruction:
Perform a loop 3 times, each time reading one word from the primary tape and another one from the auxiliary tape.
Primary tape is filled with `1, 2, 3, 4, ...`, while aux tape contains `101, 102, 103, 104, ...`.
In simple terms, the below TinyRAM program adds 1+101+2+102+3+103 and prints it through the `answer` instruction. 
```
__loop__
    read $r0, $r0, 0        ; consume next word from public tape and store it to r0
    read $r1, $r1, 1        ; consume next word from auxiliary tape and store it to r1

    add $r2, $r0, $r2       ; add them together
    add $r2, $r1, $r2       ; r2 = r0 + r1 + r2

    add $r3, $r3, 1         ; r3++
bne $r3, 3, __loop__    ; if (r3 != 3) then jump to __loop__

answer $r2, $r2, $r2    ; result should be 312
```
In order to execute the above program, simply run `./hyperion --asm ./examples-zmips/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test.pubtape --auxtape ./examples-zmips/read_test.auxtape`.

We have also implemented the same example using macros in [read_test_with_macros.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/read_test_with_macros.asm).
This specific example is not a great use-case of using macros since it expands to three loops instead of just one, but is implemented for comparison.


### A more interesting example (Knowledge of Factorization):
A more interesting example would be to prove the knowledge of the factors of a number (e.g. 15) without disclosing them to the verifier.
As we already mentioned, all the private inputs (i.e. the inputs that only the prover has knowledge of) are placed in the auxiliary tape.
```
./hyperion --asm ./examples-zmips/knowledge_of_factorization.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/knowledge_of_factorization.pubtape --auxtape ./examples-zmips/knowledge_of_factorization.auxtape
```

#### TinyRAM code for the Knowledge of Factorization example:
```
read $r1, $r1, 1        ; $r1 is filled with a private value from auxiliary tape (e.g. 3)
read $r2, $r2, 1        ; $r2 is filled with a private value from auxiliary tape (e.g. 5)
move $r11, $r0, 1       ; $r11 = 1
mult $r3, $r1, $r2      ; $r3 = $r1 * $r2
read $r5, $r5, 0        ; $r5 is filled with a public value from primary tape (e.g. 15)
beq $r3, $r5, __end__   ; if (r3 == 15) then jump to __end__ (pc = 7)
move $r11, $r0, 0       ; $r11 = 0
__end__
answer $r11, $r11, $r11 ; return $r11 // $return (r1 * $r2 == 15)
```

Also in file [knowledge_of_bignum_factorization.asm](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization.asm) we have implemented the knowledge of factorization example for big numbers (e.g., 1024 bit arithmetic) based on block multiplication. Private inputs are located [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization.auxtape) while public inputs is [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization.pubtape).
To execute this example simply run `./hyperion --asm ./examples-zmips/knowledge_of_bignum_factorization.asm -t14 --security 120 --pubtape ./examples-zmips/knowledge_of_bignum_factorization.pubtape --auxtape ./examples-zmips/knowledge_of_bignum_factorization.auxtape`. The numbers in the auxiliary and public tape are the blocks of [RSA-100](https://en.wikipedia.org/wiki/RSA_numbers) number.


### Another interesting example (Knowledge of RSA Private Key):
Prover claims he/she posseses the private RSA key of a verifier-chosen public key without revealing anything about the key to the verifier.
```
./hyperion --asm ./examples-zmips/knowledge_of_RSA_private_key.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/knowledge_of_RSA_private_key.pubtape --auxtape ./examples-zmips/knowledge_of_RSA_private_key.auxtape
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
read $r0, $r0, 1        ; r0 is filled with a private value from auxiliary tape (e.g. p = 17)
read $r1, $r1, 1        ; r1 is filled with a private value from auxiliary tape (e.g. q = 11)
sub $r3, $r0, 1         ; p - 1 = 16
sub $r4, $r1, 1         ; q - 1 = 10
mult $r5, $r3, $r4      ; phi(n) = (p - 1) * (q - 1) = 160
read $r7, $r7, 1        ; r7 is filled with a private value from auxiliary tape (e.g. d = 23)
read $r8, $r8, 0        ; public e = 7
mult $r9, $r7, $r8      ; compute d * e
umod $r11, $r9, $r5     ; compute (d * e) mod phi(n)
answer $r11, $r11, $r11 ; answer should be 1
```

## Execution results
In the simulation the Prover and Verifier interact, the Prover generates a proof and the Verifier verifies it.
During the executions the specifications of generated BAIR and APR, measurements, and Verifier's decision, are printed to the standard output.


### ![alt text][twc-logo] An open-source project by Trustworthy Computing Group


[twc-logo]: ./logos/twc.png

[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
