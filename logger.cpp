#include "logger.h"

#include "stdarg.h"
#include "stdio.h"

namespace dtest {

Logger* Logger::m_instance = NULL;

#define LOGGER_BUFFER 256

Logger::Logger(bool console, bool timestamp, const char* file)
    : m_console(console),
      m_timestamp(timestamp),
      m_file(NULL)
{
    m_file = fopen(file, "wb");
    if ( !m_file )
    {
        fclose(m_file);
    }
}

Logger::~Logger()
{

}

Logger* Logger::create_instance(bool console, bool timestamp, const char* file)
{
    if ( !m_instance )
    {
        m_instance = new Logger(console, timestamp, file);
    }

    return m_instance;
}

Logger* Logger::instance()
{
    return m_instance;
}


void Logger::log(const char* fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    char buffer[LOGGER_BUFFER] = {0};

    int count = vsnprintf(buffer, LOGGER_BUFFER, fmt, vargs);

    if ( count < 0 )
    {
        return;
    }

    if ( m_console )
    {
        printf(buffer);
    }

    fprintf(m_file, buffer);
    fflush(m_file);

    va_end(vargs);
}

} // namespace dtest
