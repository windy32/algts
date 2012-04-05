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

#ifndef NULL_TASK_H
#define NULL_TASK_H

#include "task.h"

// Any call to a null task will generate a warning
class NullTask : public Task
{
private:
    static NullTask *m_instance;
    NullTask();

public:
    static NullTask *getInstance();

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public:
    virtual enum Type getType();
    virtual void serialize(QDataStream *pStream);
    virtual void expand(qint32 length);
};

#endif /* NULL_TASK_H */

