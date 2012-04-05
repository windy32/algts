#include "../core/common.h"
#include "emulator-daemon-session.h"

/**
 * Usage: emulatord <daemon-adddress> <daemon-port>
 *
 * e.g., when users types
 * \code
 * emulatord 10.0.0.1 3201
 * \endcode
 * 
 * Emulator daemon will listen on 10.0.0.1:3201 for emulator setup requests, 
 * and provide network emulation functionality in local host.
 * For each request, daemon sends a response for result with decription 
 * (see "emulator.h" for detailed protocol definition).
 *
 * Currently, there're two defined emulators, "NistNet" and "NetEm", and only
 * the latter one has been implemented.
 *
 * netem has been implemented as a qdisc that provides Network Emulation 
 * functionality for testing protocols by emulating the properties of 
 * wide area networks. The current version emulates variable delay, loss, 
 * duplication and re-ordering.
 * 
 * In Linux 2.6 distributions, netem is already enabled in the kernel
 * and a current version of iproute2 is included.
 *
 * Currently four parameters are supported by emulator daemon: TxRate, 
 * RxRate, TxDelay and RxDelay.
 *
 * To perform rate control, the token bucket filter (tbf) is used as a parent
 * qdisc of the netem qdisc. And to deal with incoming traffic, the 
 * intermediate functional block device (ifb) is used.
 *
 * Typical setup script:
 *
 *    tc qdisc add dev eth0 root handle 1: tbf rate 10mbit buffer 1600 limit 3000
 *    tc qdisc add dev eth0 parent 1: handle 10: netem limit 10000
 *
 *    modprobe ifb
 *    ip link set dev ifb0 up
 *    tc qdisc add dev eth0 ingress
 *    tc filter add dev eth0 parent ffff: protocol ip pref 10 u32 \ 
 *       match u32 0 0 flowid 1:1 action mirred egress redirect dev ifb0
 *    tc qdisc add dev ifb0 root handle 1: tbf rate 10mbit buffer 1600 limit 3000
 *    tc qdisc add dev ifb0 parent 1: handle 10: netem limit 1000 *
 *
 * Typical reset script:
 *
 *    tc qdisc del dev eth0 root handle 1:
 *    tc qdisc del dev ifb0 root handle 1:
 *    tc filter del dev eth0 parent ffff: pref 10 *
 */

bool parseAddr(const QString &str, QHostAddress &addr)
{
    QRegExp rx("^(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)\\."
                "(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)$");
    if( rx.indexIn(str) == -1 )
    {
        LOG_ERROR(QString("Invalid daemon address %1").arg(str));
        return false;
    }
    if( rx.cap(1).toUInt() > 255 || 
        rx.cap(2).toUInt() > 255 ||
        rx.cap(3).toUInt() > 255 ||
        rx.cap(4).toUInt() > 255 )
    {
        LOG_ERROR(QString("Invalid daemon address %1").arg(str));
        return false;
    }
    
    addr = QHostAddress(str);
    return true;
}

bool parsePort(const QString &str, quint16 &port)
{
    bool ok;
    port = str.toUShort(&ok);
    if( !ok )
    {
        LOG_ERROR(QString("Invalid daemon port %1").arg(str));
        return false;
    }
    else if( port < 1024 || port > 65535 )
    {
        LOG_ERROR("Daemon port's range is 1024 ~ 65535");
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    // Check arguments
    if( argc != 3 )
    {
        printf(
            "algts emulator daemon\n"
            "---------------------\n"
            "Usage: \n"
            "    serverd <daemon-address> <daemon-port>\n"
            "Example: \n"
            "    serverd 10.0.0.1 3201\n");
        return 0;
    }
    
    QHostAddress daemonAddr;
    quint16 daemonPort;
    
    if( !parseAddr(argv[1], daemonAddr) ||
        !parsePort(argv[2], daemonPort))
    {
        return 0;
    }
    
    // Create an empty map object that stores parameters
    QMap<QString, QString> params;
    
    // Start server and listen
    QTcpServer daemon;
    if( !daemon.listen(daemonAddr, daemonPort))
    {
        LOG_ERROR(QString("Cannot start emulator daemon @ %1:%2")
            .arg(daemonAddr.toString()).arg(daemonPort));
        return 1;
    }

    while( true )
    {
        // Wait for a new connection
        daemon.waitForNewConnection(-1);
        QTcpSocket *socket = daemon.nextPendingConnection();
        LOG_INFO("Emulator daemon connection established");
        
        // Start a new thread for the session
        EmulatorDaemonSession *session = new EmulatorDaemonSession(socket, params);
        //connect(session, SIGNAL(finished()), thread, SLOT(deleteLator()));
        session->start();
        
        // Block the main thread to avoid concurrent access to paramaters
        session->wait();
    }

    // TODO: capture ctrl+c and stop the loop above
    daemon.close();    
}
