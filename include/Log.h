#ifndef _TONES_LOG_H_
#define _TONES_LOG_H_

#include <iostream>
#include <string>

#ifndef __FILE_NAME__
#define __FILE_NAME__ __BASE_FILE__ // __FILE__, __builtini_FILE(), __BASE_FILE__
#endif

// TODO: Do not compile if mismatch current level
#define LOG_DEBUG() \
    tones::log::Logger(tones::log::DEBUG, __FILE_NAME__, __LINE__).stream()

#define LOG_INFO() \
    tones::log::Logger(tones::log::INFO, __FILE_NAME__, __LINE__).stream()

#define LOG_WARN() \
    tones::log::Logger(tones::log::WARN, __FILE_NAME__, __LINE__).stream()

#define LOG_ERROR() \
    tones::log::Logger(tones::log::ERROR, __FILE_NAME__, __LINE__).stream()

#define LOG_FATAL() \
    tones::log::Logger(tones::log::FATAL, __FILE_NAME__, __LINE__).stream()

namespace tones {
namespace log {

enum Severity {
    NONE = -1,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    ALL
};

//! Set the log level
void setSeverity(Severity s);

//! Redirect the log to a file
void redirect(std::FILE* file, Severity severity = Severity::INFO);

class Logger
{
public:

    Logger(Severity severity, const char* file, int line);
    ~Logger();

    std::ostream &stream();

protected:

    void prefix(Severity severity, const char* file, int line);

    static const char *timestamp();

    std::ostream &_stream;
};

} // namespace log
} // namespace tones

#endif // _TONES_LOG_H_