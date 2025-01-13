
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    JMP ($0000) ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main