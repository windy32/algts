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

#include "terminal.h"

Terminal::Terminal()
{
    //m_responseDelay = 300; // Default response delay 300 ms
}

/*
void Terminal::waitForResponse()
{

}
*/

bool Terminal::start()
{
    QByteArray data;
    
    // Start process
    m_process.start(m_program, m_arguments);

    if( !m_process.waitForStarted(1000))
    {
        LOG_ERROR("Cannot start telnet process");
        return false;
    }

    return true;
}

void Terminal::enter(const QString &input)
{
    // LOG_DEBUG("Beginning of Terminal::enter");
    QString buffer;
    QStringList lines;
    
    // Read if possible
    while( m_process.waitForReadyRead(100))
    {
        buffer += m_process.readAll();
    }
    LOG_TERMINAL(buffer);
    
    // Write
    m_process.write(input.toLocal8Bit().data());
    if( !input.endsWith("\n"))
    {
        m_process.write(QString("\n").toLocal8Bit().data());
    }
    
    if( !m_process.waitForBytesWritten(100))
    {
        LOG_ERROR("Cannot write terminal process");
        return;
    }
    
    // Read again
    buffer = "";
    while( m_process.waitForReadyRead(100))
    {
        buffer += m_process.readAll();
    }
    LOG_TERMINAL(buffer);//lines[i]);
}

void Terminal::close()
{
    m_process.terminate();
    if( !m_process.waitForFinished())
    {
        LOG_ERROR("Cannot close terminal");
    }
}

