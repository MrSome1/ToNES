
#include "Instruction.h"
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

/* Addressing Modes */

const AddressingMode_t IMP = { &MicroProcessor::fetchNull,             code::Implied,          0, "IMP" };
const AddressingMode_t ACC = { &MicroProcessor::fetchNull,             code::Accumulator,      0, "ACC" };
const AddressingMode_t IMM = { &MicroProcessor::fetchImmediate,        code::Immediate,        1, "IMM" };
const AddressingMode_t ABS = { &MicroProcessor::fetchAbsolute,         code::Absolute,         2, "ABS" };
const AddressingMode_t ZP  = { &MicroProcessor::fetchZeroPage,         code::ZeroPage,         1,  "ZP" };
const AddressingMode_t ZPX = { &MicroProcessor::fetchIndexedZeroPageX, code::IndexedZeroPageX, 1, "ZPX" };
const AddressingMode_t ZPY = { &MicroProcessor::fetchIndexedZeroPageY, code::IndexedZeroPageY, 1, "ZPY" };
const AddressingMode_t ABX = { &MicroProcessor::fetchIndexedAbsoluteX, code::IndexedAbsoluteX, 2, "ABX" };
const AddressingMode_t ABY = { &MicroProcessor::fetchIndexedAbsoluteY, code::IndexedAbsoluteY, 2, "ABY" };
const AddressingMode_t REL = { &MicroProcessor::fetchRelative,         code::Relative,         1, "REL" };
const AddressingMode_t IDX = { &MicroProcessor::fetchIndexedIndirect,  code::IndexedIndirect,  1, "IDX" };
const AddressingMode_t IDY = { &MicroProcessor::fetchIndirectIndexed,  code::IndirectIndexed,  1, "IDY" };
const AddressingMode_t IND = { &MicroProcessor::fetchAbsoluteIndirect, code::AbsoluteIndirect, 2, "IND" };

const AddressingMode_t INVALID = { &MicroProcessor::fetchNull, code::Implied, 0, "NIL" };

/* Instructions */

const Instruction_t ADC = { &InstructionDecoder::ADC, code::ADC, "ADC" };
const Instruction_t AND = { &InstructionDecoder::AND, code::AND, "AND" };
const Instruction_t ASL = { &InstructionDecoder::ASL, code::ASL, "ASL" };

const Instruction_t BCC = { &InstructionDecoder::BCC, code::BCC, "BCC" };
const Instruction_t BCS = { &InstructionDecoder::BCS, code::BCS, "BCS" };
const Instruction_t BEQ = { &InstructionDecoder::BEQ, code::BEQ, "BEQ" };
const Instruction_t BIT = { &InstructionDecoder::BIT, code::BIT, "BIT" };
const Instruction_t BMI = { &InstructionDecoder::BMI, code::BMI, "BMI" };
const Instruction_t BNE = { &InstructionDecoder::BNE, code::BNE, "BNE" };
const Instruction_t BPL = { &InstructionDecoder::BPL, code::BPL, "BPL" };
const Instruction_t BRK = { &InstructionDecoder::BRK, code::BRK, "BRK" };
const Instruction_t BVC = { &InstructionDecoder::BVC, code::BVC, "BVC" };
const Instruction_t BVS = { &InstructionDecoder::BVS, code::BVS, "BVS" };

const Instruction_t CLC = { &InstructionDecoder::CLC, code::CLC, "CLC" };
const Instruction_t CLD = { &InstructionDecoder::CLD, code::CLD, "CLD" };
const Instruction_t CLI = { &InstructionDecoder::CLI, code::CLI, "CLI" };
const Instruction_t CLV = { &InstructionDecoder::CLV, code::CLV, "CLV" };
const Instruction_t CMP = { &InstructionDecoder::CMP, code::CMP, "CMP" };
const Instruction_t CPX = { &InstructionDecoder::CPX, code::CPX, "CPX" };
const Instruction_t CPY = { &InstructionDecoder::CPY, code::CPY, "CPY" };

const Instruction_t DEC = { &InstructionDecoder::DEC, code::DEC, "DEC" };
const Instruction_t DEX = { &InstructionDecoder::DEX, code::DEX, "DEX" };
const Instruction_t DEY = { &InstructionDecoder::DEY, code::DEY, "DEY" };

const Instruction_t EOR = { &InstructionDecoder::EOR, code::EOR, "EOR" };

const Instruction_t INC = { &InstructionDecoder::INC, code::INC, "INC" };
const Instruction_t INX = { &InstructionDecoder::INX, code::INX, "INX" };
const Instruction_t INY = { &InstructionDecoder::INY, code::INY, "INY" };

const Instruction_t JMP = { &InstructionDecoder::JMP, code::JMP, "JMP" };
const Instruction_t JSR = { &InstructionDecoder::JSR, code::JSR, "JSR" };

const Instruction_t LDA = { &InstructionDecoder::LDA, code::LDA, "LDA" };
const Instruction_t LDX = { &InstructionDecoder::LDX, code::LDX, "LDX" };
const Instruction_t LDY = { &InstructionDecoder::LDY, code::LDY, "LDY" };
const Instruction_t LSR = { &InstructionDecoder::LSR, code::LSR, "LSR" };

const Instruction_t NOP = { &InstructionDecoder::NOP, code::NOP, "NOP" };

const Instruction_t ORA = { &InstructionDecoder::ORA, code::ORA, "ORA" };

const Instruction_t PHA = { &InstructionDecoder::PHA, code::PHA, "PHA" };
const Instruction_t PHP = { &InstructionDecoder::PHP, code::PHP, "PHP" };
const Instruction_t PLA = { &InstructionDecoder::PLA, code::PLA, "PLA" };
const Instruction_t PLP = { &InstructionDecoder::PLP, code::PLP, "PLP" };

const Instruction_t ROL = { &InstructionDecoder::ROL, code::ROL, "ROL" };
const Instruction_t ROR = { &InstructionDecoder::ROR, code::ROR, "ROR" };
const Instruction_t RTI = { &InstructionDecoder::RTI, code::RTI, "RTI" };
const Instruction_t RTS = { &InstructionDecoder::RTS, code::RTS, "RTS" };

const Instruction_t SBC = { &InstructionDecoder::SBC, code::SBC, "SBC" };
const Instruction_t SEC = { &InstructionDecoder::SEC, code::SEC, "SEC" };
const Instruction_t SED = { &InstructionDecoder::SED, code::SED, "SED" };
const Instruction_t SEI = { &InstructionDecoder::SEI, code::SEI, "SEI" };
const Instruction_t STA = { &InstructionDecoder::STA, code::STA, "STA" };
const Instruction_t STX = { &InstructionDecoder::STX, code::STX, "STX" };
const Instruction_t STY = { &InstructionDecoder::STY, code::STY, "STY" };

const Instruction_t TAX = { &InstructionDecoder::TAX, code::TAX, "TAX" };
const Instruction_t TAY = { &InstructionDecoder::TAY, code::TAY, "TAY" };
const Instruction_t TSX = { &InstructionDecoder::TSX, code::TSX, "TSX" };
const Instruction_t TXA = { &InstructionDecoder::TXA, code::TXA, "TXA" };
const Instruction_t TXS = { &InstructionDecoder::TXS, code::TXS, "TXS" };
const Instruction_t TYA = { &InstructionDecoder::TYA, code::TYA, "TYA" };

const Instruction_t UNKNOWN = { &InstructionDecoder::NOP, code::NOP, "NIL"};

/* Operations */

const Operation_t UnknownOperation = { &UNKNOWN, &INVALID, 0 };
const Operation_t *NIL = &UnknownOperation;

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
const Operation_t RTI_IMP = { &RTI, &IMP, 6};

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
const Operation_t BIT_ZP  = { &BIT, &ZP , 3};
const Operation_t BIT_ABS = { &BIT, &ABS, 4};

// STY
const Operation_t STY_ZP  = { &STY, &ZP , 3};
const Operation_t STY_ABS = { &STY, &ABS, 4};
const Operation_t STY_ZPX = { &STY, &ZPX, 4};

// LDY
const Operation_t LDY_IMM = { &LDY, &IMM, 2};
const Operation_t LDY_ZP  = { &LDY, &ZP , 3};
const Operation_t LDY_ABS = { &LDY, &ABS, 4};
const Operation_t LDY_ZPX = { &LDY, &ZPX, 4};
const Operation_t LDY_ABX = { &LDY, &ABX, 4};

// CPY
const Operation_t CPY_IMM = { &CPY, &IMM, 2};
const Operation_t CPY_ZP  = { &CPY, &ZP , 3};
const Operation_t CPY_ABS = { &CPY, &ABS, 4};

// CPX
const Operation_t CPX_IMM = { &CPX, &IMM, 2};
const Operation_t CPX_ZP  = { &CPX, &ZP , 3};
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
const Operation_t ORA_ZP  = { &ORA, &ZP , 3};
const Operation_t ORA_IMM = { &ORA, &IMM, 2};
const Operation_t ORA_ABS = { &ORA, &ABS, 4};
const Operation_t ORA_IDY = { &ORA, &IDY, 5};
const Operation_t ORA_ZPX = { &ORA, &ZPX, 4};
const Operation_t ORA_ABY = { &ORA, &ABY, 4};
const Operation_t ORA_ABX = { &ORA, &ABX, 4};

// AND
const Operation_t AND_IDX = { &AND, &IDX, 6};
const Operation_t AND_ZP  = { &AND, &ZP , 3};
const Operation_t AND_IMM = { &AND, &IMM, 2};
const Operation_t AND_ABS = { &AND, &ABS, 4};
const Operation_t AND_IDY = { &AND, &IDY, 5};
const Operation_t AND_ZPX = { &AND, &ZPX, 4};
const Operation_t AND_ABY = { &AND, &ABY, 4};
const Operation_t AND_ABX = { &AND, &ABX, 4};

// EOR
const Operation_t EOR_IDX = { &EOR, &IDX, 6};
const Operation_t EOR_ZP  = { &EOR, &ZP , 3};
const Operation_t EOR_IMM = { &EOR, &IMM, 2};
const Operation_t EOR_ABS = { &EOR, &ABS, 4};
const Operation_t EOR_IDY = { &EOR, &IDY, 5};
const Operation_t EOR_ZPX = { &EOR, &ZPX, 4};
const Operation_t EOR_ABY = { &EOR, &ABY, 4};
const Operation_t EOR_ABX = { &EOR, &ABX, 4};

// ADC
const Operation_t ADC_IDX = { &ADC, &IDX, 6};
const Operation_t ADC_ZP  = { &ADC, &ZP , 3};
const Operation_t ADC_IMM = { &ADC, &IMM, 2};
const Operation_t ADC_ABS = { &ADC, &ABS, 4};
const Operation_t ADC_IDY = { &ADC, &IDY, 5};
const Operation_t ADC_ZPX = { &ADC, &ZPX, 4};
const Operation_t ADC_ABY = { &ADC, &ABY, 4};
const Operation_t ADC_ABX = { &ADC, &ABX, 4};

// STA
const Operation_t STA_IDX = { &STA, &IDX, 6};
const Operation_t STA_ZP  = { &STA, &ZP , 3};
const Operation_t STA_IMM = { &STA, &IMM, 2};
const Operation_t STA_ABS = { &STA, &ABS, 4};
const Operation_t STA_IDY = { &STA, &IDY, 5};
const Operation_t STA_ZPX = { &STA, &ZPX, 4};
const Operation_t STA_ABY = { &STA, &ABY, 4};
const Operation_t STA_ABX = { &STA, &ABX, 4};

// LDA
const Operation_t LDA_IDX = { &LDA, &IDX, 6};
const Operation_t LDA_ZP  = { &LDA, &ZP , 3};
const Operation_t LDA_IMM = { &LDA, &IMM, 2};
const Operation_t LDA_ABS = { &LDA, &ABS, 4};
const Operation_t LDA_IDY = { &LDA, &IDY, 5};
const Operation_t LDA_ZPX = { &LDA, &ZPX, 4};
const Operation_t LDA_ABY = { &LDA, &ABY, 4};
const Operation_t LDA_ABX = { &LDA, &ABX, 4};

// CMP
const Operation_t CMP_IDX = { &CMP, &IDX, 6};
const Operation_t CMP_ZP  = { &CMP, &ZP , 3};
const Operation_t CMP_IMM = { &CMP, &IMM, 2};
const Operation_t CMP_ABS = { &CMP, &ABS, 4};
const Operation_t CMP_IDY = { &CMP, &IDY, 5};
const Operation_t CMP_ZPX = { &CMP, &ZPX, 4};
const Operation_t CMP_ABY = { &CMP, &ABY, 4};
const Operation_t CMP_ABX = { &CMP, &ABX, 4};

// SBC
const Operation_t SBC_IDX = { &SBC, &IDX, 6};
const Operation_t SBC_ZP  = { &SBC, &ZP , 3};
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
const Operation_t ASL_ZP  = { &ASL, &ZP , 5};
const Operation_t ASL_ACC = { &ASL, &ACC, 2};
const Operation_t ASL_ABS = { &ASL, &ABS, 6};
const Operation_t ASL_ZPX = { &ASL, &ZPX, 6};
const Operation_t ASL_ABX = { &ASL, &ABX, 7};

// ROL
const Operation_t ROL_ZP  = { &ROL, &ZP , 5};
const Operation_t ROL_ACC = { &ROL, &ACC, 2};
const Operation_t ROL_ABS = { &ROL, &ABS, 6};
const Operation_t ROL_ZPX = { &ROL, &ZPX, 6};
const Operation_t ROL_ABX = { &ROL, &ABX, 7};

// LSR
const Operation_t LSR_ZP  = { &LSR, &ZP , 5};
const Operation_t LSR_ACC = { &LSR, &ACC, 2};
const Operation_t LSR_ABS = { &LSR, &ABS, 6};
const Operation_t LSR_ZPX = { &LSR, &ZPX, 6};
const Operation_t LSR_ABX = { &LSR, &ABX, 7};

// ROR
const Operation_t ROR_ZP  = { &ROR, &ZP , 5};
const Operation_t ROR_ACC = { &ROR, &ACC, 2};
const Operation_t ROR_ABS = { &ROR, &ABS, 6};
const Operation_t ROR_ZPX = { &ROR, &ZPX, 6};
const Operation_t ROR_ABX = { &ROR, &ABX, 7};

// STX
const Operation_t STX_ZP  = { &STX, &ZP , 3};
const Operation_t STX_ABS = { &STX, &ABS, 4};
const Operation_t STX_ZPY = { &STX, &ZPY, 4};

// LDX
const Operation_t LDX_IMM = { &LDX, &IMM, 2};
const Operation_t LDX_ZP  = { &LDX, &ZP , 3};
const Operation_t LDX_ABS = { &LDX, &ABS, 4};
const Operation_t LDX_ZPY = { &LDX, &ZPY, 4};
const Operation_t LDX_ABY = { &LDX, &ABY, 4};

// DEC
const Operation_t DEC_ZP  = { &DEC, &ZP , 5};
const Operation_t DEC_ABS = { &DEC, &ABS, 6};
const Operation_t DEC_ZPX = { &DEC, &ZPX, 6};
const Operation_t DEC_ABX = { &DEC, &ABX, 7};

// INC
const Operation_t INC_ZP  = { &INC, &ZP , 5};
const Operation_t INC_ABS = { &INC, &ABS, 6};
const Operation_t INC_ZPX = { &INC, &ZPX, 6};
const Operation_t INC_ABX = { &INC, &ABX, 7};

/** Instruction Set Op Code Matrix
 *
 * This the whole instruction set of 6502 processor, an 1:1
 * copy of the manual, thank god there are only 256 ones
 */
const Operation_t* const OperationSet[OperationSetSize] = {
/*             0         1         2    3         4         5         6    7         8         9         A    B         C         D         E    F */
/* 0 */ &BRK_IMP, &ORA_IDX,      NIL, NIL,      NIL, &ORA_ZP , &ASL_ZP , NIL, &PHP_IMP, &ORA_IMM, &ASL_ACC, NIL,      NIL, &ORA_ABS, &ASL_ABS, NIL,
/* 1 */ &BPL_REL, &ORA_IDY,      NIL, NIL,      NIL, &ORA_ZPX, &ASL_ZPX, NIL, &CLC_IMP, &ORA_ABY,      NIL, NIL,      NIL, &ORA_ABX, &ASL_ABX, NIL,
/* 2 */ &JSR_ABS, &AND_IDX,      NIL, NIL, &BIT_ZP , &AND_ZP , &ROL_ZP , NIL, &PLP_IMP, &AND_IMM, &ROL_ACC, NIL, &BIT_ABS, &AND_ABS, &ROL_ABS, NIL,
/* 3 */ &BMI_REL, &AND_IDY,      NIL, NIL,      NIL, &AND_ZPX, &ROL_ZPX, NIL, &SEC_IMP, &AND_ABY,      NIL, NIL,      NIL, &AND_ABX, &ROL_ABX, NIL,
/* 4 */ &RTI_IMP, &EOR_IDX,      NIL, NIL,      NIL, &EOR_ZP , &LSR_ZP , NIL, &PHA_IMP, &EOR_IMM, &LSR_ACC, NIL, &JMP_ABS, &EOR_ABS, &LSR_ABS, NIL,
/* 5 */ &BVC_REL, &EOR_IDY,      NIL, NIL,      NIL, &EOR_ZPX, &LSR_ZPX, NIL, &CLI_IMP, &EOR_ABY,      NIL, NIL,      NIL, &EOR_ABX, &LSR_ABX, NIL,
/* 6 */ &RTS_IMP, &ADC_IDX,      NIL, NIL,      NIL, &ADC_ZP , &ROR_ZP , NIL, &PLA_IMP, &ADC_IMM, &ROR_ACC, NIL, &JMP_IND, &ADC_ABS, &ROR_ABS, NIL,
/* 7 */ &BVS_REL, &ADC_IDY,      NIL, NIL,      NIL, &ADC_ZPX, &ROR_ZPX, NIL, &SEI_IMP, &ADC_ABY,      NIL, NIL,      NIL, &ADC_ABX, &ROR_ABX, NIL,
/* 8 */      NIL, &STA_IDX,      NIL, NIL, &STY_ZP , &STA_ZP , &STX_ZP , NIL, &DEY_IMP, &STA_IMM, &TXA_IMP, NIL, &STY_ABS, &STA_ABS, &STX_ABS, NIL,
/* 9 */ &BCC_REL, &STA_IDY,      NIL, NIL, &STY_ZPX, &STA_ZPX, &STX_ZPY, NIL, &TYA_IMP, &STA_ABY, &TXS_IMP, NIL,      NIL, &STA_ABX,      NIL, NIL,
/* A */ &LDY_IMM, &LDA_IDX, &LDX_IMM, NIL, &LDY_ZP , &LDA_ZP , &LDX_ZP , NIL, &TAY_IMP, &LDA_IMM, &TAX_IMP, NIL, &LDY_ABS, &LDA_ABS, &LDX_ABS, NIL,
/* B */ &BCS_REL, &LDA_IDY,      NIL, NIL, &LDY_ZPX, &LDA_ZPX, &LDX_ZPY, NIL, &CLV_IMP, &LDA_ABY, &TSX_IMP, NIL, &LDY_ABX, &LDA_ABX, &LDX_ABY, NIL,
/* C */ &CPY_IMM, &CMP_IDX,      NIL, NIL, &CPY_ZP , &CMP_ZP , &DEC_ZP , NIL, &INY_IMP, &CMP_IMM, &DEX_IMP, NIL, &CPY_ABS, &CMP_ABS, &DEC_ABS, NIL,
/* D */ &BNE_REL, &CMP_IDY,      NIL, NIL,      NIL, &CMP_ZPX, &DEC_ZPX, NIL, &CLD_IMP, &CMP_ABY,      NIL, NIL,      NIL, &CMP_ABX, &DEC_ABX, NIL,
/* E */ &CPX_IMM, &SBC_IDX,      NIL, NIL, &CPX_ZP , &SBC_ZP , &INC_ZP , NIL, &INX_IMP, &SBC_IMM, &NOP_IMP, NIL, &CPX_ABS, &SBC_ABS, &INC_ABS, NIL,
/* F */ &BEQ_REL, &SBC_IDY,      NIL, NIL,      NIL, &SBC_ZPX, &INC_ZPX, NIL, &SED_IMP, &SBC_ABY,      NIL, NIL,      NIL, &SBC_ABX, &INC_ABX, NIL,
};

} // namespace cpu
} // namespace tones
