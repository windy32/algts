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

#ifndef GLOBAL_TIMER_H
#define GLOBAL_TIMER_H

#include "common.h"

/**
 * \brief The global timer
 */
class GlobalTimer
{
private:
    static QTime m_time;
    
public:
    /**
     * \brief Start the timer
     */
    static void start();
    
    /**
     * \brief Returns the milliseconds elapsed since last call to start()
     * \return Milliseconds elapsed since last call to start()
     * \note If start() hasn't been called earlier, the result is undefined
     */
    static int msec();
};

#endif /* GLOBAL_TIMER_H */

