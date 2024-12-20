
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    SEC ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100101 PC=0x8001
    CLC ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002

    CLI ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100000 PC=0x8003
    SEI ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004

    SED ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00101100 PC=0x8005
    CLD ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8006
    
    CLV ; @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8007

.segment "INTERRUPT_VECTOR"
    .word main, main, main