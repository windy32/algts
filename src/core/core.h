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

#ifndef CORE_H
#define CORE_H

/**
 * \defgroup Core Core
 */

/**
 * \defgroup Constructs C++ Constructs Used by All Modules
 * \ingroup Core
 */

/**
 * \defgroup Factories Factories
 * \ingroup Core
 */

// Emulation support
#include "emulator/basic-emulator.h"
#include "emulator/nistnet-emulator.h"

// Terminal support
#include "terminal/telnet-terminal.h"
#include "terminal/ssh-terminal.h"

// Debugging support
#include "log.h"
#include "trace.h"

// Application, scenario and tasks
#include "app/console-application.h"
#include "scenario.h"

#include "task/bulk-download-task.h"
#include "task/bulk-upload-task.h"
#include "task/onoff-download-task.h"
#include "task/tcp-echo-task.h"
#include "task/async-udp-echo-task.h"

#endif /* CORE_H */

