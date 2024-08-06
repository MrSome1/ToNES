
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1
    TAX
    TAY

.segment "INTERRUPT_VECTOR"
    .word main, main, main