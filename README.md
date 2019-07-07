# The Hyperion Framework [![License MIT][badge-license]](LICENSE) [![Build Status](https://travis-ci.com/TrustworthyComputing/Hyperion.svg?token=gZwZQ8qzAHzETuTPZFp7&branch=master)](https://travis-ci.com/TrustworthyComputing/Hyperion)

## A Framework for Building Zero-Knowledge Proofs for any Application

### Overview
Hyperion is a framework that implements _Scalable_ and _Transparent_ (it does not need any trusted-party setup phase) _ARguments_ of _Knowledge_ (STARKs).
Hyperion consists of two main components: a front-end and a back-end.


The [front-end](https://github.com/jimouris/zilch-compiler) supports our novel, high-level programming language (called __Zilch__) and our custom compiler for translating the Zilch code to __zMIPS__ assembly instructions; zMIPS is our extension to the MIPS ISA to support programming ZKPs.
The front-end is complemented by our custom _code optimizer_ that performs static analysis, including live-range, dead-code, constant- and copy-propagation analyses, and optimizes the assembly code based on those results.
Our high-level programming language syntax resembles C89 for programming convenience, without the need for pointers or system calls, and incorporates a small number of custom Zilch calls for easy creation of ZKPs for any application.


The [back-end](https://github.com/TrustworthyComputing/Hyperion) translates the zMIPS assembly instructions to arithmetic circuits and generates ZKPs for verifying the evaluation of these circuits.
Our back-end builds upon the ZKP constructions of the [libSTARK](https://eprint.iacr.org/2018/046) library, and extends the programming model of libSTARK to our more powerful __zMIPS abstract machine__.
The latter offers support for two read-only input sources (viewed as "tapes"), one for public and one for private inputs, which support _both sequential and random access reads_.
zMIPS further supports MACRO instructions and custom labels, which enhances the expressivity of our assembly language.
Finally, Hyperion supports over-the-network ZKPs without trusted third parties.

_Disclaimer:_ The code is _academic grade_, meant for academic peer review and evaluation. It very likely contains _serious security flaws_.


### Compilation 

##### Dependencies:
* OpenMP (https://en.wikipedia.org/wiki/OpenMP)
* googletest (https://github.com/google/googletest) `apt-get install libgtest-dev`
* libboost (https://www.boost.org/) `apt install libboost-all-dev`
* libjson (https://github.com/open-source-parsers/jsoncpp) `apt install libjsoncpp-dev`

```
git clone https://github.com/TrustworthyComputing/Hyperion
cd Hyperion
make -j8
make hyperion-tests -j8
```

To verify the installation type `./hyperion-tests`.


### Execution:

```
$ ./hyperion --asm <zMIPS assembly file path> [--tsteps <trace length log_2>] [--security <security parameter]> [--pubtape <primaryTapeFile>] [--auxtape <auxTapeFile>] [--verifier | --prover] [--address <address:port_number>]
```

#### Prefixes
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

__Note:__ If the public tape is in the same directory as the zMIPS assembly file and has the same name but has the `.pubtape` extension the flag `--pubtape` may be omitted. The same applies for the private tape with the extension `.auxtape`. For instance the command `./hyperion --asm ./foo.zmips --pubtape foo.pubtape --auxtape foo.auxtape` is the same as `./hyperion --asm ./foo.zmips`, since Hyperion automatically locates the tapes that end with `.pubtape` and `.auxtape` extensions.


## zMIPS ISA
zMIPS is our extension to the MIPS ISA to support programming ZKPs. In [zMIPS.md](https://github.com/TrustworthyComputing/Hyperion/blob/master/zMIPS.md) we elaborate on the zMIPS enhanced instruction set.

### Labels
In Hyperion, both the prefix and the suffix of a the label should be `__`. For instance `__labelName__`.

### zMIPS Input: Enhanced read from tapes instructions
* `pubread $ri` : Consumes the next word from the public tape (if it has remaining words) and stores it in `$ri`.

* `secread $ri` : Consumes the next word from the private tape (if it has remaining words) and stores it in `$ri`.

* `pubseek $ri, $rj` : `seek` is a random access `read` command. `pubseek` consumes the word in offset `$rj` from the public tape. In case the tape does not have at least `$rj` words, this instruction raises an error.

* `secseek $ri, $rj` : `secseek` consumes the word in offset `$rj` from the private tape. In case the tape does not have at least `$rj` words, this instruction raises an error.


### User-defined Macros
In [macros.json](https://github.com/TrustworthyComputing/Hyperion/blob/master/framework/hyperion/src/macros.json) the user can define his/her own custom zMIPS macro-instructions.

For instance, we have defined `inc` and `min` macro-instructions as shown below :
```
"inc": {
    "reg1": "$x",
    "macro": "add $x, $x, 1"
}
```
This means that `inc` uses one register. A zMIPS program can use the inc instruction as
```
move $r0, 5
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

We can also simplify the `answer` instruction using MACROS like:
```
"answer": {
    "reg1": "$x",
    "macro": "answer $x, $x, $x"
}
```

```
move $r0, 5
move $r1, 9
min $r2, $r0, $r1
answer $r2
```
the answer would be 5.


## Over the Network Verification:
The default behavior (without flags `--address`, `--verifier`, `--prover`) of the `hyperion` executable results in a local execution. 
In order to enable over the network verification first the verifier should be executed (`--verifier` flag) and then the prover (`--prover` flag).
The verifier acts as a server waiting for the prover to connect, executes and prints and returns its decision to the prover.

For instance, a simple read from tapes example over the network:

First run the verifier listening on port `2324`:
```
./hyperion --asm ./examples-zmips/read_test/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape --verifier --address localhost:2324
```
And then the prover to connect to port `2324`:
```
./hyperion --asm ./examples-zmips/read_test/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape --prover --address localhost:2324
```


### Example (Collatz Conjecture):
```
./hyperion --asm ./examples-zmips/collatz.zmips --tsteps 10 --security 120
```
The above execution results in execution of STARK simulation over the collatz program, using at most 1023 (which is 2<sup>10</sup>-1) machine steps, and soundness error at most 2<sup>-120</sup>.


### An example for testing labels and read instruction:
Primary tape is filled with `1, 2, 3, 4, ...`, while aux tape contains `101, 102, 103, 104, ...`.
```
pubread $r0             ; consume next word from public tape and store it to r0
print $r0

secread $r1             ; consume next word from auxiliary tape and store it to r1
print $r1

pubseek $r0, 3          ; read the 4th word from the public tape and store it to r0
print $r0

secseek $r1, 3          ; read the 4th word from the auxiliary tape and store it to r1
print $r1

answer $r0
```
In order to execute the above program, simply run `./hyperion --asm ./examples-zmips/read_test/read_test.zmips --tsteps 5 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape`.


### A more interesting example (Knowledge of Factorization):
A more interesting example would be to prove the knowledge of the factors of a number (e.g. 15) without disclosing them to the verifier.
As we already mentioned, all the private inputs (i.e. the inputs that only the prover has knowledge of) are placed in the auxiliary tape.
```
./hyperion --asm ./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.pubtape --auxtape ./examples-zmips/knowledge_of_factorization/knowledge_of_factorization.auxtape
```

#### zMIPS code for the Knowledge of Factorization example:
```
secread $r1             ; $r1 is filled with a private value from auxiliary tape (e.g. 3)
secread $r2             ; $r2 is filled with a private value from auxiliary tape (e.g. 5)
move $r11, 1            ; $r11 = 1
mult $r3, $r1, $r2      ; $r3 = $r1 * $r2
pubread $r5             ; $r5 is filled with a public value from primary tape (e.g. 15)
beq $r3, $r5, __end__   ; if (r3 == 15) then jump to __end__ (pc = 7)
    move $r11, 0        ; $r11 = 0
__end__
answer $r11             ; return $r11 // $return (r1 * $r2 == 15)
```

Also in file [knowledge_of_bignum_factorization.zmips](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.zmips) we have implemented the knowledge of factorization example for big numbers (e.g., 1024 bit arithmetic) based on block multiplication. Private inputs are located [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.auxtape) while public inputs is [here](https://github.com/TrustworthyComputing/IndigoZK/blob/master/examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.pubtape).
To execute this example simply run `./hyperion --asm ./examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.zmips -t14 --security 120 --pubtape ./examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.pubtape --auxtape ./examples-zmips/knowledge_of_bignum_factorization/knowledge_of_bignum_factorization.auxtape`. The numbers in the auxiliary and public tape are the blocks of [RSA-100](https://en.wikipedia.org/wiki/RSA_numbers) number.


### Another interesting example (Knowledge of RSA Private Key):
Prover claims he/she posseses the private RSA key of a verifier-chosen public key without revealing anything about the key to the verifier.
```
./hyperion --asm ./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.pubtape --auxtape ./examples-zmips/knowledge_of_RSA_private_key/knowledge_of_RSA_private_key.auxtape
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

#### zMIPS code for the knowledge of RSA private key example:
```
secread $r0             ; r0 is filled with a private value from auxiliary tape (e.g. p = 17)
secread $r1             ; r1 is filled with a private value from auxiliary tape (e.g. q = 11)
sub $r3, $r0, 1         ; p - 1 = 16
sub $r4, $r1, 1         ; q - 1 = 10
mult $r5, $r3, $r4      ; phi(n) = (p - 1) * (q - 1) = 160
secread $r7             ; r7 is filled with a private value from auxiliary tape (e.g. d = 23)
pubread $r8             ; public e = 7
mult $r9, $r7, $r8      ; compute d * e
mod $r11, $r9, $r5      ; compute (d * e) mod phi(n)
answer $r11             ; answer should be 1
```

### Execution results
In the simulation the Prover and Verifier interact, the Prover generates a proof and the Verifier verifies it.
During the executions the specifications of generated BAIR and APR, measurements, and Verifier's decision, are printed to the standard output.



### ![alt text][twc-logo] An open-source project by Trustworthy Computing Group


[twc-logo]: ./logos/twc.png

[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
