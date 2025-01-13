
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ADC ($0000),Y ;
    AND ($0000),Y ;

    CMP ($0000),Y ;

    EOR ($0000),Y ;

    LDA ($0000),Y ;

    ORA ($0000),Y ;

    SBC ($0000),Y ;
    STA ($0000),Y ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main