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

#include "../core/core.h"

/**
 * \defgroup Scripts Scripts
 */

/**
 * \file reset.cpp
 * \ingroup Scripts
 * \brief A script that resets the emulator
 *
 * Usage:
 * \code
 * reset
 * \endcode
 */

/**
 * \brief The entry point of the sample
 */
int main(int argc, char *argv[])
{
    // Enable logging
    Log::enable(Log::LOG_LEVEL_DEBUG);
    
    // Setup emulator
    NetemEmulator emulator("10.0.0.1", 3201);
    emulator.reset();
    
    return 0;
}

