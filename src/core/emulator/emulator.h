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
 * Here's a typical script that sets the properties above with netem.
 *
 * \code
 * tc qdisc add dev eth0 root handle 1: tbf rate 10mbit buffer 1600 limit 3000
 * tc qdisc add dev eth0 parent 1: handle 10: netem limit 10000
 *
 * modprobe ifb
 * ip link set dev ifb0 up
 * tc qdisc add dev eth0 ingress
 * tc filter add dev eth0 parent ffff: protocol ip pref 10 u32 \ 
 *    match u32 0 0 flowid 1:1 action mirred egress redirect dev ifb0
 * tc qdisc add dev ifb0 root handle 1: tbf rate 10mbit buffer 1600 limit 3000
 * tc qdisc add dev ifb0 parent 1: handle 10: netem limit 1000
 * \endcode
 * 
 * As is shown above, setting up emulation proproties involves not only the
 * netem qdisc, but also a tbf qdisc for rate control, an ifb device dealing
 * with incoming packet, as well as Linux commands like *modprobe*. If the
 * NIST Net emulator or something else is used, the scirpt could be quite
 * different. Note that different **solutions** may also lead to different
 * emulation capabilities. 
 *
 * Therefore, the Emulator class is designed to wrap the underlying
 * implementation details. Currently, two concrete emulator classes are
 * available, NetemEmulator and NistnetEmulator. Each concrete emulator class
 * represents a **solution**, and every concrete emulator class shares a same
 * interface.
 *
 * \code
 * NetemEmulator emulator("10.0.0.1", 3201);
 * emulator.setParam("TxRate", "200kbps");
 * emulator.setParam("RxRate", "2000kbps");
 * emulator.setParam("TxDelay", "20ms");
 * emulator.setParam("RxDelay", "20ms");
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
     *     emulatorName | QString                | "NetEm" or "NistNet"
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
     *     emulatorName | QString                | "NetEm" or "NistNet"
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

