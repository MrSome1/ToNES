
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    LDX $0,Y ;
    STX $0,Y ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main