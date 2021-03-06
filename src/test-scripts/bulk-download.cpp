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

#include "../core/core.h"

/**
 * \file bulk-download.cpp
 * \ingroup Scripts
 * \brief Test script for bulk download task
 *
 * Usage:
 * \code
 * bulk-download <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 */

/**
 * \brief The entry point of the script
 */
int main(int argc, char *argv[])
{
    // Enable logging and tracing
    Log::enable(Log::LOG_LEVEL_DEBUG);
    
    // Start console application and check arguments
    ConsoleApplication app(argc, argv);
    
    // Setup scenario
    LOG_DEBUG("Setting up scenaio");
    
    Scenario s(12345, 10); // seed & length
    s.addUser("Harry");

    for (int i = 0; i < 1; i++)
    {
        s.addTask("Harry", new BulkDownloadTask(80));
        s.task()->setAttribute("MaxBytes", "INFINITE");
        s.task()->setAttribute("MaxRate", "INFINITE"/*"3Mbps"*/);
    }
        
    // Execute
    app.exec(&s);
    
    // Calculate average throughput
    int kbytes = 0;
    for (int i = 0; i < 1; i++)
    {
        RawTraceItem rti;
        app.exportRawTrace("Harry", 0, rti);
        kbytes += rti["TotalBytes"].last() / 1024;
    }
    LOG_INFO("Throughput: %d KB/s", kbytes / 10);
    
    return 0;
}

