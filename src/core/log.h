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

typedef void (* LogRedirectCallback)(const char *content);

/**
 * \file log.h
 * \brief Global logging support
 */


/**
 * \brief The global log object
 * \ingroup Constructs
 */
class Log
{
public:
    /**
     * \brief The log level
     */
    enum LogLevel
    {
        /** Show nothing */
        LOG_LEVEL_NONE     = 0x00000000,
        
        /** Serious error messages */
        LOG_ERROR          = 0x00000001,
        /** Show error messages */
        LOG_LEVEL_ERROR    = 0x00000001,

        /** Warning messages */
        LOG_WARN           = 0x00000002, 
        /** Show error and warning messages */
        LOG_LEVEL_WARN     = 0x00000003,

        /** Informational messages */
        LOG_INFO           = 0x00000004,
        /** Show error, warning and information messages */
        LOG_LEVEL_INFO     = 0x00000007,

        /** Debug messages */
        LOG_DEBUG          = 0x00000008,
        /** Show error, warning, information and debug messages */
        LOG_LEVEL_DEBUG    = 0x0000000F, 

        /** Show everything */
        LOG_LEVEL_ALL      = 0x3fffffff,

        // prefix all trace prints with function
        // LOG_PREFIX_FUNC    = 0x80000000, 
        // prefix all trace prints with simulation time
        // LOG_PREFIX_TIME    = 0x40000000, 
        /** Hide log level */
        LOG_PREFIX_NOLEVEL   = 0x80000000,
        
        /** Drop the newline character */
        LOG_PREFIX_NONEWLINE = 0x40000000, 
        
        /** Hide log level and drop the newline character */
        LOG_PREFIX_TERMINAL  = 0xC0000000
    };

private:
    static enum LogLevel m_level;
    static qint32 m_errorCount;
    static QMutex m_mutex;
    static LogRedirectCallback m_callback;

private:
    static void print(const char *content);

public:
    /**
     * \brief Enable logging at specific level
     * \param level The log level
     */
    static void enable(enum LogLevel level);
    
    /**
     * \brief Register the logging callback function
     * \param callback The callback function pointer
     */
    static void enableRedirect(LogRedirectCallback callback);
    
    /**
     * \brief Add a line in the log
     * \param level The log level of the line
     * \param str The content
     */
    static void addLine(enum LogLevel level, const QString &str);

    /**
     * \brief Add a line in the log
     * \param level The log level of the line
     * \param format The c style format string of the content
     */
    static void addLine(enum LogLevel level, const char *format, ...);

    /**
     * \brief Returns the number of log items at ERROR level
     * \return The number of log items at ERROR level
     */
    static qint32 getErrorCount();
};

// Some macros for convenience
//
// Note: The [gcc variadic macro] feature is used here
//       If any error occurrs here, check your compiler settings
/**
 * \def LOG_ERROR(args..)
 *  Add a line in the log at ERROR level
 */
#define LOG_ERROR(args...) Log::addLine(Log::LOG_ERROR, ##args)
/**
 * \def LOG_WARN(args..)
 *  Add a line in the log at WARNING level
 */
#define LOG_WARN(args...) Log::addLine(Log::LOG_WARN, ##args)
/**
 * \def LOG_INFO(args..)
 *  Add a line in the log at INFORMATION level
 */
#define LOG_INFO(args...) Log::addLine(Log::LOG_INFO, ##args)
/**
 * \def LOG_DEBUG(args..)
 *  Add a line in the log at DEBUG level
 */
#define LOG_DEBUG(args...) Log::addLine(Log::LOG_DEBUG, ##args)

/**
 * \def LOG_TERMINAL(args..)
 *  Add a line in the log at INFO level with the TERMINAL prefix
 */
#define LOG_TERMINAL(args...) Log::addLine((Log::LogLevel)(Log::LOG_INFO | Log::LOG_PREFIX_TERMINAL), ##args)

#endif /* LOG_H */

