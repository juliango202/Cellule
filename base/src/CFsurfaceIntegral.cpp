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
#include "CFsurfaceIntegral.h"

const unsigned int  SAMPLES_PER_STEP    = 3;        // Must be superior or equal to 2
const float         EPS_ABSOLUTE_ERROR  = 0.05;
const float         EPS_RELATIVE_ERROR  = 0.005;

CFsurfaceIntegral::CFsurfaceIntegral( const CFcircle& area, const CFenum& antialisingType, unsigned int maxSamples ) :
    m_maxSamples(maxSamples), m_nbSamples(0), m_heights(0), m_sum(CFfloat3(0,0,0)), m_previousAvg(CFfloat3(0,0,0))
{
    m_radius = area.GetRadius();

    // antialiasing types -> { "kNone", "kUniform", "kAdaptive" }
    
    m_generator = gsl_qrng_alloc(gsl_qrng_sobol, 2);
    m_center = CFpoint( area[0] - 0.5 * m_radius, area[1] - 0.5 * m_radius );
}

CFsurfaceIntegral::~CFsurfaceIntegral( void )
{
    gsl_qrng_free(m_generator);
}

const CFpoint& CFsurfaceIntegral::NextPoint( void )
{
    gsl_qrng_get (m_generator, m_value);
    m_point[0] = m_center[0] + static_cast<CFfloat>(m_value[0]) * m_radius;
    m_point[1] = m_center[1] + static_cast<CFfloat>(m_value[1]) * m_radius;
    return m_point;
}

void CFsurfaceIntegral::Evaluate( void )
{
    assert( m_nbSamples >= 4 );

    // Compute normal
    CFfloat3 u( m_fourPoints[3] - m_fourPoints[0] );
    CFfloat3 v( m_fourPoints[1] - m_fourPoints[0] );

    m_result.normal[0] = u[1] * v[2] - u[2] * v[1];
    m_result.normal[1] = u[2] * v[0] - u[0] * v[2];
    m_result.normal[2] = u[0] * v[1] - u[1] * v[0];

    CFfloat norm = m_result.normal.norm();
    if( norm == 0.0 || m_result.normal[2] < 0 ) 
        m_result.normal = CFfloat3(0,0,1);
    else
        m_result.normal /= norm;

    // Compute color and height as average of samples
    m_result.color  = CFcolor(m_sum/m_nbSamples,CFcolor::kRGB);
    m_result.height = m_heights/m_nbSamples;
}

bool CFsurfaceIntegral::End( void )
{
    m_sum += m_sample.color.asRGB();
    m_heights += m_sample.height;

    if( m_nbSamples < 4 )
        m_fourPoints[m_nbSamples] = CFfloat3( m_point[0], m_point[1], m_sample.height );

    m_nbSamples++;
    if( m_nbSamples >= m_maxSamples )
    {
        Evaluate();
        return true;
    }

    if( m_nbSamples % SAMPLES_PER_STEP == 0 )
    {
        CFfloat3 currentAvg = m_sum/m_nbSamples;
        if( m_nbSamples > SAMPLES_PER_STEP ) // After first iteration: compute abs & rel error
        {
            float abs_error = (m_previousAvg - currentAvg).norm();
            float rel_error = abs_error / currentAvg.norm();
            if( abs_error < EPS_ABSOLUTE_ERROR && rel_error < EPS_RELATIVE_ERROR ) 
            {
                Evaluate();
                return true;
            }
        }

        m_previousAvg = currentAvg;
    }

    return false;
}
