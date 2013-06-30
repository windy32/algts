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

#ifndef EMULATOR_H
#define EMULATOR_H

#include "../common.h"
#include "../log.h"

/**
 * \defgroup Emulators Emulators
 * \ingroup Core
 */

/**
 * \ingroup Emulators
 * \brief The base class of all emulators
 *
 * In Linux, it's easy to provide network emulation functionality with the
 * kernel modules or packages like netem or NIST Net.
 *
 * In a test for gateways, four wide area network properties are extremely
 * important.
 *  - Upstream rate
 *  - Downstream rate
 *  - Upstream delay
 *  - Downstream delay
 * 
 * Here's a typical script that sets the rate properties above.
 *
 * \code
 * modprobe ifb
 * ip link set dev ifb0 up
 * tc qdisc add dev eth0 handle ffff: ingress
 * 
 * tc filter add dev eth0 parent ffff: protocol ip pref 10 \
 *    u32 match u32 0 0 flowid 1:1 \
 *    action mirred egress redirect dev ifb0
 * 
 * tc qdisc add dev ifb0 root handle 1: htb default 1
 * tc class add dev ifb0 parent 1: classid 1:1 \
 *    htb rate 500kbit burst 6k quantum 1540
 * tc qdisc add dev ifb0 parent 1:1 handle 10: sfq perturb 10
 * 
 * tc qdisc add dev eth0 root handle 1: htb default 1
 * tc class add dev eth0 parent 1: classid 1:1 \
 *    htb rate 2000kbit burst 6k quantum 1540
 * tc qdisc add dev eth0 parent 1:1 handle 10: sfq perturb 10
 * \endcode
 * 
 * \note Setting up emulation proproties may involve various detials. \n\n
 *       e.g., to get the desired throughput, a fair queue is often necessary,
 *       however in Linux, the stochastic fair queue (sfq) and the netem are
 *       both classless qdiscs, which means that they cannot work together on a
 *       certain interface. \n\n
 *       Therefore with netem/sfq/htb, it's impossible to emulate rate limit and
 *       delay at the same time. However other solutions may still exist.
 *
 * Thus the Emulator class is designed to wrap the underlying implementation
 * details. 
 * 
 * Currently, two concrete emulator classes are available, BasicEmulator and 
 * NistnetEmulator. Each concrete emulator class represents a **solution**, and
 * every concrete emulator class shares a same interface.
 *
 * \code
 * BasicEmulator emulator("10.0.0.1", 3201);
 * emulator.setParam("TxRate", "200kbps");
 * emulator.setParam("RxRate", "2000kbps");
 * emulator.commit();
 * \endcode
 *
 * Click here for more information
 *  - http://www.linuxfoundation.org/collaborate/workgroups/networking/netem
 *  - http://snad.ncsl.nist.gov/itg/nistnet/
 */
class Emulator
{
protected:
    /**
     * \brief Emulation parameters
     */
    QMap<QString, QString> m_params;
    /**
     * \brief IPv4 address of the emulator daemon
     */
    QString m_addr;
    /**
     * \brief Port of the emulator daemon
     */
    quint16 m_port;
    
public:
    /**
     * \brief Initialize the base class
     * \param addr IPv4 address of the emulator daemon
     * \param port Port of the emulator daemon
     */
    Emulator(const QString &addr, quint16 port);
    
    /**
     * \brief Set emulation parameter
     * \param name The name of the parameter
     * \param value The value of the parameter
     */
    virtual void setParam(const QString &name, QString value);
    
    /**
     * \brief Commit emulation parameters
     *
     * **Protocol Definition**
     *  1. The emulator (client) connects to daemon
     *  2. The emulator (client) sends request
     *     Field        | Type                   | Description
     *     -------------|------------------------|------------
     *     length       | uint32                 | Length of the remaining part
     *     emulatorName | QString                | "Basic" or "NistNet"
     *     command      | QString                | "COMMIT"
     *     params       | QMap<QString, QString> | The parameters to commit
     *  3. The emulator daemon sends response
     *     Field        | Type                   | Description
     *     -------------|------------------------|------------
     *     length       | uint32                 | Length of the remaining part
     *     result       | bool                   | True for success, false otherwise
     *     description  | QString                | Detailed description
     *  4. The emulator (client) disconnects
     * 
     * \note If part of the parameters are invalid, the [result] will be false, 
     *       and emulator daemon will ignore the whole request.
     */
    virtual void commit() = 0;
    
    /**
     * \brief Reset emulation parameters
     *
     * **Protocol Definition**
     *  1. The emulator (client) connects to daemon
     *  2. The emulator (client) sends request
     *     Field        | Type                   | Description
     *     -------------|------------------------|------------
     *     length       | uint32                 | Length of the remaining part
     *     emulatorName | QString                | "Basic" or "NistNet"
     *     command      | QString                | "RESET"
     *  3. The emulator daemon sends response 
     *     Field        | Type                   | Description
     *     -------------|------------------------|------------
     *     length       | uint32                 | Length of the remaining part
     *     result       | bool                   | True for success, false otherwise
     *     description  | QString                | Detailed description
     *  4. The emulator (client) disconnects
     */
    virtual void reset() = 0;
};

#endif /* EMULATOR_H */

