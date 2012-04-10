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

#ifndef RANDOM_VARIABLE_H
#define RANDOM_VARIABLE_H

#include "common.h"
#include "rng-stream.h"

/**
 * \brief The base class of all random variables
 *
 * Note: The underlying random number generation method used in algts is the
 * RngStream code by Pierre L'Ecuyer at the University of Montreal.
 */
class RandomVariable
{
public:
	RandomVariable();

    /**
     * \brief Returns a random double from the underlying distribution
     * \return A floating point random value
     */
	virtual double getValue() = 0;
	
    /**
     * \brief Returns a random integer integer from the underlying distribution
     * \return Integer cast of RandomVariable::getValue
     */
	virtual quint32 getInteger() = 0;
    
    /**
     * \brief Returns the min integer that the random variable can generate
     * \return The min integer that the random variable can generate
     */
    virtual quint32 getMin() = 0;

    /**
     * \brief Returns the max integer that the random variable can generate
     * \return The max integer that the random variable can generate
     */
    virtual quint32 getMax() = 0;
	
    //virtual void serialize(QDataStream *pStream) = 0;

protected:
    /**
     * \brief The random variable stream used by all concrete random variables
     */
	RngStream *m_generator;
};

/**
 * \brief The null random variable
 *
 * A null variable object is used as a replacement of a NULL pointer to 
 * random variable objects so that access violations could be avoided.
 *
 * A null variable is returned when user calls RandomVariableFactory::create
 * with invalid parameters.
 *
 * \code
 * RandomVariable *rv = RandomVariableFactory::create("umiform 10 20");
 * rv->getValue(); // will return 0.0 and generate a warning at the same time
 * \endcode
 * 
 * Null variable objects also appears in some other cases, when no concrete
 * random variable is actually available. For example, when user trys to expand
 * a task with random variable attributes without specifying those variables:
 * 
 * \code
 * Scenario s(12345, 60); // seed & length
 *
 * s.addUser("Harry");
 * s.addTask(new TcpEchoTask(23));
 * s.task().setAttribute("InputSize", "Uniform 10, 20");
 * s.task().setAttribute("EchoSize", "Uniform 10, 100");
 * // The Interval attribute of the TcpEchoTask is not specified
 * 
 * ...
 * s.exec(); // The Interval attribute is null, thus a warning will be generated
 * \endcode
 * 
 */
class NullVariable : public RandomVariable
{
private:
    static NullVariable *m_instance;
    
	NullVariable();

public:
    /**
     * \brief Get an instance of the null variable
     * \return A pointer to the null variable
     */
    static NullVariable* getInstance();
    
    /**
     * \brief Returns a floating point value
     * \return A floating point value of zero
     */
	virtual double getValue();

    /**
     * \brief Returns an integer
     * \return An integer value of zero
     */
	virtual quint32 getInteger();
    
    virtual quint32 getMin();
    virtual quint32 getMax();
};

/**
 * \brief Uniform distributed random variable
 *
 * This class supports the creation of objects that return random numbers from
 * a fixed uniform distribution.  It also supports the generation of single
 * random numbers from various uniform distributions.
 *
 * The low end of the range is always included and the high end of the range is
 * always excluded.
 * \code
 * UniformVariable x (0,10);
 * x.getValue ();  //will always return numbers [0,10)
 * \endcode
 */
class UniformVariable : public RandomVariable
{
public:
    /**
     * Creates a uniform random number generator in the range [0.0 .. 1.0)
     */
	UniformVariable();
	
    /**
     * Creates a uniform random number generator with the specified range
     * \param s Low end of the range
     * \param l High end of the range
     */
	UniformVariable(double s, double l);

    /**
     * \brief Returns a floating point value from the uniform distribution
     * \return A floating point random value
     */
	virtual double getValue();

    /**
     * \brief Returns an integer from the uniform distribution
     * \return Integer cast of UniformVariable::getValue
     */
	virtual quint32 getInteger();

    /**
     * \brief Returns a random double with the specified range
     * \param s Low end of the range
     * \param l High end of the range
     * \return A floating point random value
     */
	double getValue(double s, double l);

    /**
     * \brief Returns a random unsigned integer from the interval [s,l] 
     *        including both ends.
     * \param s Low end of the range
     * \param l High end of the range
     * \return A random unsigned integer value.
     */
	quint32 getInteger(quint32 s, quint32 l);
    
    virtual quint32 getMin();
    virtual quint32 getMax();

private:
	double m_min;
	double m_max;
};

/**
 * \brief Exponentially distributed random variable
 *
 * This class supports the creation of objects that return random numbers from
 * a fixed exponential distribution.  It also supports the generation of single
 * random numbers from various exponential distributions.
 *
 * The probability density function of an exponential variable is defined over
 * the interval [0, +inf) as: \f$ \alpha  e^{-\alpha x} \f$ where 
 * \f$ \alpha = \frac{1}{mean} \f$
 *
 * The bounded version is defined over the interval [0,b] as:
 * \f$ \alpha  e^{-\alpha x} \quad x \in [0,b] \f$.
 * Note that in this case the true mean is
 * \f$ 1/\alpha - b/(e^{\alpha \, b}-1) \f$
 *
 * \code
 * ExponentialVariable x(3.14);
 * x.getValue ();  //will always return with mean 3.14
 * \endcode
 *
 */

class ExponentialVariable : public RandomVariable
{
public:
    /**
     * Constructs an exponential random variable  with a mean
     * value of 1.0.
     */
   	ExponentialVariable();

    /**
     * \brief Constructs an exponential random variable with a specified mean
     * \param m Mean value for the random variable
     */
	explicit ExponentialVariable(double m);
	
    /**
     * \brief Constructs an exponential random variable with specified mean and
     *        upper limit
     * \param m Mean value of the random variable
     * \param b Upper bound on returned values
     *
     * \note Since exponential distributions can theoretically return unbounded 
     *       values, it is sometimes useful to specify a fixed upper limit. Note
     *       however when the upper limit is specified, the true mean of the
     *       distribution is slightly smaller than the mean value specified: 
     *       \f$ m - b/(e^{b/m}-1) \f$.
     */
	ExponentialVariable(double m, double b);

    /**
     * \brief Returns a floating point value from the exponential distribution
     * \return A floating point random value
     */
    virtual double getValue();

    /**
     * \brief Returns an integer from the exponential distribution
     * \return Integer cast of ExponentialVariable::getValue
     */
    virtual quint32 getInteger();
    
    virtual quint32 getMin();
    virtual quint32 getMax();

private:
	double m_mean;  // Mean value of RV
	double m_bound; // Upper bound on value (if non-zero)
};

/**
 * \brief Pareto distributed random variable
 *
 * This class supports the creation of objects that return random numbers
 * from a fixed pareto distribution.  It also supports the generation of
 * single random numbers from various pareto distributions.
 *
 * The probability density function is defined over the range [\f$x_m\f$,+inf) 
 * as: \f$ k \frac{x_m^k}{x^{k+1}}\f$ where \f$x_m > 0\f$ is called the location
 * parameter and \f$ k > 0\f$ is called the pareto index or shape.
 *
 * The parameter \f$ x_m \f$ can be infered from the mean and the parameter 
 * \f$ k \f$ with the equation \f$ x_m = mean \frac{k-1}{k},  k > 1\f$.
 *
 * \code
 * ParetoVariable x (3.14);
 * x.getValue ();  //will always return with mean 3.14
 * \endcode
 */

class ParetoVariable : public RandomVariable
{
public:
    /**
     * \brief Constructs a pareto random variable with a mean of 1 and a shape
     *        parameter of 1.5
     */
	ParetoVariable ();
	
    /**
     * \brief Constructs a pareto random variable with specified mean and shape
     *        parameter of 1.5
     * \param m Mean value of the distribution
     */
	explicit ParetoVariable (double m);
	
    /**
     * \brief Constructs a pareto random variable with the specified mean value
     *        and shape parameter. Beware, s must be strictly greater than 1.
     * \param m Mean value of the distribution
     * \param s Shape parameter for the distribution
     */
	ParetoVariable (double m, double s);
	
    /**
     * \brief Constructs a pareto random variable with the specified mean value,
     *        shape (alpha), and upper bound. Beware, s must be strictly greater
     *        than 1.
     * \param m Mean value
     * \param s Shape parameter
     * \param b Upper limit on returned values
     *
     * \note Since pareto distributions can theoretically return unbounded
     *       values, it is sometimes useful to specify a fixed upper limit. 
     *       Note however when the upper limit is specified, the true mean of
     *       the distribution is slightly smaller than the mean value specified.
     */
	ParetoVariable (double m, double s, double b);

    /**
     * \brief Returns a floating point value from the pareto distribution
     * \return A floating point random value
     */
	virtual double getValue();
	
    /**
     * \brief Returns an integer from the Pareto distribution
     * \return Integer cast of ParetoVariable::getValue
     */
	virtual quint32 getInteger();

    virtual quint32 getMin();
    virtual quint32 getMax();

private:
	double m_scale; // Scale value of RV
	double m_shape; // Shape parameter
	double m_bound; // Upper bound on value (if non-zero)
};

#endif /* RANDOM_VARIABLE_H */

