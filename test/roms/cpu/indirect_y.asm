
;; Instructions to test
;;   + ADC AND
;;   + CMP
;;   + EOR
;;   + LDA
;;   + ORA
;;   + SBC STA
;;
;; Almost the same routine with indirect_x.asm

.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ;; Store indirect address
    LDA #$03    ; 0x8000 @REG A=0x03 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    STA  $fe    ; 0x8002 @REG A=0x03 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004 @RAM 0x00fe 0x03
    LDA #$07    ; 0x8004 @REG A=0x07 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8006
    STA  $ff    ; 0x8006 @REG A=0x07 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8008 @RAM 0x00ff 0x07

    ;; Store a number in the indirect address
    LDX #$0a    ; 0x8008 @REG A=0x07 X=0x0a Y=0x00 S=0xfd P=0b00100100 PC=0x800a
    STX $0705   ; 0x800a @REG A=0x07 X=0x0a Y=0x00 S=0xfd P=0b00100100 PC=0x800d @RAM 0x0705 0x0a

    ;; Initialize register X
    LDY #$02    ; 0x800d @REG A=0x07 X=0x0a Y=0x02 S=0xfd P=0b00100100 PC=0x800f

    ORA ($fe),Y ; 0x800f @REG A=0x0f X=0x0a Y=0x02 S=0xfd P=0b00100100 PC=0x8011

    CMP ($fe),Y ; 0x8011 @REG A=0x0f X=0x0a Y=0x02 S=0xfd P=0b00100101 PC=0x8013

    SBC ($fe),Y ; 0x8013 @REG A=0x05 X=0x0a Y=0x02 S=0xfd P=0b00100101 PC=0x8015

    STA ($fe),Y ; 0x8015 @REG A=0x05 X=0x0a Y=0x02 S=0xfd P=0b00100101 PC=0x8017 @RAM 0x0705 0x05

    ADC ($fe),Y ; 0x8017 @REG A=0x0b X=0x0a Y=0x02 S=0xfd P=0b00100100 PC=0x8019

    AND ($fe),Y ; 0x8019 @REG A=0x01 X=0x0a Y=0x02 S=0xfd P=0b00100100 PC=0x801b

    LDA ($fe),Y ; 0x801b @REG A=0x05 X=0x0a Y=0x02 S=0xfd P=0b00100100 PC=0x801d

    EOR ($fe),Y ; 0x801d @REG A=0x00 X=0x0a Y=0x02 S=0xfd P=0b00100110 PC=0x801f

.segment "INTERRUPT_VECTOR"
    .word main, main, main