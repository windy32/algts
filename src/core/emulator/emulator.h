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

#ifndef EMULATOR_H
#define EMULATOR_H

#include "../common.h"

/** 
 * Protocol Definition
 *
 * = Commit =
 *
 * 1. Client connect
 * 2. Client send request 
 *       [uint32 length] (remaining part of the request)
 *       [QString emulatorName][QString "COMMIT"][QMap<QString, QString> params]
 * 3. Emulator daemon send response 
 *       [uint32 length]
 *       [bool result][String description]
 * 4. Client disconnect
 * 
 * Note: If part of the parameters are invalid, the [result] will be false, 
 *       and emulator daemon will ignore the whole request.
 * 
 * = Reset =
 *
 * 1. Client connect
 * 2. Client send 
 *       [uint32 length]
 *       [QString emulatorName][QString "RESET"]
 * 3. Emulator daemon send response 
 *       [uint32 length]
 *       [bool result][String description]
 * 4. Client disconnect
 *
 */
 
class Emulator
{
protected:
    QMap<QString, QString> m_params;
    QString m_addr;
    quint16 m_port;
    
public:
    Emulator(const QString &addr, quint16 port);
    
    virtual void setParam(const QString &name, QString value);
    virtual void commit() = 0;
    virtual void reset() = 0;
};

#endif /* EMULATOR_H */
