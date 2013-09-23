#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

#include "threading.h"
#include "logging.h"

namespace b0 {

int Log::level_s = Log::DEBUG;
FILE* Log::fp_s = stdout;
Mutex Log::m_s;

void Log::set_level(int level) {
    ScopedLock sl(&m_s);
    level_s = level;
}

void Log::set_file(FILE* fp) {
    verify(fp != NULL);
    ScopedLock sl(&m_s);
    fp_s = fp;
}

static const char* basename(const char* fpath) {
    if (fpath == nullptr) {
        return nullptr;
    }

    const char sep = '/';
    int len = strlen(fpath);
    int idx = len - 1;
    while (idx > 0) {
        if (fpath[idx - 1] == sep) {
            break;
        }
        idx--;
    }
    verify(idx >= 0 && idx < len);
    return &fpath[idx];
}

void Log::log_v(int level, int line, const char* file, const char* fmt, va_list args) {
    static char indicator[] = { 'F', 'E', 'W', 'I', 'D' };
    assert(level <= Log::DEBUG);
    if (level <= level_s) {
        const char* filebase = basename(file);

        const int tm_str_len = 80;
        char tm_str[tm_str_len];
        time_t now = time(NULL);
        struct tm tm_val;
        localtime_r(&now, &tm_val);
        strftime(tm_str, tm_str_len - 1, "%F %T", &tm_val);
        timeval tv;
        gettimeofday(&tv, NULL);

        m_s.lock();
        fprintf(fp_s, "%c ", indicator[level]);
        if (filebase != nullptr) {
            fprintf(fp_s, "[%s:%d] ", filebase, line);
        }

        fprintf(fp_s, "%s.%03d | ", tm_str, tv.tv_usec / 1000);
        vfprintf(fp_s, fmt, args);
        fprintf(fp_s, "\n");
        fflush(fp_s);
        m_s.unlock();
    }
}

void Log::log(int level, int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(level, line, file, fmt, args);
    va_end(args);
}

void Log::fatal(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::FATAL, line, file, fmt, args);
    va_end(args);
    abort();
}

void Log::error(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::ERROR, line, file, fmt, args);
    va_end(args);
}

void Log::warn(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::WARN, line, file, fmt, args);
    va_end(args);
}

void Log::info(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::INFO, line, file, fmt, args);
    va_end(args);
}

void Log::debug(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::DEBUG, line, file, fmt, args);
    va_end(args);
}


void Log::fatal(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::FATAL, 0, nullptr, fmt, args);
    va_end(args);
    abort();
}

void Log::error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::ERROR, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::WARN, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::INFO, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::DEBUG, 0, nullptr, fmt, args);
    va_end(args);
}

} // namespace b0