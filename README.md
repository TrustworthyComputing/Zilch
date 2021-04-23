<h1 align="center">The Zilch Framework <a href="https://github.com/TrustworthyComputing/Zilch/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg"></a> </h1>
<h3 align="center">A Framework for Deploying Transparent Zero Knowledge Proofs</h3>


### Overview
Zilch is a framework that implements _Scalable_ and _Transparent_ (no need for trusted setup) _ARguments_ of _Knowledge_ (STARKs).
Zilch consists of two main components: a front-end and a back-end.


The [front-end](https://github.com/TrustworthyComputing/ZeroJava-compiler) consists of the ZeroJava programming language, a subset of Java designed for zero-knowledge arguments, and a compiler for translating the ZeroJava code to zMIPS assembly instructions; zMIPS is our extension to the MIPS ISA to support programming ZKPs.


The [back-end](https://github.com/TrustworthyComputing/Zilch) translates the zMIPS assembly instructions to arithmetic circuits and generates ZKPs for verifying the evaluation of these circuits.
The back-end builds upon the ZKP constructions of the [zkSTARK](https://eprint.iacr.org/2018/046) library, and extends the programming model of libSTARK to the zMIPS abstract machine.

__Disclaimer:__ The code is _academic grade_, meant for academic peer review and evaluation.


### Cite this work
If you find our work useful, please cite our publication ([IEEE Xplore](https://ieeexplore.ieee.org/document/9410618), [Cryptology ePrint Archive](https://eprint.iacr.org/2020/1155.pdf)):
```
D. Mouris and N. G. Tsoutsos, "Zilch: A Framework for Deploying Transparent Zero-Knowledge Proofs,"
in IEEE Transactions on Information Forensics and Security (TIFS), 2021, DOI: 10.1109/TIFS.2021.3074869
```

### Compilation

#### Zilch Dependencies:
* OpenMP (https://en.wikipedia.org/wiki/OpenMP)
* libcrypto (https://wiki.openssl.org/index.php/Main_Page) `apt install libssl-dev`
* libboost (https://www.boost.org/) `apt install libboost-all-dev`
* libjson (https://github.com/open-source-parsers/jsoncpp) `apt install libjsoncpp-dev`
* googletest (https://github.com/google/googletest) `apt-get install libgtest-dev`

For the ZeroJava compiler dependencies refer to the [ZeroJava repository]().

#### Build Zilch
```
$ git clone --recursive https://github.com/TrustworthyComputing/Zilch
$ cd Zilch
$ make -j8
$ make zilch-tests -j8
```

To verify the installation type `./zilch-tests`.

#### Build the ZeroJava Compiler

```
$ cd ZeroJava-compiler
$ mvn initialize package
```

### Execution

```
$ ./zilch --asm <zMIPS assembly file path> [--tsteps <trace length log_2>] [--security <security parameter]> [--pubtape <primaryTapeFile>] [--auxtape <auxTapeFile>] [--verifier | --prover] [--address <address:port_number>]
```

#### Command line arguments
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
see examples below on how to use the flags.

__Note:__ Zilch automatically detects the private and public tapes if they are in the same directory as the zMIPS assembly file and they are named `pubtape.txt` and `auxtape.txt`.


### zMIPS examples

In the [examples-zmips](examples-zmips) directory we include various zMIPS examples.

At the assembler level, our labels are alphanumeric tags that begin and end by a double underscore (e.g., `__example_label__`), while inside Zilch these labels are converted to instruction numbers.

For instance, below is the zMIPS code to compute the factorial of 5:
```
move $t3, 5
move $t1, 1
move $t2, 1
__L1__:
    mult $t1, $t1, $t2
    add $t2, $t2, 1
bge $t3, $t2, __L1__
answer $t1
```


### User-defined Macros
In [macros.json](https://github.com/TrustworthyComputing/Zilch/blob/master/framework/zilch/src/macros.json) we define custom macro-instructions based on existing zMIPS instructions.
For instance, we have defined `inc` and `min` macro-instructions as shown below :
```
"inc": {
    "reg1": "$x",
    "macro": "add $x, $x, 1"
}
```
This means that `inc` uses one register. A zMIPS program can use the inc instruction as
```
move $t0, 5
inc $t0
answer $t0
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
            move $x, $z
            j __end_min_label__
            __min_label__
            move $x, $y
            __end_min_label__"
    }
```


### An example for read instructions:
Primary tape is filled with `1, 2, 3, 4, ...`, while aux tape contains `101, 102, 103, 104, ...`.
```
pubread $t0             ; consume next word from public tape and store it to r0
print $t0

secread $t1             ; consume next word from auxiliary tape and store it to r1
print $t1

pubseek $t0, 3          ; read the 4th word from the public tape and store it to r0
print $t0

secseek $t1, 3          ; read the 4th word from the auxiliary tape and store it to r1
print $t1

answer $t0
```
In order to execute the above program, simply run `./zilch --asm ./examples-zmips/read_test/read_test.zmips --tsteps 5 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape`.



### Over the Network Verification:
The default behavior (without flags `--address`, `--verifier`, `--prover`) of the `zilch` executable results in a local execution.
In order to enable over the network verification first the verifier should be executed (`--verifier` flag) and then the prover (`--prover` flag).
The verifier acts as a server waiting for the prover to connect, executes and prints and returns its decision to the prover.

For instance, a simple read from tapes example over the network:

First run the verifier listening on port `2324`:
```
./zilch --asm ./examples-zmips/read_test/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape --verifier --address localhost:2324
```
And then the prover to connect to port `2324`:
```
./zilch --asm ./examples-zmips/read_test/read_test.zmips --tsteps 10 --security 120 --pubtape ./examples-zmips/read_test/read_test.pubtape --auxtape ./examples-zmips/read_test/read_test.auxtape --prover --address localhost:2324
```


<p align="center">
    <img src="./logos/twc.png" height="20%" width="20%">
</p>
<h4 align="center">Trustworthy Computing Group</h4>
