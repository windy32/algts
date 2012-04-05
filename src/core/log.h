// Copyright (C) 2012 Gao Fengyu (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
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

#ifndef LOG_H
#define LOG_H

#include "common.h"

class Log
{
public:
    enum LogLevel
    {
        LOG_LEVEL_NONE     = 0x00000000, // no logging

        LOG_ERROR          = 0x00000001, // serious error messages only
        LOG_LEVEL_ERROR    = 0x00000001,

        LOG_WARN           = 0x00000002, // warning messages
        LOG_LEVEL_WARN     = 0x00000003,

        LOG_INFO           = 0x00000004, // informational messages
        LOG_LEVEL_INFO     = 0x00000007,

        LOG_DEBUG          = 0x00000008, // debug messages
        LOG_LEVEL_DEBUG    = 0x0000000F, 

        LOG_ALL            = 0x3fffffff, // print everything
        LOG_LEVEL_ALL      = 0x3fffffff,

        // prefix all trace prints with function
        // LOG_PREFIX_FUNC    = 0x80000000, 
        // prefix all trace prints with simulation time
        // LOG_PREFIX_TIME    = 0x40000000, 
    };

private:
    static enum LogLevel m_level;
    static qint32 m_errorCount;
    static QMutex m_mutex;
    
public:
    static void enable(enum LogLevel level);
    
    static void addLine(enum LogLevel level, const QString &str);
    static void addLine(enum LogLevel level, const char *format, ...);

    static qint32 getErrorCount();
};

// Some macros for convenience
//
// Note: The [gcc variadic macro] feature is used here
//       If any error occurrs here, check your compiler settings

#define LOG_ERROR(args...) Log::addLine(Log::LOG_ERROR, ##args)
#define LOG_WARN(args...) Log::addLine(Log::LOG_WARN, ##args)
#define LOG_INFO(args...) Log::addLine(Log::LOG_INFO, ##args)
#define LOG_DEBUG(args...) Log::addLine(Log::LOG_DEBUG, ##args)

#endif /* LOG_H */

