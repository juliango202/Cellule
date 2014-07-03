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
#include "CFcircle.h"

CFcircle::CFcircle( const CFpoint& point1, const CFpoint& point2, const CFpoint& point3 )
{
    CFfloat d1  = ( point3 - point1 ) * ( point2 - point1 );
    CFfloat d2  = ( point3 - point2 ) * ( point1 - point2 );
    CFfloat d3  = ( point1 - point3 ) * ( point2 - point3 );

    CFfloat c1  = d2 * d3;
    CFfloat c2  = d3 * d1;
    CFfloat c3  = d1 * d2;

    CFfloat c   = c1 + c2 + c3;

    //  Center
    //
    *this = ( point1 * ( c2 + c3 ) + point2 * ( c3 + c1 ) + point3 * ( c1 + c2 ) ) / ( 2.0f * c );

    //  Radius
    //
    m_radius2   = 0.25f * ( ( d1 + d2 ) * ( d2 + d3 ) * ( d3 + d1 ) / c );
}

CFcircle::CFcircle( const CFcircle& point1, const CFcircle& point2, const CFcircle& point3 )
{
    const CFfloat&  Ax = point1[0];
    const CFfloat&  Ay = point1[1];

    const CFfloat&  Bx = point2[0];
    const CFfloat&  By = point2[1];

    const CFfloat&  Cx = point3[0];
    const CFfloat&  Cy = point3[1];

    CFfloat An = Ax * Ax + Ay * Ay - point1.GetRadius2();
    CFfloat Bn = Bx * Bx + By * By - point2.GetRadius2();
    CFfloat Cn = Cx * Cx + Cy * Cy - point3.GetRadius2();

    CFfloat d = 2.0f * ( Ax*(By-Cy) + Bx*(Cy-Ay) + Cx*(Ay-By) );

    m_x =   ( (By-Cy)*An + (Cy-Ay)*Bn + (Ay-By)*Cn ) / d;
    m_y = - ( (Bx-Cx)*An + (Cx-Ax)*Bn + (Ax-Bx)*Cn ) / d;

    m_radius2   = ( m_x - Ax ) * ( m_x - Ax ) + ( m_y - Ay ) * ( m_y - Ay ) - point1.GetRadius2();
}

CFfloat CFcircle::Distance( const CFcircle& circle ) const
{
    return CFpoint::Distance( circle ) - GetRadius() - circle.GetRadius(); 
}

CFfloat CFcircle::PowDist( const CFcircle& circle ) const
{
    return CFpoint::Distance2( circle ) - GetRadius2() - circle.GetRadius2(); 
}

bool CFcircle::Collide( const CFpoint& p1, const CFpoint&   p2, bool segment ) const
{
    // http://astronomy.swin.edu.au/~pbourke/geometry/sphereline/
    //
    CFfloat     dx  = p2[0] - p1[0];
    CFfloat     dy  = p2[1] - p1[1];
    CFfloat     a   = dx * dx + dy * dy;
    CFfloat     b   = 2 * ( dx * ( p1[0] - m_x ) + dy * ( p1[1] - m_y ) );
    CFfloat     c   = m_x * m_x + m_y * m_y + p1[0] * p1[0] + p1[1] * p1[1] - 2 * ( m_x * p1[0] + m_y * p1[1] ) - m_radius2;

    CFfloat     d   = b * b - 4 * a * c;

    if( d <= 0.0 )  return false;

    if( ! segment ) return true;

    CFfloat     u1  = ( - b - sqrt( d ) ) / 2 * a;
    CFfloat     u2  = ( - b + sqrt( d ) ) / 2 * a;

    assert( u2 > u1 );

    if( u1 < 0.0 )
    {
        if( u2 < 0.0 )
            return false;

        return true;
    }
    else
    {
        if( u1 > 1.0 )
            return false;

        return true;
    }
}

void CFcircle::Draw() const
{
    float a;
    float r = sqrt( m_radius2 );

    glBegin( GL_LINE_LOOP );

    for( a = 0.0; a < 6.28318; a += 0.314159 )
    {
        glVertex2f( m_x + cos( a ) * r, m_y + sin( a ) * r );
    }

    glEnd();
}
