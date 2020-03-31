  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #


.global delay
.global enable_interrupt

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	    \reg,0($sp)
.endm

.macro	POP reg
	lw	    \reg,0($sp)
	addi	$sp,$sp,4
.endm

.data
.align 2
.text
main:
  #
  # Please add your own code below this lin
  #


delay:     #$a0 = ms i c
    move    $s3,$a0


while:
    ble     $s3,$0,done
    nop
    addi    $s3,$s3,-1
    addi    $s4,$0,0        #int i = 0;
    addi    $t3,$0,32000       # t3 = 4711, 'The constant 4711 must be easy to change'
loop:
    slt     $t4,$s4,$t3        #om i < 4711, at = 1. om i > 4711, at = 0
    beq     $t4,$0,while
    nop        #go to while om i = > 4711
    addi    $s4,$s4,1        # i = i +1;
    j       loop
    nop

done:
    jr     $ra
    nop

enable_interrupt:

ei
jr $ra
nop
