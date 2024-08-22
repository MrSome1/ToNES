
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    NOP ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8001

.segment "INTERRUPT_VECTOR"
    .word main, main, main