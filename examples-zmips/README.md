# ZMIPS ISA
zMIPS is our extension to the MIPS ISA to support programming ZKPs. Below we provide the zMIPS enhanced ISA:

| Instruction         | Description |
|---------------------|-------------|
| add $ri, $rj, A     | $ri = $rj + A |
| sub $ri, $rj, A     | $ri = $rj - A |
| mult $ri, $rj, A    | $ri = $rj * A |
| div $ri, $rj, A     | $ri = $rj / A |
| mod $ri, $rj, A     | $ri = $rj % A |
| move $ri, A         | $ri = A |
| la $ri, L           | $ri = L |
| and $ri, $rj, A     | $ri = $rj & A              |
| or $ri, $rj, A      | $ri = $rj \| A             |
| xor $ri, $rj, A     | $ri = $rj ^ A              |
| not $ri, A          | $ri = ~A                   |
| sll $ri, $rj, A     | $ri = $rj << A             |
| srl $ri, $rj, A     | $ri = $rj >> A             |
| beq $ri, $rj, L     | if $ri == $rj goto L        |
| bne $ri, $rj, L     | if $ri != $rj goto L        |
| blt $ri, $rj, L     | if $ri < $rj goto L         |
| ble $ri, $rj, L     | if $ri <= $rj goto L        |
| seq $ri, $rj, A     | if $ri = ($rj == A)        |
| sne $ri, $rj, A     | if $ri = ($rj != A)        |
| slt $ri, $rj, A     | if $ri = ($rj < A)        |
| sle $ri, $rj, A     | if $ri = ($rj <= A)        |
| j L                 | goto label L                |
| jr $ri              | goto instrcution pointed by $ri |
| sw $ri, A($rj)      | [A+$rj] = $ri               |
| lw $ri, A($rj)      | $ri = [A+$rj]               |
| pubread $ri         | $ri = next from public 		|
| secread $ri         | $ri = next from private		|
| pubseek $ri, A      | $ri = public[A]				|
| secseek $ri, A      | $ri = private[A]	   		|
| print $ri           | print ri	                |
| answer $ri          | return ri                   |


## Labels
In Zilch, both the prefix and the suffix of a the label should be `__`. For instance `__labelName__`.

### zMIPS Input: Enhanced read from tapes instructions
* `pubread $ri` : Consumes the next word from the public tape (if it has remaining words) and stores it in `$ri`.

* `secread $ri` : Consumes the next word from the private tape (if it has remaining words) and stores it in `$ri`.

* `pubseek $ri, $rj` : `seek` is a random access `read` command. `pubseek` consumes the word in offset `$rj` from the public tape. In case the tape does not have at least `$rj` words, this instruction raises an error.

* `secseek $ri, $rj` : `secseek` consumes the word in offset `$rj` from the private tape. In case the tape does not have at least `$rj` words, this instruction raises an error.


## User-defined Macros
In [macros.json](https://github.com/TrustworthyComputing/Zilch/blob/master/framework/zilch/src/macros.json) the user can define her own custom zMIPS macro-instructions.

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
answer $r0
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
```
move $r0, 5
move $r1, 9
min $r2, $r0, $r1
answer $r2
```
the answer would be 5.

