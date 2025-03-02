
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include "MicroProcessor.h"

#define LOG_REG_FLAG "A:"

using Registers_t = tones::MicroProcessor::Registers_t;

/**
 * @brief Parse the ground truth from a log file
 */
class LogParser
{
public:

    void load(const std::string &filepath);

    void next(Registers_t &reg);

    static bool parseReg(const std::string &str, Registers_t &reg);

private:

    std::list<Registers_t> _regs;
};

void LogParser::next(Registers_t &reg)
{ 
    if (!_regs.empty()) {
        reg = _regs.front();
        _regs.pop_front();
    }
}

void LogParser::load(const std::string &filepath)
{
    if (!filepath.size())
        return;

    std::ifstream file(filepath, std::ios_base::in);
    if (!file)
        return;

    std::string line;
    Registers_t reg;

    _regs.clear();
    while (std::getline(file, line)) {

        // if (std::string::npos == line.find(REG_FLAG))
        if (0 != line.find(LOG_REG_FLAG))
            continue;

        parseReg(line, reg);

        _regs.emplace_back(reg);
    }
}

bool LogParser::parseReg(const std::string &str, Registers_t &reg)
{
    char p[8] = { 0 };
    char s[128] = { 0 };

    int32_t ret = sscanf(str.c_str(), "A:%hhx X:%hhx Y:%hhx S:%hhx P:%8c   $%hx: %s",
                         &(reg.A), &(reg.X), &(reg.Y), &(reg.S), p, &(reg.PC), s);

    uint16_t reg_P = 0;
    for (int i = 0; i < 8; ++i) {
        int bit = p[i] >= 'A' && p[i] <= 'Z' ? 1 : 0;
        reg_P |= bit;
        reg_P <<= 1;
    }
    reg.P = (reg_P >> 1 | 0x20) & 0xff; // the unused bit is set to 0 in FCEUX

    return ret == 7;
}
