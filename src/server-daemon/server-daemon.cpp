#include "../core/common.h"
#include "../core/log.h"
#include "../core/server/server-factory.h"

/**
 * \defgroup ServerDaemon Server Daemon
 */

/**
 * \file server-daemon.cpp
 * \ingroup ServerDaemon
 * \brief The server daemon
 * 
 * Usage: 
 * \code
 * serverd <daemon-adddress> <daemon-port> <server-address-range>
 * \endcode
 * e.g., when users types
 * \code
 * serverd 10.0.0.1 3200 10.0.0.8/29
 * \endcode
 * 
 * The server daemon will listen on 10.0.0.1:3200 for server layout requests.
 * A server layout request consists of a TaskType with a corresponding port.
 * 
 * Field        | Type         | Description
 * -------------|--------------|---------------------------
 * TaskType     | int32        | Type of the server
 * Port         | uint16       | Port of the server
 * 
 * Each server layout request occupies a new tcp connection, which means that in
 * order to set up multiple servers, a number of connection should be created.
 * The design is simply used to simplify coding, since efficiency is inessential
 * when setting up servers.
 * 
 * Currently, there're 5 task types. Consider the scenario below.
 *  - Bulk Download @ port 80
 *  - Bulk Download @ port 8080
 *  - Bulk Upload @ port 80
 *  - On/Off Download @ port 80
 *  - TCP Echo @ port 23
 *  - UDP Echo @ port 5000
 *
 * For convenience, tasks of the same type occupy a same address.
 *  - 10.0.0.8: Bulk Download Server @ port 80, 8080
 *  - 10.0.0.9: Bulk Upload Server @ port 80
 *  - 10.0.0.10: On/Off Download Server @ port 80
 *  - 10.0.0.11: TCP Echo Server @ port 23
 *  - 10.0.0.12: UDP Echo Server @ port 5000
 *
 * The server daemon first checks how many server addresses are actually
 * available. When the number of addresses available are less than 5, an error
 * message will be generated and the daemon will not continue.
 *
 * For each server layout request, daemon sends a reply which consists of the
 * following elements:
 *
 * Field        | Type         | Description
 * -------------|--------------|---------------------------
 * BlockSize    | uint32       | Size of the remaining part
 * Result       | bool         | true
 * Address      | quint32      | IPv4 Address of the server
 * Description  | QString      | Detailed description
 *
 * or
 *
 * Field        | Type         | Description
 * -------------|--------------|---------------------------
 * BlockSize    | uint32       | Size of the remaining part
 * Result       | bool         | false
 * Description  | QString      | Detailed description
 *
 * \note When a server is already listening at a specific port, server daemon
 *       will not try to start a server with the same port again, and the result
 *       would be true.
 */

/**
 * \brief The ip address for tasks
 *
 * Since currently, only five task types are available, the length of the array
 * is fixed to 5. The index of the array is determined by the Task::Type
 * emulation.
 * \see Task
 */
QHostAddress taskAddrs[5];
/**
 * \brief The running servers
 *
 * The first element of the QPari represents the IPv4 address, and the second
 * represents the port.
 */
QSet<QPair<qint32, quint16> > servers;

/**
 * \brief Parse the IPv4 address
 * \param str The string to be parsed
 * \param addr The output ip address
 * \return Returns true if str represents a valid IPv4 address, and false
 *         otherwise.
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

/**
 * \brief Parse the port
 * \param str The string to be parsed
 * \param port The output port
 * \return Returns true if str represents a valid port, and false otherwise.
 */
bool parsePort(const QString &str, quint16 &port)
{
    bool ok;
    port = str.toUShort(&ok);
    if( !ok )
    {
        LOG_ERROR(QString("Invalid daemon port %1").arg(str));
        return false;
    }
    else if( port < 1024 )
    {
        LOG_ERROR("Daemon port's range is 1024 ~ 65535");
        return false;
    }
    return true;
}

/**
 * \brief Parse the IPv4 address range
 * \param str The string to be parsed
 * \param addrRange The output IPv4 address range
 * \return Returns true if str represents a valid range, and false otherwise.
 *
 * \note The size of the subnet should be greater than or equal to 8 (since
 *       there are five task types), and less than or equal to 64.\n
 *       An example of the expression is "10.0.0.8/29"
 */
bool parseAddrRange(const QString &str, QList<QHostAddress> &addrRange)
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

/**
 * \brief The server daemon session
 * \ingroup ServerDaemon
 */
class ServerDaemonSession : public QThread
{
private:
    QTcpSocket *m_socket;
    
protected:
    /**
     * \brief The entry point the thread function
     *
     * The daemon receives the request and trys to start a server here.
     * \see server-daemon.cpp
     */
    virtual void run();

public:
    /**
     * \brief Initialize the server daemon session with an established socket
     */
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
    
    LOG_DEBUG("TaskType = %d, Port = %d", (int)taskType, (int)port);
    
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
            out << taskAddrs[taskType].toIPv4Address();
            out << QString("Server already started @ %1:%2")
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
    m_socket->close();
    LOG_DEBUG("End of ServerDaemonSession::run");
}

/**
 * \brief The entry point the server daemon
 */
int main(int argc, char *argv[])
{
    // Check arguments
    if( argc != 4 )
    {
        printf(
            "algts server daemon\n"
            "-------------------\n"
            "Usage: \n"
            "   serverd <daemon-address> <daemon-port> <server-address-range>\n"
            "Example: \n"
            "   serverd 10.0.0.1 3200 10.0.0.8/29\n");
        return 0;
    }
    
    Log::enable(Log::LOG_LEVEL_INFO);
    
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
            LOG_INFO(QString("Address %1 not available")
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
