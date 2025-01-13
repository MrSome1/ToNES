
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ADC $0,X ;
    AND $0,X ;
    ASL $0,X ;

    CMP $0,X ;

    DEC $0,X ;

    EOR $0,X ;

    INC $0,X ;

    LDA $0,X ;
    LDY $0,X ;
    LSR $0,X ;

    ORA $0,X ;

    ROL $0,X ;
    ROR $0,X ;

    SBC $0,X ;
    STA $0,X ;
    STY $0,X ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main