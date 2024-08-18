
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1 ; @REG
    TAX     ; @REG
    TAY     ; @REG

.segment "INTERRUPT_VECTOR"
    .word main, main, main