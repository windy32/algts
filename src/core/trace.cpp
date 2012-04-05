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

void TextTrace::generate(QMap<QString, QVector<Task *> > &tasks)
{

}
