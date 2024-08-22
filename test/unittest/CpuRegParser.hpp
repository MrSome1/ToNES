
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "MicroProcessor.h"

#define REG_FLAG   "@REG"

using Registers_t = tones::MicroProcessor::Registers_t;

class CpuRegParser
{
public:

    CpuRegParser() : _counter(0) {}

    void load(const std::string &filepath);

    size_t size() { return _regs.size(); }

    const Registers_t *next()
    {
        return _counter < _regs.size() ? &(_regs[_counter++]) : nullptr;
    }

    const std::string *code()
    {
        return _counter < _codes.size() ? &(_codes[_counter]) : nullptr;
    }

    static bool parseRegs(const std::string &str, Registers_t &reg);

private:

    int _counter;

    std::vector<Registers_t> _regs;

    std::vector<std::string> _codes;
};

void CpuRegParser::load(const std::string &filepath)
{
    if (!filepath.size())
        return;

    std::ifstream file(filepath, std::ios_base::in);
    if (!file)
        return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        auto flag = line.find(REG_FLAG);
        if (std::string::npos == flag)
            continue;

        Registers_t reg;
        if (parseRegs(line.substr(flag), reg)) {
            _regs.emplace_back(reg);
            _codes.emplace_back(line.substr(0, flag));
        }
    }
}

bool CpuRegParser::parseRegs(const std::string &str, Registers_t &reg)
{
    char p[8] = { 0 };

    int32_t ret = sscanf(str.c_str(), REG_FLAG " A=0x%hhx X=0x%hhx Y=0x%hhx S=0x%hhx P=0b%8c PC=0x%hx",
                         &(reg.A), &(reg.X), &(reg.Y), &(reg.S), p, &(reg.PC));

    reg.P = stoi(std::string(p, 8), nullptr, 2);

    return ret == 6;
}
