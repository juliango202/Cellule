//  (C) Copyright 2005-2008 Benjamin Godon and contributors.
//
//  This file is part of Cellule.
//
//  Cellule is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  Cellule is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Cellule; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef __CFsurfaceintegral_h__
#define __CFsurfaceintegral_h__

#include "CFcircle.h"
#include "CFcolor.h"
#include "CFenum.h"

#include <gsl/gsl_qrng.h>


struct CFsample {

    CFcolor     color;
    CFfloat     height;
    CFcolor     transparency;
    CFcolor     specular;
    CFfloat3    normal;
};

//! \brief  Integrate a shading function over a surface by averaging random samples until convergence
//! \author JuL
//! \date   2008 April
//!
class CFsurfaceIntegral
{

public:

                    CFsurfaceIntegral   ( const CFcircle& area, 
                                          const CFenum&   antialisingType,
                                          unsigned int    maxSamples        );

                    ~CFsurfaceIntegral  ( void                              );

    const CFpoint&  NextPoint           ( void                              );
    bool            End                 ( void                              );

    CFsample&       ShadingResult       ( void                              ) { return m_sample; }
    const CFsample& Result              ( void                              ) { return m_result; }

protected:

    void            Evaluate            ( void                              );

    CFpoint         m_center;
    CFfloat         m_radius;

    CFpoint         m_point;
    CFsample        m_sample;
    CFsample        m_result;

    gsl_qrng*       m_generator;
    double          m_value[2];

    unsigned int    m_maxSamples;

    //
    // Stat
    unsigned int    m_nbSamples;
    CFfloat         m_heights;
    CFfloat3        m_fourPoints[4];
    CFfloat3        m_sum;
    CFfloat3        m_previousAvg;
};

#endif //__CFsurfaceintegral_h__


