
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ADC $0000,X ;
    AND $0000,X ;
    ASL $0000,X ;

    CMP $0000,X ;

    DEC $0000,X ;

    EOR $0000,X ;

    INC $0000,X ;

    LDA $0000,X ;
    LDY $0000,X ;
    LSR $0000,X ;

    ORA $0000,X ;

    ROL $0000,X ;
    ROR $0000,X ;

    SBC $0000,X ;
    STA $0000,X ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main