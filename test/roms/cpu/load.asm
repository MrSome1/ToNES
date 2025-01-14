
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1 ; @REG A=0x01 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    LDX #$2 ; @REG A=0x01 X=0x02 Y=0x00 S=0xfd P=0b00100100 PC=0x8004
    LDY #$3 ; @REG A=0x01 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x8006

    STA $1  ; @REG A=0x01 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x8008 @RAM 0x0001 0x01
    STX $2  ; @REG A=0x01 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x800a @RAM 0x0002 0x02
    STY $3  ; @REG A=0x01 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x800c @RAM 0x0003 0x03

    LDA $2  ; @REG A=0x02 X=0x02 Y=0x03 S=0xfd P=0b00100100 PC=0x800e
    LDX $3  ; @REG A=0x02 X=0x03 Y=0x03 S=0xfd P=0b00100100 PC=0x8010
    LDY $1  ; @REG A=0x02 X=0x03 Y=0x01 S=0xfd P=0b00100100 PC=0x8012

.segment "INTERRUPT_VECTOR"
    .word main, main, main