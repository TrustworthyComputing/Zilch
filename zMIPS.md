# ZMIPS ISA
zMIPS is our extension to the MIPS ISA to support programming ZKPs. Below we provide the zMIPS enhanced ISA:

| Instruction        | Description                                              |
|--------------------|----------------------------------------------------------|
| and $ri, $rj, A    | $ri, = $rj & A                                           |
| or $ri, $rj, A     | $ri, = $rj \| A                                          |
| xor $ri, $rj, A    | $ri, = $rj ^ A                                           |
| not $ri, $rj, A    | $ri, = !A                                                |
| add $ri, $rj, A    | $ri, = $rj + A                                           |
| sub $ri, $rj, A    | $ri, = $rj - A                                           |
| mult $ri, $rj, A   | $ri, = $rj * A                                           |
| sll $ri, $rj, A    | $ri, = $rj << A                                          |
| srl $ri, $rj, A    | $ri, = $rj >> A                                          |
| cmpe $ri, $rj, A   | flag = $rj == A                                          |
| cmpne $ri, $rj, A  | flag = $rj != A                                          |
| cmpg $ri, $rj, A   | flag = $rj > A                                           |
| cmpge $ri, $rj, A  | flag = $rj >= A                                          |
| beq $ri, $rj, A    | if $ri == $rj goto A                                     |
| bne $ri, $rj, A    | if $ri != $rj goto A                                     |
| bgt $ri, $rj, A    | if $ri > $rj goto A                                      |
| bge $ri, $rj, A    | if $ri >= $rj goto A                                     |
| blt $ri, $rj, A    | if $ri < $rj goto A                                      |
| ble $ri, $rj, A    | if $ri <= $rj goto A                                     |
| move $ri, $rj, A   | $ri = A                                                  |
| read $ri, $rj, A   | $ri = (A == 0) ? next from public : next from private    |
| seek $ri, $rj, A   | $ri = (A == 0) ? public[$rj] : $ri = private[$rj]        |
| j $ri, $rj, A      | goto label A                                             |
| cjmp $ri, $rj, A   | if (flag) then goto label A                              |
| cnjmp $ri, $rj, A  | if (!flag) then goto label A                             |
| sw $ri, A($rj)     | [A+$rj] = $ri                                            |
| lw $ri, A($rj)     | $ri = [A+$rj]                                            |
| answer $ri, $rj, A | return A                                                 |


## Labels
In Hyperion, both the prefix and the suffix of a the label should be `__`. For instance `__labelName__`.

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

