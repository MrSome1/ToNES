
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ADC $0 ;
    AND $0 ;
    ASL $0 ;

    BIT $0 ;

    CMP $0 ;
    CPX $0 ;
    CPY $0 ;

    DEC $0 ;

    EOR $0 ;

    INC $0 ;

    LDA $0 ;
    LDX $0 ;
    LDY $0 ;
    LSR $0 ;

    ORA $0 ;

    ROL $0 ;
    ROR $0 ;

    SBC $0 ;
    STA $0 ;
    STX $0 ;
    STY $0 ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main