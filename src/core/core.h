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

#include "random-variable.h"
#include "scenario.h"
#include "log.h"
#include "trace.h"
#include "app/console-application.h"

#include "task/bulk-download-task.h"
#include "task/bulk-upload-task.h"
#include "task/onoff-download-task.h"
#include "task/tcp-echo-task.h"
#include "task/async-udp-echo-task.h"

#include "client/bulk-download-client.h"
#include "client/bulk-upload-client.h"
#include "client/onoff-download-client.h"
#include "client/tcp-echo-client.h"
#include "client/async-udp-echo-client.h"

#include "server/server-factory.h"
#include "server/bulk-download-server.h"
#include "server/bulk-upload-server.h"
#include "server/onoff-download-server.h"
#include "server/tcp-echo-server.h"
#include "server/async-udp-echo-server.h"

#include "emulator/netem-emulator.h"
#include "emulator/nistnet-emulator.h"

#include "terminal/telnet-terminal.h"
#include "terminal/ssh-terminal.h"

#endif /* CORE_H */

