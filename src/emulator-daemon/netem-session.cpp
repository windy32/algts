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

#include "netem-session.h"
#include "../core/log.h"

bool NetemSession::execCommand(const QString &command, 
                               const QString &expectedOutput)
{
    LOG_DEBUG("Beginning of NetemSession::execCommand");

    // Start process
    QProcess process;
    process.start(command);
    
    if( !process.waitForStarted(1000))
    {
        LOG_ERROR(QString("Cannot execute command %1").arg(command));
        process.terminate();
        return false;
    }
    
    // Get output and close the process
    QByteArray output;
    if( process.waitForReadyRead(100))
    {
        output = process.readAll();
    }
    process.waitForFinished(-1); //terminate();
    
    // Compare and return
    if( output == expectedOutput.toLocal8Bit())
    {
        LOG_DEBUG("End of NetemSession::execCommand");
        return true;
    }
    else
    {
        LOG_ERROR(QString("An error occurred executing command %1")
            .arg(command));
        LOG_INFO(QString("Expected output: %1").arg(expectedOutput));
        LOG_INFO(QString("Actual output: %1").arg(QString(output)));
        return false;
    }
}

// ! Need reconstruct
bool NetemSession::execCommit(QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of NetemSession::execCommit");

    LOG_INFO("Updating emulator settings...");
    QMap<QString, QString>::const_iterator it;
    for(it = params.constBegin(); it != params.constEnd(); ++it)
    {
        LOG_INFO(QString("* %1 = %2").arg(it.key()).arg(it.value()));
    }
    
    // Init parameters
    int txRate = 100000; // 100mbps
    int rxRate = 100000; // 100mbps
    int txDelay = 0; // 0ms
    int rxDelay = 0; // 0ms
    
    // Read parameters (no error handling is necessary here)
    if( params.contains("TxRate"))
    {
        QString number, unit;
        QRegExp rx("^([0-9]+)(m|M|k|K)bps$");
        if( rx.indexIn(params["TxRate"]) != -1 )
        {
            number = rx.cap(1);
            unit = rx.cap(2);
            int inc = ( unit == "M" || unit == "m" ) ? 1024 : 1;
            txRate = number.toLong() * inc; // unit is kbps
        }
    }
    
    if( params.contains("RxRate"))
    {
        QString number, unit;
        QRegExp rx("^([0-9]+)(m|M|k|K)bps$");
        if( rx.indexIn(params["RxRate"]) != -1 )
        {
            number = rx.cap(1);
            unit = rx.cap(2);
            int inc = ( unit == "M" || unit == "m" ) ? 1024 : 1;
            rxRate = number.toLong() * inc; // unit is kbps
        }
    }
    
    if( params.contains("TxDelay"))
    {
        QRegExp rx("^([0-9]+)ms$");
        if( rx.indexIn(params["TxDelay"]) != -1 )
        {
            txDelay = rx.cap(1).toLong();
        }
    }

    if( params.contains("RxDelay"))
    {
        QRegExp rx("^([0-9]+)ms$");
        if( rx.indexIn(params["RxDelay"]) != -1 )
        {
            rxDelay = rx.cap(1).toLong();
        }
    }

    // Build commands
    QString commands[8];
    commands[0] = QString( // Token 20KB, Queue 40KB
        "tc qdisc add dev eth0 root handle 1: "
        "tbf rate %1kbit buffer 20000 limit 40000").arg(rxRate);
    commands[1] = QString(
        "tc qdisc add dev eth0 parent 1: handle 10: "
        "netem limit 100000 delay %1ms").arg(rxDelay); // Queue 100KB
    commands[2] = QString("modprobe ifb");
    commands[3] = QString("ip link set dev ifb0 up");
    commands[4] = QString("tc qdisc add dev eth0 ingress");
    commands[5] = QString(
        "tc filter add dev eth0 parent ffff: protocol ip pref 10 "
        "   u32 match u32 0 0 flowid 1:1 "
        "   action mirred egress redirect dev ifb0");
    commands[6] = QString( // Token 5KB, Queue 10KB
        "tc qdisc add dev ifb0 root handle 1: "
        "tbf rate %1kbit buffer 5000 limit 10000").arg(txRate);
    commands[7] = QString(
        "tc qdisc add dev ifb0 parent 1: handle 10: "
        "netem limit 100000 delay %1ms").arg(txDelay); // Queue 100KB
    
    // The sixth result appears in a clean ubuntu server 10.04.4
    QString expectedOutputs[8] = 
        { "", "", "", "", "", "Action 4 device ifb0 ifindex 3\n", "", "" }; 
    
    // Execute commands
    //
    // Note:
    //    Commands below require root privilege, if the emulator daemon doesn't
    //    have root privilege, the output will always be empty
    for(int i = 0; i < 8; i++)
    {
        if( !execCommand(commands[i], expectedOutputs[i]))
        {
            return false;
        }
    }

    LOG_DEBUG("End of NetemSession::execCommit");
    return true;
}

bool NetemSession::execReset()
{
    LOG_DEBUG("Beginning of NetemSession::execReset");
    LOG_INFO("Reseting emulator...");
    
    if( !execCommand("tc qdisc del dev eth0 root handle 1:", ""))
        return false;
    
    if( !execCommand("tc qdisc del dev eth0 ingress", ""))
        return false;
    
    if( !execCommand("tc filter del dev eth0 parent ffff: pref 10", ""))
        return false;
    
    if( !execCommand("tc qdisc del dev ifb0 root handle 1:", ""))
        return false;

    LOG_DEBUG("End of NetemSession::execReset");
    return true;
}

bool NetemSession::parseCommit(
    QTcpSocket *socket, QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of NetemSession::parseCommit");
    
    QMap<QString, QString> newParams;
    QDataStream in(socket);
    in >> newParams;
    
    bool paramValid = true;
    QString description;
    
    // Validate parameters
    QMap<QString, QString>::const_iterator it;
    for(it = newParams.constBegin(); it != newParams.constEnd(); ++it)
    {
        if( it.key() == "TxRate" || it.key() == "RxRate" )
        {
            // Capture number and unit
            QString number, unit;
            QRegExp rx("^([0-9]+)(m|M|k|K)bps$");

            if( rx.indexIn(it.value()) == -1 )
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
            number = rx.cap(1);
            unit = rx.cap(2);

            // Check range
            bool ok;
            qint32 intValue = number.toLong(&ok);
            qint32 inc = ( unit == "M" || unit == "m" ) ? 1024 * 1024 : 1024;

            if( !ok || // cannot convert to long
                intValue < 1024 / inc || // less than 1kbps
                intValue > 100 * 1024 * 1024 / inc ) // or greater than 100mbps
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
        }
        else if( it.key() == "TxDelay" || it.key() == "RxDelay" )
        {
            // Capture number
            QString number;
            QRegExp rx("^([0-9]+)ms$");
            
            if( rx.indexIn(it.value()) == -1 )
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
            
            number = rx.cap(1);
            
            // Convert to bytes with limitation
            bool ok;
            qint32 intValue = number.toLong(&ok);

            if( !ok || // cannot convert to long, 
                intValue < 10 || // less than 10ms
                intValue > 1000 ) // or greater than 1000ms
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
        }
        else
        {
            LOG_WARN(QString("Invalid parameter %1").arg(it.key()));
            paramValid = false;
            description.append(QString("Invalid parameter %1").arg(it.key()));
            continue;
        }
    }
    
    if( paramValid )
    {
        // Update parameter list    
        for(it = newParams.constBegin(); it != newParams.constEnd(); ++it)
        {
            params[it.key()] = it.value();
        }
    }
    else // Send response with a unsuccessful result
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint32)0;
        out << false;
        out << description;
        out.device()->seek(0);
        out << (quint32)(block.size() - 4);

        socket->write(block);
        socket->waitForBytesWritten(-1);
    }
    LOG_DEBUG("End of NetemSession::parseCommit");
    return paramValid;
}

void NetemSession::parse(QTcpSocket *socket, QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of NetemSession::parse");
    
    QDataStream in(socket);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    // Get command
    QString command;
    while( socket->bytesAvailable() < 4 ) 
    {
        if( !socket->waitForReadyRead(3 * 1000)) 
        {
            LOG_ERROR(QString("Cannot get emulator command, %1")
                .arg(socket->errorString()));
            return;
        }
    }
    in >> command;
    
    if( command == "COMMIT" )
    {
        if( parseCommit(socket, params))
        {
            out << (quint32)0;
            if( execCommit(params) == true )
            {
                out << true;
                out << QString("Parameters updated successfully");
            }
            else
            {
                out << false;
                out << QString("Cannot update parameters");
            }
            out.device()->seek(0);
            out << (quint32)(block.size() - 4);
            
            socket->write(block);
            socket->waitForBytesWritten(-1);
        }
    }
    else if( command == "RESET" )
    {
        out << (quint32)0;
        if( execReset() == true )
        {
            out << true;
            out << QString("Emulator reset successfully");
        }
        else
        {
            out << false;
            out << QString("Cannot reset parameters");
        }
        out.device()->seek(0);
        out << (quint32)(block.size() - 4);

        socket->write(block);
        socket->waitForBytesWritten(-1);
    }
    else
    {
        LOG_ERROR(QString("Invalid command %1").arg(command));
        out << false;
        out << QString("Invalid command %1").arg(command);
        socket->write(block);
        socket->waitForBytesWritten(-1);
    }
    
    // Close connection
    socket->close();
    LOG_DEBUG("End of NetemSession::parse");
}
