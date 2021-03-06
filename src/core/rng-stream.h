// Copyright (C) 2001 Pierre L'Ecuyer (lecuyer@iro.umontreal.ca)
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
//
// Modified for ns-3 by: Rajib Bhattacharjea<raj.b@gatech.edu>
// Modified for algts by: Gao Fengyu<feng32tc@gmail.com>

#ifndef RNGSTREAM_H
#define RNGSTREAM_H

#include "common.h"

/**
 * \ingroup RandomVariable
 * \brief The MRG32k3a combined multiple-recursive random number generator
 *
 * The details of this class are explained in:
 * http://www.iro.umontreal.ca/~lecuyer/myftp/papers/streams00.pdf
 */
class RngStream
{
public:  //public api
    /**
     * \brief Initialize the generator
     */
	RngStream();
    /**
     * \brief Initialize the generator
     */
	RngStream(const RngStream&);
	
    /**
     * \brief Set the antithesis flag of the generator.
     * 
     * If the flag is set true, RngStream::RandU01 will return 
     * (1 - the_original_value)
     */
	void SetAntithetic(bool a);	
	/**
     * \brief Increase precision
     */
	void IncreasedPrecis(bool incp);
	
	/**
     * \brief Generate a random number in the range [0.0, 1.0)
     */
	double RandU01();

public: //public static api
    /**
     * \brief Set the global seed used by all RngStream objects
     */
	static bool SetPackageSeed(quint32 seed);
    /**
     * \brief Set the global seed used by all RngStream objects
     */
	static bool SetPackageSeed(const quint32 seed[6]);
	
    /**
     * \brief Returns the global seed used by all RngStream objects
     */
	static void GetPackageSeed(quint32 seed[6]);
	
    /**
     * \brief Check if the global seed is valid
     */
	static bool CheckSeed(quint32 seed);
    /**
     * \brief Check if the global seed is valid
     */
	static bool CheckSeed(const quint32 seed[6]);

private: // members
	double Cg[6], Bg[6], Ig[6];
	bool anti, incPrec;
	double U01();
	double U01d();

private: // static members
	static double nextSeed[6];
};

#endif /* RNGSTREAM_H */

