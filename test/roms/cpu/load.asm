
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1 ; @REG
    LDX #$1 ; @REG
    LDY #$1 ; @REG

.segment "INTERRUPT_VECTOR"
    .word main, main, main