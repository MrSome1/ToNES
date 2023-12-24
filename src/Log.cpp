
#include <array>

#include "Log.h"

namespace tones {
namespace log {

static Severity severity = Severity::INFO;

static std::array<std::ostream*, ALL> Sinks = {
    &std::cerr,  // FATAL
    &std::cerr,  // ERROR
    &std::cout,  // WARN
    &std::cout,  // INFO
    &std::cout,  // DEBUG
};

static const char *Patterns[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
};

void setSeverity(Severity s)
{
    if (s >= Severity::FATAL && s <= Severity::DEBUG)
        severity = s;
}

void redirect(std::FILE* file, Severity severity)
{
    /* TODO */
}

/* Logger */

Logger::Logger( Severity severity, const char* file, int line)
    : _stream(*Sinks[severity])
{
    prefix(severity, file, line);
}

Logger::~Logger()
{
    _stream << std::endl;
}

std::ostream &Logger::stream()
{
    return _stream;
}

const char *Logger::timestamp()
{
    static char time[20] = { 0 };
    /* TODO */
    return time;
}

void Logger::prefix(Severity severity, const char* file, int line)
{
    _stream << timestamp()
            << " [" << Patterns[severity] << "] "
            << file << '@' << line << ": "; 
}

} // namespace log
} // namespace tones