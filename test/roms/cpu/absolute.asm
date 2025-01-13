
.segment "NES_FILE_HEADER"
.byte "NES", $1A, 2

.segment "CODE"
main:
    ADC $0000 ;
    AND $0000 ;
    ASL $0000 ;

    BIT $0000 ;

    CMP $0000 ;
    CPX $0000 ;
    CPY $0000 ;

    DEC $0000 ;

    EOR $0000 ;

    INC $0000 ;

    JMP $0000 ;
    JSR $0000 ;

    LDA $0000 ;
    LDX $0000 ;
    LDY $0000 ;
    LSR $0000 ;

    ORA $0000 ;

    ROL $0000 ;
    ROR $0000 ;

    SBC $0000 ;
    STA $0000 ;
    STX $0000 ;
    STY $0000 ;

.segment "INTERRUPT_VECTOR"
    .word main, main, main