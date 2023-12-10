
#include "Instruction.h"

namespace tones {
namespace cpu {
namespace code {

const Instruction_t *Null = &UnknownInstruction;

/* Instruction Group 0
 *
 *   Total: 16
 * 
 *       | IMM | ZP | - | ABS | - | ZP, X | - | ABS, X |
 *   BIT         +         +
 *   STY         +         +          +
 *   LDY    +    +         +          +            +
 *   CPY    +    +         + 
 *   CPX    +    +         + 
 */

// BIT
const Instruction_t BIT_ZP  = {BIT, ZeroPage, 3};
const Instruction_t BIT_ABS = {BIT, Absolute, 4};

// STY
const Instruction_t STY_ZP    = {STY, ZeroPage, 3};
const Instruction_t STY_ABS   = {STY, Absolute, 4};
const Instruction_t STY_ZP_X  = {STY, IndexedZeroPageX, 4};

// LDY
const Instruction_t LDY_IMM   = {LDY, Immediate, 2};
const Instruction_t LDY_ZP    = {LDY, ZeroPage, 3};
const Instruction_t LDY_ABS   = {LDY, Absolute, 4};
const Instruction_t LDY_ZP_X  = {LDY, IndexedZeroPageX, 4};
const Instruction_t LDY_ABS_X = {LDY, IndexedAbsoluteX, 4};

// CPY
const Instruction_t CPY_IMM = {CPY, Immediate, 2};
const Instruction_t CPY_ZP  = {CPY, ZeroPage, 3};
const Instruction_t CPY_ABS = {CPY, Absolute, 4};

// CPX
const Instruction_t CPX_IMM = {CPX, Immediate, 2};
const Instruction_t CPX_ZP  = {CPX, ZeroPage, 3};
const Instruction_t CPX_ABS = {CPX, Absolute, 4};
   
/* Instruction Group 1
 *
 *   Total: 64
 * 
 *       | (IND, X) | ZP | IMM | ABS | (IND), Y | ZP, X | ABS, Y | ABS, X |
 *   ORA       +      +     +     +        +        +       +        +
 *   AND       +      +     +     +        +        +       +        +
 *   EOR       +      +     +     +        +        +       +        +
 *   ADC       +      +     +     +        +        +       +        +
 *   STA       +      +     +     +        +        +       +        +
 *   LDA       +      +     +     +        +        +       +        +
 *   CMP       +      +     +     +        +        +       +        +
 *   SBC       +      +     +     +        +        +       +        +
 */
   
// ORA
const Instruction_t ORA_IND_X = {ORA, IndexedIndirect, 6};
const Instruction_t ORA_ZP    = {ORA, ZeroPage, 3};
const Instruction_t ORA_IMM   = {ORA, Immediate, 2};
const Instruction_t ORA_ABS   = {ORA, Absolute, 4};
const Instruction_t ORA_IND_Y = {ORA, IndirectIndexed, 5};
const Instruction_t ORA_ZP_X  = {ORA, IndexedZeroPageX, 4};
const Instruction_t ORA_ABS_Y = {ORA, IndexedAbsoluteY, 4};
const Instruction_t ORA_ABS_X = {ORA, IndexedAbsoluteX, 4};

// AND
const Instruction_t AND_IND_X = {AND, IndexedIndirect, 6};
const Instruction_t AND_ZP    = {AND, ZeroPage, 3};
const Instruction_t AND_IMM   = {AND, Immediate, 2};
const Instruction_t AND_ABS   = {AND, Absolute, 4};
const Instruction_t AND_IND_Y = {AND, IndirectIndexed, 5};
const Instruction_t AND_ZP_X  = {AND, IndexedZeroPageX, 4};
const Instruction_t AND_ABS_Y = {AND, IndexedAbsoluteY, 4};
const Instruction_t AND_ABS_X = {AND, IndexedAbsoluteX, 4};

// EOR
const Instruction_t EOR_IND_X = {EOR, IndexedIndirect, 6};
const Instruction_t EOR_ZP    = {EOR, ZeroPage, 3};
const Instruction_t EOR_IMM   = {EOR, Immediate, 2};
const Instruction_t EOR_ABS   = {EOR, Absolute, 4};
const Instruction_t EOR_IND_Y = {EOR, IndirectIndexed, 5};
const Instruction_t EOR_ZP_X  = {EOR, IndexedZeroPageX, 4};
const Instruction_t EOR_ABS_Y = {EOR, IndexedAbsoluteY, 4};
const Instruction_t EOR_ABS_X = {EOR, IndexedAbsoluteX, 4};

// ADC
const Instruction_t ADC_IND_X = {ADC, IndexedIndirect, 6};
const Instruction_t ADC_ZP    = {ADC, ZeroPage, 3};
const Instruction_t ADC_IMM   = {ADC, Immediate, 2};
const Instruction_t ADC_ABS   = {ADC, Absolute, 4};
const Instruction_t ADC_IND_Y = {ADC, IndirectIndexed, 5};
const Instruction_t ADC_ZP_X  = {ADC, IndexedZeroPageX, 4};
const Instruction_t ADC_ABS_Y = {ADC, IndexedAbsoluteY, 4};
const Instruction_t ADC_ABS_X = {ADC, IndexedAbsoluteX, 4};

// STA
const Instruction_t STA_IND_X = {STA, IndexedIndirect, 6};
const Instruction_t STA_ZP    = {STA, ZeroPage, 3};
const Instruction_t STA_IMM   = {STA, Immediate, 2};
const Instruction_t STA_ABS   = {STA, Absolute, 4};
const Instruction_t STA_IND_Y = {STA, IndirectIndexed, 5};
const Instruction_t STA_ZP_X  = {STA, IndexedZeroPageX, 4};
const Instruction_t STA_ABS_Y = {STA, IndexedAbsoluteY, 4};
const Instruction_t STA_ABS_X = {STA, IndexedAbsoluteX, 4};

// LDA
const Instruction_t LDA_IND_X = {LDA, IndexedIndirect, 6};
const Instruction_t LDA_ZP    = {LDA, ZeroPage, 3};
const Instruction_t LDA_IMM   = {LDA, Immediate, 2};
const Instruction_t LDA_ABS   = {LDA, Absolute, 4};
const Instruction_t LDA_IND_Y = {LDA, IndirectIndexed, 5};
const Instruction_t LDA_ZP_X  = {LDA, IndexedZeroPageX, 4};
const Instruction_t LDA_ABS_Y = {LDA, IndexedAbsoluteY, 4};
const Instruction_t LDA_ABS_X = {LDA, IndexedAbsoluteX, 4};

// CMP
const Instruction_t CMP_IND_X = {CMP, IndexedIndirect, 6};
const Instruction_t CMP_ZP    = {CMP, ZeroPage, 3};
const Instruction_t CMP_IMM   = {CMP, Immediate, 2};
const Instruction_t CMP_ABS   = {CMP, Absolute, 4};
const Instruction_t CMP_IND_Y = {CMP, IndirectIndexed, 5};
const Instruction_t CMP_ZP_X  = {CMP, IndexedZeroPageX, 4};
const Instruction_t CMP_ABS_Y = {CMP, IndexedAbsoluteY, 4};
const Instruction_t CMP_ABS_X = {CMP, IndexedAbsoluteX, 4};

// SBC
const Instruction_t SBC_IND_X = {SBC, IndexedIndirect, 6};
const Instruction_t SBC_ZP    = {SBC, ZeroPage, 3};
const Instruction_t SBC_IMM   = {SBC, Immediate, 2};
const Instruction_t SBC_ABS   = {SBC, Absolute, 4};
const Instruction_t SBC_IND_Y = {SBC, IndirectIndexed, 5};
const Instruction_t SBC_ZP_X  = {SBC, IndexedZeroPageX, 4};
const Instruction_t SBC_ABS_Y = {SBC, IndexedAbsoluteY, 4};
const Instruction_t SBC_ABS_X = {SBC, IndexedAbsoluteX, 4};

/* Instruction Group 2
 *
 *   Total: 36
 * 
 *       | IMM | ZP | Accum | ABS | - | ZP, X | - | ABS, X |
 *   ASL         +      +      +          +            +
 *   ROL         +      +      +          +            +
 *   LSR         +      +      +          +            +
 *   ROR         +      +      +          +            +
 *   STX         +             +        ZP, Y
 *   LDX    +    +             +        ZP, Y       ABS, Y
 *   DEC         +             +          +            +
 *   INC         +             +          +            +
 */

// ASL
const Instruction_t ASL_ZP    = {ASL, ZeroPage, 5};
const Instruction_t ASL_Accum = {ASL, Accumulator, 2};
const Instruction_t ASL_ABS   = {ASL, Absolute, 6};
const Instruction_t ASL_ZP_X  = {ASL, IndexedZeroPageX, 6};
const Instruction_t ASL_ABS_X = {ASL, IndexedAbsoluteX, 7};

// ROL
const Instruction_t ROL_ZP    = {ROL, ZeroPage, 5};
const Instruction_t ROL_Accum = {ROL, Accumulator, 2};
const Instruction_t ROL_ABS   = {ROL, Absolute, 6};
const Instruction_t ROL_ZP_X  = {ROL, IndexedZeroPageX, 6};
const Instruction_t ROL_ABS_X = {ROL, IndexedAbsoluteX, 7};

// LSR
const Instruction_t LSR_ZP    = {LSR, ZeroPage, 5};
const Instruction_t LSR_Accum = {LSR, Accumulator, 2};
const Instruction_t LSR_ABS   = {LSR, Absolute, 6};
const Instruction_t LSR_ZP_X  = {LSR, IndexedZeroPageX, 6};
const Instruction_t LSR_ABS_X = {LSR, IndexedAbsoluteX, 7};

// ROR
const Instruction_t ROR_ZP    = {ROR, ZeroPage, 5};
const Instruction_t ROR_Accum = {ROR, Accumulator, 2};
const Instruction_t ROR_ABS   = {ROR, Absolute, 6};
const Instruction_t ROR_ZP_X  = {ROR, IndexedZeroPageX, 6};
const Instruction_t ROR_ABS_X = {ROR, IndexedAbsoluteX, 7};

// STX
const Instruction_t STX_ZP    = {STX, ZeroPage, 3};
const Instruction_t STX_ABS   = {STX, Absolute, 4};
const Instruction_t STX_ZP_Y  = {STX, IndexedZeroPageY, 4};

// LDX
const Instruction_t LDX_IMM   = {LDX, Immediate, 2};
const Instruction_t LDX_ZP    = {LDX, ZeroPage, 3};
const Instruction_t LDX_ABS   = {LDX, Absolute, 4};
const Instruction_t LDX_ZP_Y  = {LDX, IndexedZeroPageY, 4};
const Instruction_t LDX_ABS_Y = {LDX, IndexedAbsoluteY, 4};

// DEC
const Instruction_t DEC_ZP    = {DEC, ZeroPage, 5};
const Instruction_t DEC_ABS   = {DEC, Absolute, 6};
const Instruction_t DEC_ZP_X  = {DEC, IndexedZeroPageX, 6};
const Instruction_t DEC_ABS_X = {DEC, IndexedAbsoluteX, 7};

// INC
const Instruction_t INC_ZP    = {INC, ZeroPage, 5};
const Instruction_t INC_ABS   = {INC, Absolute, 6};
const Instruction_t INC_ZP_X  = {INC, IndexedZeroPageX, 6};
const Instruction_t INC_ABS_X = {INC, IndexedAbsoluteX, 7};

/* Instruction Group 3
 * 
 * Group 3 is totally empty according to the manual of 6502
 * processor, just for future extention instructions here
 */

/* Un-grouped Instructions
 *
 */

// BRK
const Instruction_t BRK_Implied = {BRK, Implied, 7};

// BPL
const Instruction_t BPL_Relative = {BPL, Relative, 2};

// JSR
const Instruction_t JSR_Absolute = {JSR, Absolute, 6};

// BMI
const Instruction_t BMI_Relative = {BMI, Relative, 2};

// RTI
const Instruction_t RTI_Implied = {BRK, Implied, 6};

// BVC
const Instruction_t BVC_Relative = {BVC, Relative, 2};

// RTS
const Instruction_t RTS_Implied = {RTS, Implied, 6};

// BVS
const Instruction_t BVS_Relative = {BVS, Relative, 2};

// BCC
const Instruction_t BCC_Relative = {BCC, Relative, 2};

// BCS
const Instruction_t BCS_Relative = {BCS, Relative, 2};

// BNE
const Instruction_t BNE_Relative = {BNE, Relative, 2};

// BEQ
const Instruction_t BEQ_Relative = {BEQ, Relative, 2};

// PHP
const Instruction_t PHP_Implied = {PHP, Implied, 3};

// CLC
const Instruction_t CLC_Implied = {CLC, Implied, 2};

// PLP
const Instruction_t PLP_Implied = {PLP, Implied, 4};

// SEC
const Instruction_t SEC_Implied = {SEC, Implied, 2};

// PHA
const Instruction_t PHA_Implied = {PHA, Implied, 3};

// CLI
const Instruction_t CLI_Implied = {CLI, Implied, 2};

// PLA
const Instruction_t PLA_Implied = {PLA, Implied, 4};

// SEI
const Instruction_t SEI_Implied = {SEI, Implied, 2};

// DEY
const Instruction_t DEY_Implied = {DEY, Implied, 2};

// TYA
const Instruction_t TYA_Implied = {TYA, Implied, 2};

// TAY
const Instruction_t TAY_Implied = {TAY, Implied, 2};

// CLV
const Instruction_t CLV_Implied = {CLV, Implied, 2};

// INY
const Instruction_t INY_Implied = {INY, Implied, 2};

// CLD
const Instruction_t CLD_Implied = {CLD, Implied, 2};

// INX
const Instruction_t INX_Implied = {INX, Implied, 2};

// SED
const Instruction_t SED_Implied = {SED, Implied, 2};

// TXA
const Instruction_t TXA_Implied = {TXA, Implied, 2};

// TXS
const Instruction_t TXS_Implied = {TXS, Implied, 2};

// TAX
const Instruction_t TAX_Implied = {TAX, Implied, 2};

// TSX
const Instruction_t TSX_Implied = {TSX, Implied, 2};

// DEX
const Instruction_t DEX_Implied = {DEX, Implied, 2};

// NOP
const Instruction_t NOP_Implied = {NOP, Implied, 2};

// JMP
const Instruction_t JMP_ABS = {JMP, Absolute, 3};
const Instruction_t JMP_Indirect = {JMP, AbsoluteIndirect, 5};

/* Instruction Set
 *
 * This the whole instruction set of 6502 processor, an 1:1
 * copy of the manual
 */
const std::array<const Instruction_t*, InstructionSetSize> InstructionSet = {
/*      0              1           2         3     4          5          6          7     8             9           A             B              C     D           E           F */
/* 0 */ &BRK_Implied,  &ORA_IND_X, Null,     Null, Null,      &ORA_ZP  , &ASL_ZP,   Null, &PHP_Implied, &ORA_IMM  , &ASL_Accum,   Null,          Null, &ORA_ABS  , &ASL_ABS,   Null,
/* 1 */ &BPL_Relative, &ORA_IND_Y, Null,     Null, Null,      &ORA_ZP_X, &ASL_ZP_X, Null, &CLC_Implied, &ORA_ABS_Y, Null,         Null,          Null, &ORA_ABS_X, &ASL_ABS_X, Null,
/* 2 */ &JSR_Absolute, &AND_IND_X, Null,     Null, &BIT_ZP,   &AND_ZP  , &ROL_ZP,   Null, &PLP_Implied, &AND_IMM  , &ROL_Accum,   &BIT_ABS,      Null, &AND_ABS  , &ROL_ABS,   Null,
/* 3 */ &BMI_Relative, &AND_IND_Y, Null,     Null, Null,      &AND_ZP_X, &ROL_ZP_X, Null, &SEC_Implied, &AND_ABS_Y, Null,         Null,          Null, &AND_ABS_X, &ROL_ABS_X, Null,
/* 4 */ &RTI_Implied,  &EOR_IND_X, Null,     Null, Null,      &EOR_ZP  , &LSR_ZP  , Null, &PHA_Implied, &EOR_IMM  , &LSR_Accum,   &JMP_ABS,      Null, &EOR_ABS  , &LSR_ABS,   Null,
/* 5 */ &BVC_Relative, &EOR_IND_Y, Null,     Null, Null,      &EOR_ZP_X, &LSR_ZP_X, Null, &CLI_Implied, &EOR_ABS_Y, Null,         Null,          Null, &EOR_ABS_X, &LSR_ABS_X, Null,
/* 6 */ &RTS_Implied,  &ADC_IND_X, Null,     Null, Null,      &ADC_ZP  , &ROR_ZP  , Null, &PLA_Implied, &ADC_IMM  , &ROR_Accum,   &JMP_Indirect, Null, &ADC_ABS  , &ROR_ABS,   Null,
/* 7 */ &BVS_Relative, &ADC_IND_Y, Null,     Null, Null,      &ADC_ZP_X, &ROR_ZP_X, Null, &SEI_Implied, &ADC_ABS_Y, Null,         Null,          Null, &ADC_ABS_X, &ROR_ABS_X, Null,
/* 8 */ Null,          &STA_IND_X, Null,     Null, &STY_ZP,   &STA_ZP  , &STX_ZP  , Null, &DEY_Implied, &STA_IMM  , &TXA_Implied, &STY_ABS,      Null, &STA_ABS  , &STX_ABS,   Null,
/* 9 */ &BCC_Relative, &STA_IND_Y, Null,     Null, &STY_ZP_X, &STA_ZP_X, &STX_ZP_Y, Null, &TYA_Implied, &STA_ABS_Y, &TXS_Implied, Null,          Null, &STA_ABS_X, Null,       Null,
/* A */ &LDY_IMM,      &LDA_IND_X, &LDX_IMM, Null, &LDY_ZP,   &LDA_ZP  , &LDX_ZP  , Null, &TAY_Implied, &LDA_IMM  , &TAX_Implied, &LDY_ABS,      Null, &LDA_ABS  , &LDX_ABS,   Null,
/* B */ &BCS_Relative, &LDA_IND_Y, Null,     Null, &LDY_ZP_X, &LDA_ZP_X, &LDX_ZP_Y, Null, &CLV_Implied, &LDA_ABS_Y, &TSX_Implied, &LDY_ABS_X,    Null, &LDA_ABS_X, &LDX_ABS_Y, Null,
/* C */ &CPY_IMM,      &CMP_IND_X, Null,     Null, &CPY_ZP,   &CMP_ZP  , &DEC_ZP  , Null, &INY_Implied, &CMP_IMM  , &DEX_Implied, &DEC_ABS,      Null, &CMP_ABS  , &DEC_ABS,   Null,
/* D */ &BNE_Relative, &CMP_IND_Y, Null,     Null, Null,      &CMP_ZP_X, &DEC_ZP_X, Null, &CLD_Implied, &CMP_ABS_Y, Null,         &DEC_ABS_X,    Null, &CMP_ABS_X, &DEC_ABS_X, Null,
/* E */ &CPX_IMM,      &SBC_IND_X, Null,     Null, &CPX_ZP,   &SBC_ZP  , &INC_ZP  , Null, &INX_Implied, &SBC_IMM  , &NOP_Implied, &INC_ABS,      Null, &SBC_ABS  , &INC_ABS,   Null,
/* F */ &BEQ_Relative, &SBC_IND_Y, Null,     Null, Null,      &SBC_ZP_X, &INC_ZP_X, Null, &SED_Implied, &SBC_ABS_Y, Null,         &INC_ABS_X,    Null, &SBC_ABS_X, &INC_ABS_X, Null,
};

} // namespace code
} // namespace cpu
} // namespace tones
