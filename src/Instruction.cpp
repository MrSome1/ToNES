
#include "Instruction.h"

namespace tones {
namespace cpu {

/* Addressing Modes */

const AddressingMode_t IMP = { code::Implied,          0, "IMP" };
const AddressingMode_t ACC = { code::Accumulator,      0, "ACC" };
const AddressingMode_t IMM = { code::Immediate,        1, "IMM" };
const AddressingMode_t ABS = { code::Absolute,         2, "ABS" };
const AddressingMode_t ZP  = { code::ZeroPage,         1,  "ZP" };
const AddressingMode_t ZPX = { code::IndexedZeroPageX, 1, "ZPX" };
const AddressingMode_t ZPY = { code::IndexedZeroPageY, 1, "ZPY" };
const AddressingMode_t ABX = { code::IndexedAbsoluteX, 2, "ABX" };
const AddressingMode_t ABY = { code::IndexedAbsoluteY, 2, "ABY" };
const AddressingMode_t REL = { code::Relative,         1, "REL" };
const AddressingMode_t IDX = { code::IndexedIndirect,  1, "IDX" };
const AddressingMode_t IDY = { code::IndirectIndexed,  1, "IDY" };
const AddressingMode_t IND = { code::AbsoluteIndirect, 2, "IND" };

const AddressingMode_t INVALID = { code::Invalid, 0, "INVALID" };

/* Instructions */

const Instruction_t ADC = { code::ADC, code::R , "ADC" };
const Instruction_t AND = { code::AND, code::R , "AND" };
const Instruction_t ASL = { code::ASL, code::RW, "ASL" };

const Instruction_t BCC = { code::BCC, code::R , "BCC" };
const Instruction_t BCS = { code::BCS, code::R , "BCS" };
const Instruction_t BEQ = { code::BEQ, code::R , "BEQ" };
const Instruction_t BIT = { code::BIT, code::R , "BIT" };
const Instruction_t BMI = { code::BMI, code::R , "BMI" };
const Instruction_t BNE = { code::BNE, code::R , "BNE" };
const Instruction_t BPL = { code::BPL, code::R , "BPL" };
const Instruction_t BRK = { code::BRK, code::NO, "BRK" };
const Instruction_t BVC = { code::BVC, code::R , "BVC" };
const Instruction_t BVS = { code::BVS, code::R , "BVS" };

const Instruction_t CLC = { code::CLC, code::NO, "CLC" };
const Instruction_t CLD = { code::CLD, code::NO, "CLD" };
const Instruction_t CLI = { code::CLI, code::NO, "CLI" };
const Instruction_t CLV = { code::CLV, code::NO, "CLV" };
const Instruction_t CMP = { code::CMP, code::R , "CMP" };
const Instruction_t CPX = { code::CPX, code::R , "CPX" };
const Instruction_t CPY = { code::CPY, code::R , "CPY" };

const Instruction_t DEC = { code::DEC, code::RW, "DEC" };
const Instruction_t DEX = { code::DEX, code::NO, "DEX" };
const Instruction_t DEY = { code::DEY, code::NO, "DEY" };

const Instruction_t EOR = { code::EOR, code::R , "EOR" };

const Instruction_t INC = { code::INC, code::RW, "INC" };
const Instruction_t INX = { code::INX, code::NO, "INX" };
const Instruction_t INY = { code::INY, code::NO, "INY" };

const Instruction_t JMP = { code::JMP, code::R , "JMP" };
const Instruction_t JSR = { code::JSR, code::R , "JSR" };

const Instruction_t LDA = { code::LDA, code::R , "LDA" };
const Instruction_t LDX = { code::LDX, code::R , "LDX" };
const Instruction_t LDY = { code::LDY, code::R , "LDY" };
const Instruction_t LSR = { code::LSR, code::RW, "LSR" };

const Instruction_t NOP = { code::NOP, code::NO, "NOP" };

const Instruction_t ORA = { code::ORA, code::R , "ORA" };

const Instruction_t PHA = { code::PHA, code::NO, "PHA" };
const Instruction_t PHP = { code::PHP, code::NO, "PHP" };
const Instruction_t PLA = { code::PLA, code::NO, "PLA" };
const Instruction_t PLP = { code::PLP, code::NO, "PLP" };

const Instruction_t ROL = { code::ROL, code::RW, "ROL" };
const Instruction_t ROR = { code::ROR, code::RW, "ROR" };
const Instruction_t RTI = { code::RTI, code::NO, "RTI" };
const Instruction_t RTS = { code::RTS, code::NO, "RTS" };

const Instruction_t SBC = { code::SBC, code::R , "SBC" };
const Instruction_t SEC = { code::SEC, code::NO, "SEC" };
const Instruction_t SED = { code::SED, code::NO, "SED" };
const Instruction_t SEI = { code::SEI, code::NO, "SEI" };
const Instruction_t STA = { code::STA, code::W , "STA" };
const Instruction_t STX = { code::STX, code::W , "STX" };
const Instruction_t STY = { code::STY, code::W , "STY" };

const Instruction_t TAX = { code::TAX, code::NO, "TAX" };
const Instruction_t TAY = { code::TAY, code::NO, "TAY" };
const Instruction_t TSX = { code::TSX, code::NO, "TSX" };
const Instruction_t TXA = { code::TXA, code::NO, "TXA" };
const Instruction_t TXS = { code::TXS, code::NO, "TXS" };
const Instruction_t TYA = { code::TYA, code::NO, "TYA" };

const Instruction_t UNKNOWN = { code::Unknown, code::NO, "Unknown"};

/* Operations */

const Operation_t UnknownOperation = { &UNKNOWN, &INVALID, 0 };
const Operation_t *Null = &UnknownOperation;

/** Routine Instructions
 *
 *   Total: 6
 * 
 *   Code Shape: 0XX0 XX00
 */

// BRK - 0000 0000
const Operation_t BRK_IMP = { &BRK, &IMP, 7};

// JSR - 0010 0000
const Operation_t JSR_ABS = { &JSR, &ABS, 6};

// RTI - 0100 0000
const Operation_t RTI_IMP = { &BRK, &IMP, 6};

// RTS - 0110 0000
const Operation_t RTS_IMP = { &RTS, &IMP, 6};

// JMP - 01X0 1100
const Operation_t JMP_ABS = { &JMP, &ABS, 3};
const Operation_t JMP_IND = { &JMP, &IND, 5};

/** Branch Instructions
 *
 *   Total: 8
 * 
 *   Code Shape: XXX1 0000
 */

// BPL - 0001 0000
const Operation_t BPL_REL = { &BPL, &REL, 2};

// BMI - 0011 0000
const Operation_t BMI_REL = { &BMI, &REL, 2};

// BVC - 0101 0000
const Operation_t BVC_REL = { &BVC, &REL, 2};

// BVS - 0111 0000
const Operation_t BVS_REL = { &BVS, &REL, 2};

// BCC - 1001 0000
const Operation_t BCC_REL = { &BCC, &REL, 2};

// BCS - 1011 0000
const Operation_t BCS_REL = { &BCS, &REL, 2};

// BNE - 1101 0000
const Operation_t BNE_REL = { &BNE, &REL, 2};

// BEQ - 1111 0000
const Operation_t BEQ_REL = { &BEQ, &REL, 2};

/** Stack Instructions
 *
 *   Total: 4
 * 
 *   Code Shape: 0XX0 1000
 */

// PHP - 0000 1000
const Operation_t PHP_IMP = { &PHP, &IMP, 3};

// PLP - 0010 1000
const Operation_t PLP_IMP = { &PLP, &IMP, 4};

// PHA - 0100 1000
const Operation_t PHA_IMP = { &PHA, &IMP, 3};

// PLA - 0110 1000
const Operation_t PLA_IMP = { &PLA, &IMP, 4};

/** Status Instructions
 *
 *   Total: 7
 * 
 *   Code Shape: XXX1 1000
 */

// CLC - 0001 1000
const Operation_t CLC_IMP = { &CLC, &IMP, 2};

// SEC - 0011 1000
const Operation_t SEC_IMP = { &SEC, &IMP, 2};

// CLI - 0101 1000
const Operation_t CLI_IMP = { &CLI, &IMP, 2};

// SEI - 0111 1000
const Operation_t SEI_IMP = { &SEI, &IMP, 2};

// CLV - 1011 1000
const Operation_t CLV_IMP = { &CLV, &IMP, 2};

// CLD - 1101 1000
const Operation_t CLD_IMP = { &CLD, &IMP, 2};

// SED - 1111 1000
const Operation_t SED_IMP = { &SED, &IMP, 2};

/** Index Instructions
 *
 *   Total: 4
 * 
 *   Code Shape: 1XX0 10X0
 */

// DEY - 1000 1000
const Operation_t DEY_IMP = { &DEY, &IMP, 2};

// INY - 1100 1000
const Operation_t INY_IMP = { &INY, &IMP, 2};

// INX - 1110 1000
const Operation_t INX_IMP = { &INX, &IMP, 2};

// DEX - 1100 1010
const Operation_t DEX_IMP = { &DEX, &IMP, 2};

/** Transfer Instructions
 *
 *   Total: 6
 * 
 *   Code Shape: 10XX 10X0
 */

// TYA - 1001 1000
const Operation_t TYA_IMP = { &TYA, &IMP, 2};

// TAY - 1010 1000
const Operation_t TAY_IMP = { &TAY, &IMP, 2};

// TXA - 1000 1010
const Operation_t TXA_IMP = { &TXA, &IMP, 2};

// TXS - 1001 1010
const Operation_t TXS_IMP = { &TXS, &IMP, 2};

// TAX - 1010 1010
const Operation_t TAX_IMP = { &TAX, &IMP, 2};

// TSX - 1011 1010
const Operation_t TSX_IMP = { &TSX, &IMP, 2};

/* No Operation */

// NOP - 1110 1010
const Operation_t NOP_IMP = { &NOP, &IMP, 2};

/** Instruction Group 0
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
const Operation_t BIT_ZP  = { &BIT, &ZP, 3};
const Operation_t BIT_ABS = { &BIT, &ABS, 4};

// STY
const Operation_t STY_ZP  = { &STY, &ZP, 3};
const Operation_t STY_ABS = { &STY, &ABS, 4};
const Operation_t STY_ZPX = { &STY, &ZPX, 4};

// LDY
const Operation_t LDY_IMM = { &LDY, &IMM, 2};
const Operation_t LDY_ZP  = { &LDY, &ZP, 3};
const Operation_t LDY_ABS = { &LDY, &ABS, 4};
const Operation_t LDY_ZPX = { &LDY, &ZPX, 4};
const Operation_t LDY_ABX = { &LDY, &ABX, 4};

// CPY
const Operation_t CPY_IMM = { &CPY, &IMM, 2};
const Operation_t CPY_ZP  = { &CPY, &ZP, 3};
const Operation_t CPY_ABS = { &CPY, &ABS, 4};

// CPX
const Operation_t CPX_IMM = { &CPX, &IMM, 2};
const Operation_t CPX_ZP  = { &CPX, &ZP, 3};
const Operation_t CPX_ABS = { &CPX, &ABS, 4};
   
/** Instruction Group 1
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
const Operation_t ORA_IDX = { &ORA, &IDX, 6};
const Operation_t ORA_ZP  = { &ORA, &ZP, 3};
const Operation_t ORA_IMM = { &ORA, &IMM, 2};
const Operation_t ORA_ABS = { &ORA, &ABS, 4};
const Operation_t ORA_IDY = { &ORA, &IDY, 5};
const Operation_t ORA_ZPX = { &ORA, &ZPX, 4};
const Operation_t ORA_ABY = { &ORA, &ABY, 4};
const Operation_t ORA_ABX = { &ORA, &ABX, 4};

// AND
const Operation_t AND_IDX = { &AND, &IDX, 6};
const Operation_t AND_ZP  = { &AND, &ZP, 3};
const Operation_t AND_IMM = { &AND, &IMM, 2};
const Operation_t AND_ABS = { &AND, &ABS, 4};
const Operation_t AND_IDY = { &AND, &IDY, 5};
const Operation_t AND_ZPX = { &AND, &ZPX, 4};
const Operation_t AND_ABY = { &AND, &ABY, 4};
const Operation_t AND_ABX = { &AND, &ABX, 4};

// EOR
const Operation_t EOR_IDX = { &EOR, &IDX, 6};
const Operation_t EOR_ZP  = { &EOR, &ZP, 3};
const Operation_t EOR_IMM = { &EOR, &IMM, 2};
const Operation_t EOR_ABS = { &EOR, &ABS, 4};
const Operation_t EOR_IDY = { &EOR, &IDY, 5};
const Operation_t EOR_ZPX = { &EOR, &ZPX, 4};
const Operation_t EOR_ABY = { &EOR, &ABY, 4};
const Operation_t EOR_ABX = { &EOR, &ABX, 4};

// ADC
const Operation_t ADC_IDX = { &ADC, &IDX, 6};
const Operation_t ADC_ZP  = { &ADC, &ZP, 3};
const Operation_t ADC_IMM = { &ADC, &IMM, 2};
const Operation_t ADC_ABS = { &ADC, &ABS, 4};
const Operation_t ADC_IDY = { &ADC, &IDY, 5};
const Operation_t ADC_ZPX = { &ADC, &ZPX, 4};
const Operation_t ADC_ABY = { &ADC, &ABY, 4};
const Operation_t ADC_ABX = { &ADC, &ABX, 4};

// STA
const Operation_t STA_IDX = { &STA, &IDX, 6};
const Operation_t STA_ZP  = { &STA, &ZP, 3};
const Operation_t STA_IMM = { &STA, &IMM, 2};
const Operation_t STA_ABS = { &STA, &ABS, 4};
const Operation_t STA_IDY = { &STA, &IDY, 5};
const Operation_t STA_ZPX = { &STA, &ZPX, 4};
const Operation_t STA_ABY = { &STA, &ABY, 4};
const Operation_t STA_ABX = { &STA, &ABX, 4};

// LDA
const Operation_t LDA_IDX = { &LDA, &IDX, 6};
const Operation_t LDA_ZP  = { &LDA, &ZP, 3};
const Operation_t LDA_IMM = { &LDA, &IMM, 2};
const Operation_t LDA_ABS = { &LDA, &ABS, 4};
const Operation_t LDA_IDY = { &LDA, &IDY, 5};
const Operation_t LDA_ZPX = { &LDA, &ZPX, 4};
const Operation_t LDA_ABY = { &LDA, &ABY, 4};
const Operation_t LDA_ABX = { &LDA, &ABX, 4};

// CMP
const Operation_t CMP_IDX = { &CMP, &IDX, 6};
const Operation_t CMP_ZP  = { &CMP, &ZP, 3};
const Operation_t CMP_IMM = { &CMP, &IMM, 2};
const Operation_t CMP_ABS = { &CMP, &ABS, 4};
const Operation_t CMP_IDY = { &CMP, &IDY, 5};
const Operation_t CMP_ZPX = { &CMP, &ZPX, 4};
const Operation_t CMP_ABY = { &CMP, &ABY, 4};
const Operation_t CMP_ABX = { &CMP, &ABX, 4};

// SBC
const Operation_t SBC_IDX = { &SBC, &IDX, 6};
const Operation_t SBC_ZP  = { &SBC, &ZP, 3};
const Operation_t SBC_IMM = { &SBC, &IMM, 2};
const Operation_t SBC_ABS = { &SBC, &ABS, 4};
const Operation_t SBC_IDY = { &SBC, &IDY, 5};
const Operation_t SBC_ZPX = { &SBC, &ZPX, 4};
const Operation_t SBC_ABY = { &SBC, &ABY, 4};
const Operation_t SBC_ABX = { &SBC, &ABX, 4};

/** Instruction Group 2
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
const Operation_t ASL_ZP  = { &ASL, &ZP, 5};
const Operation_t ASL_ACC = { &ASL, &ACC, 2};
const Operation_t ASL_ABS = { &ASL, &ABS, 6};
const Operation_t ASL_ZPX = { &ASL, &ZPX, 6};
const Operation_t ASL_ABX = { &ASL, &ABX, 7};

// ROL
const Operation_t ROL_ZP  = { &ROL, &ZP, 5};
const Operation_t ROL_ACC = { &ROL, &ACC, 2};
const Operation_t ROL_ABS = { &ROL, &ABS, 6};
const Operation_t ROL_ZPX = { &ROL, &ZPX, 6};
const Operation_t ROL_ABX = { &ROL, &ABX, 7};

// LSR
const Operation_t LSR_ZP  = { &LSR, &ZP, 5};
const Operation_t LSR_ACC = { &LSR, &ACC, 2};
const Operation_t LSR_ABS = { &LSR, &ABS, 6};
const Operation_t LSR_ZPX = { &LSR, &ZPX, 6};
const Operation_t LSR_ABX = { &LSR, &ABX, 7};

// ROR
const Operation_t ROR_ZP  = { &ROR, &ZP, 5};
const Operation_t ROR_ACC = { &ROR, &ACC, 2};
const Operation_t ROR_ABS = { &ROR, &ABS, 6};
const Operation_t ROR_ZPX = { &ROR, &ZPX, 6};
const Operation_t ROR_ABX = { &ROR, &ABX, 7};

// STX
const Operation_t STX_ZP  = { &STX, &ZP, 3};
const Operation_t STX_ABS = { &STX, &ABS, 4};
const Operation_t STX_ZPY = { &STX, &ZPY, 4};

// LDX
const Operation_t LDX_IMM = { &LDX, &IMM, 2};
const Operation_t LDX_ZP  = { &LDX, &ZP, 3};
const Operation_t LDX_ABS = { &LDX, &ABS, 4};
const Operation_t LDX_ZPY = { &LDX, &ZPY, 4};
const Operation_t LDX_ABY = { &LDX, &ABY, 4};

// DEC
const Operation_t DEC_ZP  = { &DEC, &ZP, 5};
const Operation_t DEC_ABS = { &DEC, &ABS, 6};
const Operation_t DEC_ZPX = { &DEC, &ZPX, 6};
const Operation_t DEC_ABX = { &DEC, &ABX, 7};

// INC
const Operation_t INC_ZP  = { &INC, &ZP, 5};
const Operation_t INC_ABS = { &INC, &ABS, 6};
const Operation_t INC_ZPX = { &INC, &ZPX, 6};
const Operation_t INC_ABX = { &INC, &ABX, 7};

/** Instruction Set Op Code Matrix
 *
 * This the whole instruction set of 6502 processor, an 1:1
 * copy of the manual, thank god there are only 256 ones
 */
const std::array<const Operation_t*, InstructionSetSize> OperationSet = {
/*             0         1         2     3         4         5         6     7         8         9         A     B         C         D         E     F */
/* 0 */ &BRK_IMP, &ORA_IDX,     Null, Null,     Null,  &ORA_ZP,  &ASL_ZP, Null, &PHP_IMP, &ORA_IMM, &ASL_ACC, Null,     Null, &ORA_ABS, &ASL_ABS, Null,
/* 1 */ &BPL_REL, &ORA_IDY,     Null, Null,     Null, &ORA_ZPX, &ASL_ZPX, Null, &CLC_IMP, &ORA_ABY,     Null, Null,     Null, &ORA_ABX, &ASL_ABX, Null,
/* 2 */ &JSR_ABS, &AND_IDX,     Null, Null,  &BIT_ZP,  &AND_ZP,  &ROL_ZP, Null, &PLP_IMP, &AND_IMM, &ROL_ACC, Null, &BIT_ABS, &AND_ABS, &ROL_ABS, Null,
/* 3 */ &BMI_REL, &AND_IDY,     Null, Null,     Null, &AND_ZPX, &ROL_ZPX, Null, &SEC_IMP, &AND_ABY,     Null, Null,     Null, &AND_ABX, &ROL_ABX, Null,
/* 4 */ &RTI_IMP, &EOR_IDX,     Null, Null,     Null,  &EOR_ZP,  &LSR_ZP, Null, &PHA_IMP, &EOR_IMM, &LSR_ACC, Null, &JMP_ABS, &EOR_ABS, &LSR_ABS, Null,
/* 5 */ &BVC_REL, &EOR_IDY,     Null, Null,     Null, &EOR_ZPX, &LSR_ZPX, Null, &CLI_IMP, &EOR_ABY,     Null, Null,     Null, &EOR_ABX, &LSR_ABX, Null,
/* 6 */ &RTS_IMP, &ADC_IDX,     Null, Null,     Null,  &ADC_ZP,  &ROR_ZP, Null, &PLA_IMP, &ADC_IMM, &ROR_ACC, Null, &JMP_IND, &ADC_ABS, &ROR_ABS, Null,
/* 7 */ &BVS_REL, &ADC_IDY,     Null, Null,     Null, &ADC_ZPX, &ROR_ZPX, Null, &SEI_IMP, &ADC_ABY,     Null, Null,     Null, &ADC_ABX, &ROR_ABX, Null,
/* 8 */     Null, &STA_IDX,     Null, Null,  &STY_ZP,  &STA_ZP,  &STX_ZP, Null, &DEY_IMP, &STA_IMM, &TXA_IMP, Null, &STY_ABS, &STA_ABS, &STX_ABS, Null,
/* 9 */ &BCC_REL, &STA_IDY,     Null, Null, &STY_ZPX, &STA_ZPX, &STX_ZPY, Null, &TYA_IMP, &STA_ABY, &TXS_IMP, Null,     Null, &STA_ABX,     Null, Null,
/* A */ &LDY_IMM, &LDA_IDX, &LDX_IMM, Null,  &LDY_ZP,  &LDA_ZP,  &LDX_ZP, Null, &TAY_IMP, &LDA_IMM, &TAX_IMP, Null, &LDY_ABS, &LDA_ABS, &LDX_ABS, Null,
/* B */ &BCS_REL, &LDA_IDY,     Null, Null, &LDY_ZPX, &LDA_ZPX, &LDX_ZPY, Null, &CLV_IMP, &LDA_ABY, &TSX_IMP, Null, &LDY_ABX, &LDA_ABX, &LDX_ABY, Null,
/* C */ &CPY_IMM, &CMP_IDX,     Null, Null,  &CPY_ZP,  &CMP_ZP,  &DEC_ZP, Null, &INY_IMP, &CMP_IMM, &DEX_IMP, Null, &CPY_ABS, &CMP_ABS, &DEC_ABS, Null,
/* D */ &BNE_REL, &CMP_IDY,     Null, Null,     Null, &CMP_ZPX, &DEC_ZPX, Null, &CLD_IMP, &CMP_ABY,     Null, Null,     Null, &CMP_ABX, &DEC_ABX, Null,
/* E */ &CPX_IMM, &SBC_IDX,     Null, Null,  &CPX_ZP,  &SBC_ZP,  &INC_ZP, Null, &INX_IMP, &SBC_IMM, &NOP_IMP, Null, &CPX_ABS, &SBC_ABS, &INC_ABS, Null,
/* F */ &BEQ_REL, &SBC_IDY,     Null, Null,     Null, &SBC_ZPX, &INC_ZPX, Null, &SED_IMP, &SBC_ABY,     Null, Null,     Null, &SBC_ABX, &INC_ABX, Null,
};

} // namespace cpu
} // namespace tones
