
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$0  ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100110 PC=0x8002
    LDA #$ff ; @REG A=0xff X=0x00 Y=0x00 S=0xfd P=0b10100100 PC=0x8004

.segment "INTERRUPT_VECTOR"
    .word main, main, main