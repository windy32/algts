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

#ifndef RANDOM_VARIABLE_FACTORY_H
#define RANDOM_VARIABLE_FACTORY_H

#include "common.h"
#include "random-variable.h"

/**
 * \brief Factory for creating random variable objects
 * 
 * Examples:
 * \code
 * RandomVariable *v1, *v2, *v3, *v4, *v5, *v6, *v7;
 *
 * // Create a uniform variable in the range [0.0, 1.0)
 * v1 = RandomVariableFactory::create("Uniform");
 *
 * // Create a uniform variable in the range [500, 10000)
 * v2 = RandomVariableFactory::create("Uniform 500, 10000");
 *
 * // Create a exponential variable with mean value 2000
 * v3 = RandomVariableFactory::create("Exponential 2000");
 *
 * // Create a exponential variable with mean value 20 and upper bound 1000
 * v4 = RandomVariableFactory::create("Exponential 20, 1000");
 *
 * // Create a pareto variable with mean value 1000 and 
 * // default shape parameter 1.5
 * v5 = RandomVariableFactory::create("Pareto 1000");
 *
 * // Create a pareto variable with mean value 600 and shape parameter 1.4
 * v6 = RandomVariableFactory::create("Pareto 600, 1.4");
 *
 * // Create a pareto variable with mean value 500, shape parameter 1.5 and
 * // upper bound 10000
 * v7 = RandomVariableFactory::create("Pareto 500, 1.5, 10000");
 * 
 * \endcode
 */
class RandomVariableFactory
{
public:
    /**
     * \brief Create a random variable object
     * \param param The type of the random variable along with its parameters
     * \return The pointer to the created random variable object
     * \note If parameter is invalid, a pointer to a null variable will be
     *       returned.
     */
    static RandomVariable *create(const QString &param);
};

#endif /* RANDOM_VARIABLE_FACTORY_H */

