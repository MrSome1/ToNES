
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDA #$1
    LDX #$1
    LDY #$1

.segment "INTERRUPT_VECTOR"
    .word main, main, main