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

#include "trace.h"
#include "client/client.h"

bool TextTrace::m_enabled = false;
QString TextTrace::m_filename;

void TextTrace::enable(const QString &filename)
{
    m_enabled = true;
    m_filename = filename;
}

bool TextTrace::enabled()
{
    return m_enabled;
}

void TextTrace::generate(QMap<QString, QVector<Task *> > &tasks,
                         QList<Client *> &clients)
{
    // Open file
    QFile traceFile(m_filename + ".trace");
    if (!traceFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG_ERROR(QString("Cannot open file %1").arg(m_filename + ".trace"));
        return;
    }
    QTextStream out(&traceFile);
    
    // Traverse and write
    QMap<QString, QVector<Task *> >::iterator it;
    int index = 0;
    for(it = tasks.begin(); it != tasks.end(); ++it)
    {
        for(int i = 0; i < it.value().size(); i++)
        {
            // Heading of a text trace item
            Task *task = it.value()[i];
            out << "UserName=\"" << it.key() << "\""
                << " Type=\"" << task->getName() << "\"\n";
            out << "{\n";
            
            
            // The content
            TextTraceItem content;
            clients[index++]->generateTextTrace(content);
            for(int j = 0; j < content.size(); j++)
            {
                out << "  " << content[j] << "\n";
            }
            
            // Foot of a text trace item
            out << "}\n";
        }
    }
    
    // Close file
    LOG_INFO("Trace file generated successfully");
    traceFile.close();
}

