
;; Instructions to test
;;   + ADC AND ASL
;;   + BIT
;;   + CMP CPX CPY
;;   + DEC
;;   + EOR
;;   + INC
;;   + LDA LDX LDY LSR
;;   + ORA ROL ROR
;;   + SBC STA STX STY

.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    STA $0f ; 0x8000 @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002 @RAM 0x000f 0x00

    INC $0f ; 0x8002 @REG A=0x00 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004 @RAM 0x000f 0x01

    LDX $0f ; 0x8004 @REG A=0x00 X=0x01 Y=0x00 S=0xfd P=0b00100100 PC=0x8006

    ROR $0f ; 0x8006 @REG A=0x00 X=0x01 Y=0x00 S=0xfd P=0b10100101 PC=0x8008 @RAM 0x000f 0x80
    LSR $0f ; 0x8008 @REG A=0x00 X=0x01 Y=0x00 S=0xfd P=0b00100100 PC=0x800a @RAM 0x000f 0x40

    LDY $0f ; 0x800a @REG A=0x00 X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x800c

    DEC $0f ; 0x800c @REG A=0x00 X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x800e @RAM 0x000f 0x3f

    LDA $0f ; 0x800e @REG A=0x3f X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x8010

    STX $00 ; 0x8010 @REG A=0x3f X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x8012 @RAM 0x0000 0x01
    STY $ff ; 0x8012 @REG A=0x3f X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x8014 @RAM 0x00ff 0x40

    SBC $00 ; 0x8014 @REG A=0x3e X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x8016
    ADC $ff ; 0x8016 @REG A=0x7e X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x8018

    EOR $0f ; 0x8018 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b00100100 PC=0x801a

    BIT $ff ; 0x801a @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100100 PC=0x801c

    AND $ff ; 0x801c @REG A=0x40 X=0x01 Y=0x40 S=0xfd P=0b01100100 PC=0x801e
    ORA $00 ; 0x801e @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100100 PC=0x8020

    ASL $ff ; 0x8020 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b11100100 PC=0x8022 @RAM 0x00ff 0x80
    ROL $ff ; 0x8022 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100101 PC=0x8024 @RAM 0x00ff 0x01

    CMP $0f ; 0x8024 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100100 PC=0x8026
    CPX $00 ; 0x8026 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100110 PC=0x8028
    CPY $ff ; 0x8028 @REG A=0x41 X=0x01 Y=0x40 S=0xfd P=0b01100100 PC=0x802a

.segment "INTERRUPT_VECTOR"
    .word main, main, main