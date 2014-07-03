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
#include "CFpoint.h"
#include "CFtypes.h"

#include <cmath>
#include "GL/gl.h"

CFfloat CFpoint::Distance2( const CFpoint& point ) const
{
    return ( point - (*this) ).Length2();
}

CFfloat CFpoint::Distance( const CFpoint& point ) const
{
    return sqrt( Distance2( point ) );
}

CFfloat CFpoint::Angle( const CFpoint& vect ) const
{
    CFfloat angle = acos( ( *this * vect ) / ( Length() * vect.Length() ) );

    if( ( *this ^ vect ) < 0.0 )
    {
        return -angle;
    }

    return angle;
}

CFfloat CFpoint::Length2( void ) const
{
    return ( m_x * m_x + m_y * m_y );
}
CFfloat CFpoint::Length( void ) const
{
    return sqrt( Length2() );
}

CFpoint& CFpoint::Normalize( void )
{
    (*this) /= Length();

    return *this;
}

////////////////////////////////////////////////////////////
//  m_x = angle
//  m_y = length
//
void CFpoint::ToPolar( void )
{
    CFfloat _length = Length();

    m_x = acos( m_x / _length );

    if( m_y < 0.0 )
        m_x = PIx2 - m_x;

    m_y = _length;
}

void CFpoint::ToCartesian( void )
{
    CFfloat _angle = m_x;

    m_x = cos( _angle ) * m_y;
    m_y = sin( _angle ) * m_y;
}

void CFpoint::Rotate( const CFfloat angle )
{
    ToPolar();
    m_x += angle;
    ToCartesian();
}

void CFpoint::Draw( void ) const
{
    glVertex2f( m_x, m_y );
}

/////////// OPERATORS ////////

void CFpoint::operator+=( const CFpoint &point )
{
    m_x += point[0]; m_y += point[1];
}
void CFpoint::operator-=( const CFpoint &point )
{
    m_x -= point[0]; m_y -= point[1];
}

void CFpoint::operator/=( const CFfloat factor )
{
    m_x /= factor; m_y /= factor;
}
void CFpoint::operator*=( const CFfloat factor )
{
    m_x *= factor; m_y *= factor;
}
void CFpoint::operator/=( const CFpoint &byAxisFactors )
{
    m_x /= byAxisFactors[0]; m_y /= byAxisFactors[1];
}

void CFpoint::operator*=( const CFpoint &byAxisFactors )
{
    m_x *= byAxisFactors[0]; m_y *= byAxisFactors[1];
}

CFpoint CFpoint::operator+( const CFpoint &point ) const
{
    return CFpoint( m_x+point[0], m_y+point[1] );
}

CFpoint CFpoint::operator*( const CFfloat factor ) const
{
    return CFpoint( m_x*factor, m_y*factor );
}

CFpoint CFpoint::operator/( const CFfloat factor ) const
{
    return CFpoint( m_x/factor, m_y/factor );
}
