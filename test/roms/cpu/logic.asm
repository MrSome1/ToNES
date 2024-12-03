
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$55 ; @REG A=0x55 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    ASL      ; @REG A=0xaa X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8003
    LSR      ; @REG A=0x55 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004

.segment "INTERRUPT_VECTOR"
    .word main, main, main