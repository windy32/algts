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

#include "random-variable-factory.h"

RandomVariable *RandomVariableFactory::create(const QString &param)
{
    QRegExp u0("^Uniform$"); // case sensitive by default
    QRegExp u2("^Uniform ([0-9]+),\\s*([0-9+])$");
    
    QRegExp e1("^Exponential ([0-9]+)$");
    QRegExp e2("^Exponential ([0-9]+),\\s*([0-9]+)$");

    QRegExp p1("^Pareto ([0-9]+)$");
    QRegExp p2("^Pareto ([0-9]+),\\s*([0-9]+)$");
    QRegExp p3("^Pareto ([0-9]+),\\s*([0-9]+),\\s*([0-9]+)$");
    
    if( u0.indexIn(param) != -1 )
    {
        return new UniformVariable();
    }
    else if( u2.indexIn(param) != -1 )
    {
        return new UniformVariable(
            u2.cap(1).toDouble(), u2.cap(2).toDouble());
    }
    else if( e1.indexIn(param) != -1 )
    {
        return new ExponentialVariable(
            e1.cap(1).toDouble());
    }
    else if( e2.indexIn(param) != -1 )
    {
        return new ExponentialVariable(
            e2.cap(1).toDouble(), e2.cap(2).toDouble());
    }
    else if( p1.indexIn(param) != -1 )
    {
        return new ParetoVariable(
            p1.cap(1).toDouble());
    }
    else if( p2.indexIn(param) != -1 )
    {
        return new ParetoVariable(
            p2.cap(1).toDouble(), p2.cap(2).toDouble());
    }
    else if( p3.indexIn(param) != -1 )
    {
        return new ParetoVariable(
            p3.cap(1).toDouble(), p3.cap(2).toDouble(), p3.cap(3).toDouble());
    }
    else
    {
        LOG_ERROR(QString("Invalid random variable %1").arg(param));
        return NullVariable::getInstance();
    }
}