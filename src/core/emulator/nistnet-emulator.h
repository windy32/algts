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

#ifndef NISTNET_EMULATOR_H
#define NISTNET_EMULATOR_H

#include "emulator.h"

/**
 * \brief The nistnet emulator
 * \note This class hasn't been implemented yet, and any call to commit/reset
 *       will do nothing but generate a warning
 */
class NistnetEmulator : public Emulator
{
public:
    /**
     * \brief Initialize the nistnet emulator
     * \param addr IPv4 address of the emulator daemon
     * \param port Port of the emulator daemon
     */
    NistnetEmulator(const QString &addr, quint16 port);
    
    virtual void commit();
    virtual void reset();
};

#endif /* NISTNET_EMULATOR_H */
