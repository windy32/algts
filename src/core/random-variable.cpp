// Copyright (c) 2006 Georgia Tech Research Corporation
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
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
// Author: Hadi Arbabi<marbabi@cs.odu.edu>
//
// Modified for algts by: Gao Fengyu<feng32tc@gmail.com>

#include "random-variable.h"
#include <math.h>

// Random Variable
RandomVariable::RandomVariable()
{
    m_generator = new RngStream();
}

// Null variable
NullVariable *NullVariable::m_instance = NULL;

NullVariable::NullVariable()
{
}

NullVariable* NullVariable::getInstance()
{
    if( m_instance == NULL )
    {
        m_instance = new NullVariable();
    }
    return m_instance;
}

double NullVariable::getValue()
{
    LOG_WARN("Call to NullVariable::getValue()");
    return 0.0;
}

quint32 NullVariable::getInteger()
{
    LOG_WARN("Call to NullVariable::getInteger()");
    return 0;
}

quint32 NullVariable::getMin()
{
    return 0;
}

quint32 NullVariable::getMax()
{
    return 0;
}

// Uniform variable
UniformVariable::UniformVariable()
    : m_min(0), 
      m_max(1.0)
{
}

UniformVariable::UniformVariable(double s, double l)
    : m_min(s), 
      m_max(l)
{
}

double UniformVariable::getValue()
{
    return m_min + m_generator->RandU01() * (m_max - m_min);
}

quint32 UniformVariable::getInteger()
{
    return static_cast<quint32>(getValue());
}

quint32 UniformVariable::getMin()
{
    return static_cast<quint32>(m_min);
}

quint32 UniformVariable::getMax()
{
    return static_cast<quint32>(m_max);
}

double UniformVariable::getValue(double s, double l)
{
    return s + m_generator->RandU01() * (l - s);
}

quint32 UniformVariable::getInteger(quint32 s, quint32 l)
{
    return static_cast<quint32>(getValue(s, l + 1));
}

// Exponential Variable
ExponentialVariable::ExponentialVariable()
    : m_mean(1.0), 
      m_bound(0)
{
}

ExponentialVariable::ExponentialVariable(double m)
    : m_mean(m), 
      m_bound(0)
{
}


ExponentialVariable::ExponentialVariable(double m, double b)
    : m_mean(m), 
      m_bound(b)
{
}

double ExponentialVariable::getValue()
{
    while(true)
    {
        double r = -m_mean * log(m_generator->RandU01());
        if( m_bound == 0 || r <= m_bound )
        {
            return r;
        }
        // otherwise, try again
    }
}

quint32 ExponentialVariable::getInteger()
{
    return static_cast<quint32>(getValue());
}

quint32 ExponentialVariable::getMin()
{
    return 0;
}

quint32 ExponentialVariable::getMax()
{
    return (m_bound == 0) ? 0xFFFFFFFF : static_cast<quint32>(m_bound);
}

// Pareto Variable

ParetoVariable::ParetoVariable()
    : m_scale(0.5 / 1.5), 
      m_shape(1.5), 
      m_bound(0)
{

}

ParetoVariable::ParetoVariable(double m)
    : m_scale(m * 0.5 / 1.5), 
      m_shape(1.5), 
      m_bound(0)
{
}


ParetoVariable::ParetoVariable(double m, double s)
    : m_scale(m * (s - 1.0) / s), 
      m_shape(s), 
      m_bound(0)
{
}


ParetoVariable::ParetoVariable(double m, double s, double b)
    : m_scale(m * (s - 1.0) / s), 
      m_shape(s), 
      m_bound(b)
{
}

double ParetoVariable::getValue()
{
    while(true)
    {
        double r = (m_scale * (1.0 / pow(m_generator->RandU01(), 1.0 / m_shape)));
        if (m_bound == 0 || r <= m_bound)
        {
            return r;
        }
        // otherwise, try again
    }
}

quint32 ParetoVariable::getInteger()
{
    return static_cast<quint32>(getValue());
}

quint32 ParetoVariable::getMin()
{
    return static_cast<quint32>(m_scale);
}

quint32 ParetoVariable::getMax()
{
    return (m_bound == 0) ? 0xFFFFFFFF : static_cast<quint32>(m_bound);
}
