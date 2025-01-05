
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include "MicroProcessor.h"

#define REG_FLAG "@REG"
#define RAM_FLAG "@RAM"

using Registers_t = tones::MicroProcessor::Registers_t;

/**
 * @brief Parse the ground truth from a test rom
 */
class RomParser
{
public:

    struct Line {
        int num;
        std::string code;
        Registers_t regs;
        int ramAddr;
        int ramValue;

        Line() : num(0), ramAddr(-1) {}

        Line(int n, const std::string &s) : num(n), code(s), ramAddr(-1) {}
    };

    RomParser() : _line(_lines.end()) {}

    void load(const std::string &filepath);

    void next() { ++_line; }

    const size_t size() const { return _lines.size(); }

    const Line &line() const { return _line != _lines.end() ? *_line : null; }

    static bool hasRam(const Line &l) { return l.ramAddr >= 0 && l.ramAddr <= 0xffff; }

    static bool parseReg(const std::string &str, Registers_t &reg);

    static void parseRam(const std::string &str, int &ramAddr, int &ramValue);

private:

    std::list<Line> _lines;

    std::list<Line>::iterator _line;
    
    static const Line null;
};

const RomParser::Line RomParser::null;

void RomParser::load(const std::string &filepath)
{
    if (!filepath.size())
        return;

    std::ifstream file(filepath, std::ios_base::in);
    if (!file)
        return;

    int num = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++num;

        auto flag = line.find(REG_FLAG);
        if (std::string::npos == flag)
            continue;

        Line l(num, line.substr(0, flag));
        parseReg(line.substr(flag), l.regs);

        flag = line.find(RAM_FLAG);
        if (std::string::npos != flag) {
            parseRam(line.substr(flag), l.ramAddr, l.ramValue);
        }

        _lines.emplace_back(l);
    }

    _line = _lines.begin();
}

bool RomParser::parseReg(const std::string &str, Registers_t &reg)
{
    char p[8] = { 0 };

    int32_t ret = sscanf(str.c_str(), REG_FLAG " A=0x%hhx X=0x%hhx Y=0x%hhx S=0x%hhx P=0b%8c PC=0x%hx",
                         &(reg.A), &(reg.X), &(reg.Y), &(reg.S), p, &(reg.PC));

    reg.P = stoi(std::string(p, 8), nullptr, 2);

    return ret == 6;
}

void RomParser::parseRam(const std::string &str, int &ramAddr, int &ramValue)
{
    uint16_t addr;
    uint8_t value;
    if (2 == sscanf(str.c_str(), RAM_FLAG " 0x%hx 0x%hhx", &addr, &value)) {
        ramAddr = addr;
        ramValue = value;
    }
}
