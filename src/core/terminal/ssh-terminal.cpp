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

#include "ssh-terminal.h"

SshTerminal::SshTerminal(const QString &addr)
{
    m_program = "sshpass";
    m_arguments = QStringList(addr.split(" "));
}

void SshTerminal::enter(const QString &input)
{
    // With sshpass, it's impossible to get input from stdout
    // so it's a bit different here

    QString buffer;
    QStringList lines;
    
    // Read if possible
    while( m_process.waitForReadyRead(100))
    {
        buffer += m_process.readAll();
    }
    
    if (buffer != "")
    {
        lines = buffer.split("\n");
        for(int i = 0; i < lines.size(); i++)
        {
            LOG_INFO(QString("> %1").arg(lines[i]));
        }
    }
    
    // Write
    m_process.write(input.toLocal8Bit().data());
    if( !m_process.waitForBytesWritten(100))
    {
        LOG_ERROR("Cannot write terminal process");
        return;
    }
    
    QString inputWithoutNewline = input;
    while (inputWithoutNewline.endsWith("\n"))
    {
        inputWithoutNewline.resize(inputWithoutNewline.size() - 1);
    }
    LOG_INFO(QString("> %1").arg(inputWithoutNewline));
    
    // Read again
    buffer = "";
    while( m_process.waitForReadyRead(100))
    {
        buffer += m_process.readAll();
    }
    
    if (buffer != "")
    {
        lines = buffer.split("\n");
        for(int i = 0; i < lines.size(); i++)
        {
            LOG_INFO(QString("> %1").arg(lines[i]));
        }
    }
}

