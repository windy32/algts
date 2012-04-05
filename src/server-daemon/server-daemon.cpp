#include "../core/core.h"

/**
 * Usage: serverd <daemon-adddress> <daemon-port> <server-address-range>
 *
 * e.g., when users types
 * \code
 * serverd 10.0.0.1 3200 10.0.0.8/29
 * \endcode
 * 
 * Server daemon will listen on 10.0.0.1:3200 for server layout requests.
 * A server layout request consists of a TaskType with a corresponding
 * port: 
 * 
 *   [int32 taskType, uint16 port]
 * 
 * Each server layout request occupies a new tcp connection, which means that
 * in order to set up multiple servers, a number of connection should be created.
 * This design is simply used to simplify coding, since efficiency is inessential
 * when setting up servers.
 * 
 * Currently, there're 5 task types. Consider the scenario below:
 * 
 *   Bulk Download @ port 80
 *   Bulk Download @ port 8080
 *   Bulk Upload @ port 80
 *   On/Off Download @ port 80
 *   TCP Echo @ port 23
 *   UDP Echo @ port 5000
 *
 * For convenience, tasks of the same type occupy a same address.
 *
 *   10.0.0.8: Bulk Download Server @ port 80, 8080
 *   10.0.0.9: Bulk Upload Server @ port 80
 *   10.0.0.10: On/Off Download Server @ port 80
 *   10.0.0.11: TCP Echo Server @ port 23
 *   10.0.0.12: UDP Echo Server @ port 5000
 *
 * Server daemon first checks how many server addresses are actually available. 
 * When the number of addresses available are less than 5, an error message
 * will be generated and the daemon will not continue.
 *
 * For each server layout request, daemon sends a reply which consists of 
 * following elements:
 *
 *   [uint32 length]
 *   [bool result][uint32 Address][QString success decription]
 *
 * or
 *
 *   [uint32 length]
 *   [bool result][QString failure description]
 *
 * When a server is already listening at a specific port, server daemon
 * will not try to start a server with the same port again,
 * The reply will be {true, "Server already started @ addr:port"}.
 *
 */

QHostAddress taskAddrs[5];
QSet<QPair<qint32, quint16> > servers;

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

bool parseAddrRange(const QString &str, QList<QHostAddress> addrRange)
{
    QRegExp rx("^(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)\\."
                "(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)/(0|[1-9][0-9]*)$");
    if( rx.indexIn(str) == -1 )
    {
        LOG_ERROR(QString("Invalid server address range %1").arg(str));
        return false;
    }
    if( rx.cap(1).toUInt() > 255 || 
        rx.cap(2).toUInt() > 255 ||
        rx.cap(3).toUInt() > 255 ||
        rx.cap(4).toUInt() > 255 )
    {
        LOG_ERROR(QString("Invalid server address range %1").arg(str));
        return false;
    }
    if( rx.cap(5).toInt() < 26 || 
        rx.cap(5).toInt() > 29 )
    {
        LOG_ERROR("Subnet mask should be /26, /27, /28 or /29");
        return false;
    }
    
    quint32 mask = 0xffffffff << (32 - rx.cap(5).toUInt());
    int n = 1 << (32 - rx.cap(5).toUInt());
    quint32 firstAddr = (
        (rx.cap(1).toUInt() << 24) | 
        (rx.cap(2).toUInt() << 16) | 
        (rx.cap(3).toUInt() << 8) | 
        (rx.cap(4).toUInt())) & mask;

    for(int i = 0; i < n; i++)
    {
        addrRange.append(QHostAddress(firstAddr + i));
    }
    return true;
}


class ServerDaemonSession : public QThread
{
private:
    QTcpSocket *m_socket;
    
protected:
    virtual void run();

public:
    ServerDaemonSession(QTcpSocket *socket);
};

ServerDaemonSession::ServerDaemonSession(QTcpSocket *socket)
    : m_socket(socket)
{
}

void ServerDaemonSession::run()
{
    LOG_DEBUG("Beginning of ServerDaemonSession::run");

    qint32 taskType;
    quint16 port;
    QDataStream in(m_socket);

    // Receive the whole packet
    while( m_socket->bytesAvailable() < 6) 
    {
        if (!m_socket->waitForReadyRead(3 * 1000)) 
        {
            LOG_INFO("Server daemon session timed out");
            return;
        }
    }
    in >> taskType >> port;
    
    // Start server
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)0;

    if( taskType >= (qint32)Task::BULK_DOWNLOAD &&
        taskType <= (qint32)Task::ASYNC_UDP_ECHO )
    {
        if( servers.contains(QPair<qint32, quint16>(taskType, port)))
        {
            out << true;
            out << QString("Server already started @ %1:%1")
                       .arg(taskAddrs[taskType].toIPv4Address()).arg(port);
        }
        else
        {
            Server *server = ServerFactory::create(
                (Task::Type)taskType, taskAddrs[taskType], port);
            QString description;
            
            if( server->start(description))
            {
                // Send response
                out << true;
                out << taskAddrs[taskType].toIPv4Address();
                out << description;
                
                servers.insert(QPair<qint32, quint16>(taskType, port));
            }
            else
            {
                out << false;
                out << description;
            }
        }
    
    }
    else
    {
        out << false;
        out << QString("Invalid task type %1").arg(taskType);
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - 4);

    m_socket->write(block);
    m_socket->waitForBytesWritten(-1);
    
    // Close connection
    m_socket->disconnectFromHost();
    m_socket->waitForDisconnected();

    LOG_DEBUG("End of ServerDaemonSession::run");
}

int main(int argc, char *argv[])
{
    // Check arguments
    if( argc != 4 )
    {
        printf(
            "algts server daemon\n"
            "-------------------\n"
            "Usage: \n"
            "    serverd <daemon-address> <daemon-port> <server-address-range>\n"
            "Example: \n"
            "    serverd 10.0.0.1 3200 10.0.0.8/29\n");
        return 0;
    }
    
    QHostAddress daemonAddr;
    quint16 daemonPort;
    QList<QHostAddress> serverAddrRange;
    
    if( !parseAddr(argv[1], daemonAddr) ||
        !parsePort(argv[2], daemonPort) ||
        !parseAddrRange(argv[3], serverAddrRange))
    {
        return 0;
    }
    
    // Assign addresses to tasks
    QList<QHostAddress> allAddrs = QNetworkInterface::allAddresses();
    int numAddr = 0;
    
    for(int i = 0; i < serverAddrRange.size(); i++)
    {
        if( allAddrs.contains(serverAddrRange[i]))
        {
            LOG_INFO(QString("Address %1 available")
                .arg(serverAddrRange[i].toString()));
            if( numAddr < 5 )
            {
                taskAddrs[numAddr] = serverAddrRange[i];
            }
            numAddr += 1;
        }
        else
        {
            LOG_INFO(QString("Address %s not available")
                .arg(serverAddrRange[i].toString()));
        }
    }
    
    if( numAddr < 5 )
    {
        LOG_ERROR("No enough addresses");
        return 1;
    }
    
    // Start server and listen
    QTcpServer daemon;
    if( !daemon.listen(daemonAddr, daemonPort))
    {
        LOG_ERROR(QString("Cannot start server daemon @ %1:%2")
            .arg(daemonAddr.toString()).arg(daemonPort));
        return 1;
    }

    while( true )
    {
        // Wait for a new connection
        daemon.waitForNewConnection(-1);
        QTcpSocket *socket = daemon.nextPendingConnection();
        LOG_INFO("Server daemon connection established");
        
        // Start a new thread for the session
        ServerDaemonSession *session = new ServerDaemonSession(socket);
        //connect(session, SIGNAL(finished()), thread, SLOT(deleteLator()));
        session->start();
        
        // Block the thread to avoid concurrent visit to QSet servers
        session->wait();
    }

    // TODO: capture ctrl+c and stop the loop above
    daemon.close();    
}
