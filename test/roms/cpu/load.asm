
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1 ; @REG A=0x01 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    LDX #$2 ; @REG A=0x01 X=0x02 Y=0x00 S=0xfd P=0b00100100 PC=0x8004
    LDY #$3 ; @REG A=0x01 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x8006

.segment "INTERRUPT_VECTOR"
    .word main, main, main