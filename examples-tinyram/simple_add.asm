move $t2, $t2, 13
addi $t2, $t2, 7
move $t1, $t1, 12
sub $t1, $t1, 1
sub $t1, $t1, 1
sll $t1, $t1, 1
lw $t5, 4($sp)
lw $t5, 512
addu $t3, $t1, $t2
answer $t3, $t3, $t3
