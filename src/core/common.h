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

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdarg.h>

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QMutex>
#include <QProcess>
#include <QRegExp>
#include <QThread>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDataStream>
#include <QVector>
#include <QMap>
#include <QFile>

/**
 * \brief The serialization interface
 */
class Serializable
{
public:
    /**
     * \brief Serialize the object
     *
     * If the I/O device associated with the stream is in reading mode 
     * (QIODevice::ReadOnly), the function will serialize the object from the
     * device.
     * 
     * If the I/O device associated with the stream is in writing mode
     * (QIODevice::WriteOnly), the function will serialize the object to the
     * device.
     *
     * Otherwise, an error is generated.
     */
	virtual void serialize(QDataStream &stream) = 0;
};

#endif /* COMMON_H */

