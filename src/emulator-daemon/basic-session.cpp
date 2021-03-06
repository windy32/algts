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

#include "basic-session.h"
#include "../core/log.h"

bool BasicSession::execCommand(const QString &command, 
                               const QString &expectedOutput)
{
    LOG_DEBUG("Beginning of BasicSession::execCommand");

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
    if( output.size() == 0 || output == expectedOutput.toLocal8Bit())
    {
        LOG_DEBUG("End of BasicSession::execCommand");
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
bool BasicSession::execCommit(QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of BasicSession::execCommit");

    LOG_INFO("Updating emulator settings...");
    QMap<QString, QString>::const_iterator it;
    for(it = params.constBegin(); it != params.constEnd(); ++it)
    {
        LOG_INFO(QString("* %1 = %2").arg(it.key()).arg(it.value()));
    }
    
    // Init parameters
    int txRate = 100000; // 100mbps
    int rxRate = 100000; // 100mbps
    enum enumAlgorithm { htb, tbf } algorithm = htb;
    enum enumFairQueue { on, off } fairQueue = on;
    int mtu = 0; // only used for tbf
    
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
    
    if (params.contains("Algorithm")) // "htb" or "tbf"
    {
        if (params["Algorithm"].toLower() == "tbf")
        {
            algorithm = tbf;        
        }
    }
    
    if (params.contains("FairQueue")) // "on" or "off"
    {
        if (params["FairQueue"].toLower() == "off")
        {
            fairQueue = off;
        }
    }
    
    if (params.contains("MTU"))
    {
        if (params["MTU"].toLong() > 0)
        {
            mtu = params["MTU"].toLong();
        }
    }
    
    // Build commands
    QString cmds[10];
    cmds[0] = QString("modprobe ifb");
    cmds[1] = QString("ip link set dev ifb0 up");
    cmds[2] = QString("tc qdisc add dev eth0 handle ffff: ingress");
    cmds[3] = QString("tc filter add dev eth0 parent ffff: protocol ip pref 10"
                      "   u32 match u32 0 0 flowid 1:1"
                      "   action mirred egress redirect dev ifb0");
    int n = 0;
    if (algorithm == htb && fairQueue == on)
    {
        n = 10;
        cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: htb default 1");
        cmds[5] = QString("tc class add dev ifb0 parent 1: classid 1:1 htb"
                          "   rate %1kbit burst 6k quantum 1540").arg(txRate);
	    cmds[6] = QString("tc qdisc add dev ifb0 parent 1:1 handle 10: sfq"
	                      "   perturb 10");
	
        cmds[7] = QString("tc qdisc add dev eth0 root handle 1: htb default 1");
        cmds[8] = QString("tc class add dev eth0 parent 1: classid 1:1 htb"
                          "   rate %1kbit burst 6k quantum 1540").arg(rxRate);
        cmds[9] = QString("tc qdisc add dev eth0 parent 1:1 handle 10: sfq"
                          "   perturb 10");
    }
    else if (algorithm == htb && fairQueue == off)
    {
        n = 10;
        cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: htb default 1");
        cmds[5] = QString("tc class add dev ifb0 parent 1: classid 1:1 htb"
                          "   rate %1kbit burst 6k quantum 1540").arg(txRate);
	    cmds[6] = QString("tc qdisc add dev ifb0 parent 1:1 handle 10: bfifo"
	                      "   limit %1kb").arg(txRate / 16); // max delay = 500 ms
        
        cmds[7] = QString("tc qdisc add dev eth0 root handle 1: htb default 1");
        cmds[8] = QString("tc class add dev eth0 parent 1: classid 1:1 htb"
                          "   rate %1kbit burst 6k quantum 1540").arg(rxRate);
        cmds[9] = QString("tc qdisc add dev eth0 parent 1:1 handle 10: bfifo"
                          "   limit %1kb").arg(rxRate / 16); // max delay = 500 ms
    }
    else if (algorithm == tbf && fairQueue == on)
    {
        n = 8;
        
        // Calculate burst
        int txBurst = 6250;
        int rxBurst = 6250;
        txBurst = (txRate > 50000) ? 158 * (txRate * 0.001) - 1652 : txBurst;
        rxBurst = (rxRate > 50000) ? 158 * (rxRate * 0.001) - 1652 : rxBurst;
        
        if (mtu > 0)
        {
            cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms mtu %3").arg(txRate).arg(txBurst).arg(mtu);
            cmds[5] = QString("tc qdisc add dev ifb0 parent 1: handle 10: sfq"
                              "   perturb 10");

            cmds[6] = QString("tc qdisc add dev eth0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms mtu %3").arg(rxRate).arg(rxBurst).arg(mtu);
            cmds[7] = QString("tc qdisc add dev eth0 parent 1: handle 10: sfq"
                              "   perturb 10");
        }
        else // Do not specify mtu
        {
            cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms").arg(txRate).arg(txBurst);
            cmds[5] = QString("tc qdisc add dev ifb0 parent 1: handle 10: sfq"
                              "   perturb 10");

            cmds[6] = QString("tc qdisc add dev eth0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms").arg(rxRate).arg(rxBurst);
            cmds[7] = QString("tc qdisc add dev eth0 parent 1: handle 10: sfq"
                              "   perturb 10");
        }
    }
    else if (algorithm == tbf && fairQueue == off)
    {
        n = 6;

        // Calculate burst
        int txBurst = 6250;
        int rxBurst = 6250;
        txBurst = (txRate > 50000) ? 158 * (txRate * 0.001) - 1652 : txBurst;
        rxBurst = (rxRate > 50000) ? 158 * (rxRate * 0.001) - 1652 : rxBurst;

        if (mtu > 0)
        {
            cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms mtu %3").arg(txRate).arg(txBurst).arg(mtu);

            cmds[5] = QString("tc qdisc add dev eth0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms mtu %3").arg(rxRate).arg(rxBurst).arg(mtu);
        }
        else
        {
            cmds[4] = QString("tc qdisc add dev ifb0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms").arg(txRate).arg(txBurst);

            cmds[5] = QString("tc qdisc add dev eth0 root handle 1: tbf"
                              "   rate %1kbit burst %2b latency 500ms").arg(rxRate).arg(rxBurst);
        }
    }
    
    // The fourth result "Action 4..." appears in a clean ubuntu server 10.04.4
    // In a clean ubuntu server 12.04.1, however, nothing shows up
    QString expectedOutputs[10] = 
        { "", "", "", "Action 4 device ifb0 ifindex 3\n", 
          "", "", "", "", "", "" }; 
    
    // Execute commands
    //
    // Note:
    //    Commands below require root privilege, if the emulator daemon doesn't
    //    have root privilege, the output will always be empty
    for(int i = 0; i < n; i++)
    {
        if( !execCommand(cmds[i], expectedOutputs[i]))
        {
            return false;
        }
    }

    LOG_DEBUG("End of BasicSession::execCommit");
    return true;
}

bool BasicSession::execReset()
{
    LOG_DEBUG("Beginning of BasicSession::execReset");
    LOG_INFO("Reseting emulator...");
    
    if( !execCommand("tc qdisc del dev eth0 root", ""))
        return false;
    
    if( !execCommand("tc qdisc del dev eth0 ingress", ""))
        return false;
        
    if( !execCommand("tc qdisc del dev ifb0 root", ""))
        return false;

    LOG_DEBUG("End of BasicSession::execReset");
    return true;
}

bool BasicSession::parseCommit(
    QTcpSocket *socket, QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of BasicSession::parseCommit");
    
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
        else if (it.key() == "Algorithm")
        {
            if (it.value().toLower() != "htb" && it.value().toLower() != "tbf")
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
        }
        else if (it.key() == "FairQueue")
        {
            if (it.value().toLower() != "on" && it.value().toLower() != "off")
            {
                LOG_WARN(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                paramValid = false;
                description.append(QString("Invalid value %1 for parameter %2")
                    .arg(it.value()).arg(it.key()));
                continue;
            }
        }
        else if (it.key() == "MTU")
        {
            if (it.value().toLong() <= 0)
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
    LOG_DEBUG("End of BasicSession::parseCommit");
    return paramValid;
}

void BasicSession::parse(QTcpSocket *socket, QMap<QString, QString> &params)
{
    LOG_DEBUG("Beginning of BasicSession::parse");
    
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
    LOG_DEBUG("End of BasicSession::parse");
}

