
;; Instructions to test
;;   + ADC AND
;;   + CMP
;;   + EOR
;;   + LDA LDX
;;   + ORA
;;   + SBC STA

.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ;; Initialize
    LDA #$5a    ; 0x8000 @REG A=0x5a X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    LDY #$01    ; 0x8002 @REG A=0x5a X=0x00 Y=0x01 S=0xfd P=0b00100100 PC=0x8004
    STY  $0200  ; 0x8004 @REG A=0x5a X=0x00 Y=0x01 S=0xfd P=0b00100100 PC=0x8007 @RAM 0x0200 0x01

    STA $07ff,Y ; 0x8007 @REG A=0x5a X=0x00 Y=0x01 S=0xfd P=0b00100100 PC=0x800a @RAM 0x0000 0x5a

    LDX $07ff,Y ; 0x800a @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x800d

    CMP $07ff,Y ; 0x800d @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100110 PC=0x8010

    SBC $07ff,Y ; 0x8010 @REG A=0x00 X=0x5a Y=0x01 S=0xfd P=0b00100110 PC=0x8013
    ADC $07ff,Y ; 0x8013 @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x8016

    EOR $07ff,Y ; 0x8016 @REG A=0x00 X=0x5a Y=0x01 S=0xfd P=0b00100110 PC=0x8019

    LDA $07ff,Y ; 0x8019 @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x801c

    ORA $01ff,Y ; 0x801c @REG A=0x5b X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x801f
    AND $07ff,Y ; 0x801f @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x8022

.segment "INTERRUPT_VECTOR"
    .word main, main, main