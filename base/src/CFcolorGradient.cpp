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
#include "CFcolorGradient.h"

#include <algorithm>

#include <cmath>
#include <assert.h>

CFcolorGradient::CFcolorGradient( Interpolation interpolation ) : m_interpolation( interpolation )
{
    insertKey( Key( 0.0, CFcolor( 0.00, 0.66, 1.0, CFcolor::kHSV ) ) );
    insertKey( Key( 0.5, CFcolor( 0.33, 0.66, 1.0, CFcolor::kHSV ) ) );
    insertKey( Key( 1.0, CFcolor( 0.66, 0.66, 1.0, CFcolor::kHSV ) ) );
}

CFcolor CFcolorGradient::evaluate( CFfloat position ) const
{
    if( m_interpolation == kConstant ) return constant( position );
    if( m_interpolation == kLinear   ) return linear  ( position );

    assert( m_interpolation == kSmooth );

    return smooth( position );
}

CFcolor CFcolorGradient::constant( CFfloat position ) const
{
    if( ! keyCount() ) return CFcolor();

    unsigned int nextKeyIdx( findNextKey( position ) );                             assert( nextKeyIdx <= keyCount() );

    if( nextKeyIdx ) return m_keys[ nextKeyIdx - 1 ].value();

    return m_keys[ 0 ].value();
}

CFcolor CFcolorGradient::linear( CFfloat position ) const
{
    if( keyCount() == 0 ) return CFcolor();

    unsigned int nextKeyIdx( findNextKey( position ) );                             assert( nextKeyIdx <= keyCount() );

    if( nextKeyIdx == keyCount() ) return m_keys[ nextKeyIdx - 1 ].value();
    if( nextKeyIdx == 0          ) return m_keys[ 0              ].value();

    unsigned int prevKeyIdx = nextKeyIdx - 1;

    CFfloat normalizedPosition = ( position                        - m_keys[ prevKeyIdx ].position() ) /
                                 ( m_keys[ nextKeyIdx ].position() - m_keys[ prevKeyIdx ].position() );

    return m_keys[ prevKeyIdx ].value().Blend( m_keys[ nextKeyIdx ].value(), normalizedPosition );
}

CFcolor CFcolorGradient::smooth( CFfloat position ) const
{
    if( keyCount() == 0 ) return CFcolor();

    unsigned int nextKeyIdx( findNextKey( position ) );                             assert( nextKeyIdx <= keyCount() );

    if( nextKeyIdx == keyCount() ) return m_keys[ nextKeyIdx - 1 ].value();
    if( nextKeyIdx == 0          ) return m_keys[ 0              ].value();

    unsigned int prevKeyIdx = nextKeyIdx - 1;

    CFfloat normalizedPosition = ( position                        - m_keys[ prevKeyIdx ].position() ) /
                                 ( m_keys[ nextKeyIdx ].position() - m_keys[ prevKeyIdx ].position() );

    normalizedPosition = 0.5 - cos( normalizedPosition * 3.14159 ) * 0.5;

    return m_keys[ prevKeyIdx ].value().Blend( m_keys[ nextKeyIdx ].value(), normalizedPosition );
}

void CFcolorGradient::saveTo( std::ostream& stream ) const
{
    stream << m_interpolation << " ";

    CFkey1DList<CFcolor>::saveTo( stream );
}

void CFcolorGradient::loadFrom( std::istream& stream )
{
    int enumValue;

    stream >> enumValue;

         if( enumValue == 0 ) m_interpolation = kConstant;
    else if( enumValue == 1 ) m_interpolation = kLinear;
    else if( enumValue == 2 ) m_interpolation = kSmooth;
    else                      assert( 0 );

    CFkey1DList<CFcolor>::loadFrom( stream );
}
