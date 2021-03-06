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

#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/**
 * \file utils.h
 * \brief Utilities
 */

/**
 * \brief The utilities class
 * \ingroup Constructs
 */

class Utils : public QThread
{
public:
    /**
     * \brief Sleep for some seconds
     */
    static void sleep(unsigned long secs);
    /**
     * \brief Sleep for some milliseconds
     */
    static void msleep(unsigned long msecs); 
    /**
     * \brief Sleep for some microseconds
     */
    static void usleep(unsigned long usecs);
};

#endif /* UTILS_H */

