// Copyright (C) 2012-2013 Fengyu Gao (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "log.h"

Log::LogLevel Log::m_level = Log::LOG_LEVEL_WARN;
qint32 Log::m_errorCount = 0;
QMutex Log::m_mutex;
LogRedirectCallback Log::m_callback = NULL;

void Log::enable(enum LogLevel level)
{
    if( level == LOG_LEVEL_ERROR || 
        level == LOG_LEVEL_WARN ||
        level == LOG_LEVEL_INFO ||
        level == LOG_LEVEL_DEBUG )
    {
        m_level = level;
    }
    else
    {
        printf("Warning: Invalid log level, default level INFO used.\n");
        m_level = LOG_LEVEL_INFO;
    }
}

void Log::enableRedirect(LogRedirectCallback callback)
{
    m_callback = callback;
}

void Log::print(const char *content)
{
    if( m_callback == NULL )
    {
        printf("%s", content);
    }
    else
    {
        m_callback(content);
    }
}

void Log::addLine(enum LogLevel level, const QString &str)
{
    Log::addLine(level, "%s", str.toLocal8Bit().data());
}

void Log::addLine(enum LogLevel level, const char *format, ...)
{
    char buffer[1024];
    m_mutex.lock();
    
    if( static_cast<int>(m_level) & static_cast<int>(level))
    {
        // Print log level
        if( level & ~LOG_PREFIX_NOLEVEL )
        {
            switch( level )
            {
            case LOG_ERROR:
                print("Error: ");
                m_errorCount += 1;
                break;
            case LOG_WARN:
                print("Warning: ");
                break;
            case LOG_INFO:
                print("Info: ");
                break;
            case LOG_DEBUG:
                print("Debug: ");
                break;
            default:
                break;
            }
        }
        
        // Print remaining message
        va_list argList;
        va_start(argList, format);
        
        vsprintf(buffer, format, argList);
        
        if( level & ~LOG_PREFIX_NONEWLINE )
        {
            strcat(buffer, "\n");
        }
        print(buffer);
        
        va_end(argList);
    }
    // otherwise output with specific level is disabled
    m_mutex.unlock();
}

qint32 Log::getErrorCount()
{
    return m_errorCount;
}

