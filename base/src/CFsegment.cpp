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
#include "CFsegment.h"

#include <limits>

//  Intersection of two line segments
//  ---------------------------------
//
//  The return value of the function is
//
//      0 if there is no intersection
//      1 if there is a unique intersection
//      2 if the two segments overlap and the intersection set is a segment itself
//
//  The return value also indicates the number of valid points of the "where" result
//
int CFsegment::Collide( const CFsegment& with, CFsegment& where ) const
{
    const CFfloat sqrEpsilon = std::numeric_limits<CFfloat>::epsilon();

    CFpoint E = with - ( *this );

    CFfloat kross       = m_direction[0] * with.m_direction[1] - m_direction[1] * with.m_direction[0];
    CFfloat sqrKross    = kross * kross;
    CFfloat sqrLen0     = m_direction[0] * m_direction[0] + m_direction[1] * m_direction[1];
    CFfloat sqrLen1     = with.m_direction[0] * with.m_direction[0] + with.m_direction[1] * with.m_direction[1];

    if( sqrKross > sqrEpsilon * sqrLen0 * sqrLen1 )
    {
        // lines of the segments are not parallel
        //
        CFfloat s = ( E[0] * with.m_direction[1] - E[1] * with.m_direction[0] ) / kross;
        if( s < 0 || s > 1 )
        {
            return 0;
        }

        CFfloat t = ( E[0] * m_direction[1] - E[1] * m_direction[0] ) / kross;
        if( t < 0 || t > 1 )
        {
            return 0;
        }
    
        // intersection of lines is a point
        //
        where = ( *this ) + m_direction * s;

        return 1;
    }

    // lines of the segments are parallel
    //
    CFfloat sqrLenE = E[0] * E[0] + E[1] * E[1];
    kross = E[0] * m_direction[1] - E[1] * m_direction[0];
    sqrKross = kross * kross;

    if( sqrKross > sqrEpsilon * sqrLen0 * sqrLenE )
    {
        // lines of the segments are different
        return 0;
    }

    // Lines of the segments are the same
    // Test for overlap of segments
    //
    CFfloat s0 = ( m_direction * E ) / sqrLen0;
    CFfloat s1 = s0 + ( m_direction * with.m_direction ) / sqrLen0;
    CFfloat inter1;
    CFfloat inter2 = 0.0;

    CFfloat smin = s0 < s1 ? s0 : s1;
    CFfloat smax = s0 > s1 ? s0 : s1;

    int imax = IntervalCollide( 0.0, 1.0, smin, smax, inter1, inter2 );

    if( imax > 0 )      where = ( *this ) + m_direction * inter1;
    if( imax > 1 )      where.m_direction = ( *this ) + m_direction * inter2;

    return imax;
}


//  Intersection of two intervals [u0, u1] and [v0, v1], where u0 < u1 and v0 < v1
//  -----------------------------
//
//  The return value is
//
//      0 if the intervals do not intersect
//      1 if they intersect at a single point, in which case w[0] contains that point
//      2 if they intersect in an interval whose end points are inter1 and inter2
//
int CFsegment::IntervalCollide( CFfloat u0, CFfloat u1, CFfloat v0, CFfloat v1, CFfloat& inter1, CFfloat& inter2 )
{
    if( u1 < v0 || u0 > v1 )    return 0;

    if( u1 > v0 )
    {
        if( u0 < v1 )
        {
            if( u0 < v0 )   inter1 = v0;    else    inter1 = u0;
            if( u1 > v1 )   inter2 = v1;    else    inter2 = u1;

            return 2;
        }
        else
        {
            inter1 = u0; return 1;
        }
    }
    else
    {
        inter1 = u1; return 1;
    }
}
