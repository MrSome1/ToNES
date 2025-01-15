
;; Instructions to test
;;   + LDX
;;   + STX

.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ;; Initialize
    LDA #$5a   ; @REG A=0x5a X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8002
    STA  $ff   ; @REG A=0x5a X=0x00 Y=0x00 S=0xfd P=0b00100100 PC=0x8004 @RAM 0x00ff 0x5a
    LDY #$01   ; @REG A=0x5a X=0x00 Y=0x01 S=0xfd P=0b00100100 PC=0x8006

    LDX  $fe,Y ; @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x8008
    STX  $ff,Y ; @REG A=0x5a X=0x5a Y=0x01 S=0xfd P=0b00100100 PC=0x800a @RAM 0x0000 0x5a

.segment "INTERRUPT_VECTOR"
    .word main, main, main