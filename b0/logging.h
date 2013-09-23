#pragma once

#include "threading.h"

#define Log_debug(msg, ...) ::b0::Log::debug(__LINE__, __FILE__, msg, ## __VA_ARGS__)
#define Log_info(msg, ...) ::b0::Log::info(__LINE__, __FILE__, msg, ## __VA_ARGS__)
#define Log_warn(msg, ...) ::b0::Log::warn(__LINE__, __FILE__, msg, ## __VA_ARGS__)
#define Log_error(msg, ...) ::b0::Log::error(__LINE__, __FILE__, msg, ## __VA_ARGS__)
#define Log_fatal(msg, ...) ::b0::Log::fatal(__LINE__, __FILE__, msg, ## __VA_ARGS__)

namespace b0 {

class Log {
    static int level_s;
    static FILE* fp_s;
    static Mutex m_s;

    static void log_v(int level, int line, const char* file, const char* fmt, va_list args);
public:

    enum {
        FATAL = 0, ERROR = 1, WARN = 2, INFO = 3, DEBUG = 4
    };

    static void set_file(FILE* fp);
    static void set_level(int level);

    static void log(int level, int line, const char* file, const char* fmt, ...);

    static void fatal(int line, const char* file, const char* fmt, ...);
    static void error(int line, const char* file, const char* fmt, ...);
    static void warn(int line, const char* file, const char* fmt, ...);
    static void info(int line, const char* file, const char* fmt, ...);
    static void debug(int line, const char* file, const char* fmt, ...);

    static void fatal(const char* fmt, ...);
    static void error(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void debug(const char* fmt, ...);
};

} // namespace b0
