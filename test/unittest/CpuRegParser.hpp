
#include <iostream>
#include <fstream>
#include <vector>

#include "MicroProcessor.h"

using Registers_t = tones::MicroProcessor::Registers_t;

const char *REG_FLAG = "@REG";
const int REG_FLAG_LEN = strlen(REG_FLAG);

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

private:

    int _counter;

    std::vector<Registers_t> _regs;
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
        if (!line.size())
            continue;

        auto flag = line.find(REG_FLAG);
        if (std::string::npos == flag)
            continue;

        Registers_t reg;

        // TODO: parse reg

        _regs.emplace_back(reg);
    }
}
