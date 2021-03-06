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

#include "emulator.h"

Emulator::Emulator(const QString &addr, quint16 port)
{
    m_addr = addr;
    m_port = port;
}

void Emulator::setParam(const QString &name, QString value)
{
    LOG_DEBUG("Beginning of Emulator::setParam");
    
    if( m_params.contains(name))
    {
        LOG_INFO(QString("Parameter %1 reset to %2").arg(name).arg(value));
    }
    else
    {
        LOG_INFO(QString("Parameter %1 created with value %2")
            .arg(name).arg(value));
    }
    m_params[name] = value;

    LOG_DEBUG("End of Emulator::setParam");
}