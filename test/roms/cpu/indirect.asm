
;; Instructions to test
;;   + JMP

.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ;; Initialize
    LDA #$01    ; @REG A=0x01 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    STA  $f0    ; @REG A=0x01 X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004 @RAM 0x00f0 0x01
    LDA #$cc    ; @REG A=0xcc X=0x00 Y=0x00 S=0xfd P=0b10100100 PC=0x8006
    STA  $f1    ; @REG A=0xcc X=0x00 Y=0x00 S=0xfd P=0b10100100 PC=0x8008 @RAM 0x00f1 0xcc

    JMP ($00f0) ; @REG A=0xcc X=0x00 Y=0x00 S=0xfd P=0b10100100 PC=0xcc01

.segment "INTERRUPT_VECTOR"
    .word main, main, main