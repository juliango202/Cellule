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
#include "CFcolor.h"

const CFcolor CFcolor::kBlack( 0.0, 0.0, 0.0, kRGB );
const CFcolor CFcolor::kRed  ( 1.0, 0.0, 0.0, kRGB );
const CFcolor CFcolor::kGreen( 0.0, 1.0, 0.0, kRGB );
const CFcolor CFcolor::kBlue ( 0.0, 0.0, 1.0, kRGB );
const CFcolor CFcolor::kGray ( 0.5, 0.5, 0.5, kRGB );
const CFcolor CFcolor::kWhite( 1.0, 1.0, 1.0, kRGB );

CFcolor::CFcolor() : m_rgb( 0.0, 0.0, 0.0 )
{
}

CFcolor::CFcolor( const CFfloat3& components, Space space ) : m_rgb( components )
{
    if( space == kHSV ) m_rgb = HSVtoRGB( components );

    else assert( space == kRGB );
}

CFcolor::CFcolor( CFfloat component1, CFfloat component2, CFfloat component3, Space space )
{
    *this = CFcolor( CFfloat3( component1, component2, component3 ), space );
}

CFcolor::CFcolor( const QColor& color ) : m_rgb( color.redF(), color.greenF(), color.blueF() )
{
}

QColor CFcolor::asQColor( void ) const
{
    QColor color; color.setRgbF( m_rgb[0], m_rgb[1], m_rgb[2] ); return color;
}

void CFcolor::ClampRGB( CFfloat min, CFfloat max )
{
    if( m_rgb[0] < min ) m_rgb[0] = min;  if( m_rgb[0] > max ) m_rgb[0] = max;
    if( m_rgb[1] < min ) m_rgb[1] = min;  if( m_rgb[1] > max ) m_rgb[1] = max;
    if( m_rgb[2] < min ) m_rgb[2] = min;  if( m_rgb[2] > max ) m_rgb[2] = max;
}

CFcolor CFcolor::Blend( const CFcolor& color, CFfloat parameter ) const
{
    return CFcolor
    (
        parameter * color.m_rgb[0] + ( 1.0 - parameter ) * m_rgb[0],
        parameter * color.m_rgb[1] + ( 1.0 - parameter ) * m_rgb[1],
        parameter * color.m_rgb[2] + ( 1.0 - parameter ) * m_rgb[2],
        kRGB
    );
}

CFcolor CFcolor::operator*( const CFcolor& color ) const
{
    return CFcolor
    (
        m_rgb[0] * color.m_rgb[0],
        m_rgb[1] * color.m_rgb[1],
        m_rgb[2] * color.m_rgb[2],
        kRGB
    );
}

CFfloat3 CFcolor::RGBtoHSV( const CFfloat3& rgb ) const
{
    CFfloat min = rgb[0] < rgb[1] ? ( rgb[0] < rgb[2] ? rgb[0] : rgb[2] ) : ( rgb[1] < rgb[2] ? rgb[1] : rgb[2] );
    CFfloat max = rgb[0] > rgb[1] ? ( rgb[0] > rgb[2] ? rgb[0] : rgb[2] ) : ( rgb[1] > rgb[2] ? rgb[1] : rgb[2] );

    CFfloat delta = max - min;

    CFfloat3 hsv;

    hsv[2] = max;

    if( delta == 0.0 )
    {
        hsv[0] = 0.0;
        hsv[1] = 0.0;

        return hsv;
    }

    hsv[1] = delta / max;

    delta *= 6.0;

    if      ( rgb[0] == max ) hsv[0] = ( rgb[1] - rgb[2] ) / delta;
    else if ( rgb[1] == max ) hsv[0] = ( rgb[2] - rgb[0] ) / delta + 0.3333333333333333333;
    else                      hsv[0] = ( rgb[0] - rgb[1] ) / delta + 0.6666666666666666666;

    if( hsv[0] < 0.0  ) hsv[0] += 1.0;
    if( hsv[0] >= 1.0 ) hsv[0] -= 1.0;

    assert( hsv[0] >= 0.0 );
    assert( hsv[0] <  1.0 );

    assert( hsv[1] >= 0.0 );
    assert( hsv[1] <= 1.0 );

    return hsv;
}

CFfloat3 CFcolor::HSVtoRGB( const CFfloat3& hsv ) const
{
    CFfloat3 rgb;

    if( hsv[1] == 0.0 )
    {
        //  No saturation
        //  so it is gray
        //
        rgb[0] = hsv[2];
        rgb[1] = hsv[2];
        rgb[2] = hsv[2];

        return rgb;
    }

    CFfloat h6 = hsv[0] * 6.0;

    int     i = static_cast<int>( floor( h6 ) );
    CFfloat f = h6 - i;

    CFfloat p = hsv[2] * ( 1.0 - hsv[1]               );
    CFfloat q = hsv[2] * ( 1.0 - hsv[1] * f           );
    CFfloat t = hsv[2] * ( 1.0 - hsv[1] * ( 1.0 - f ) );

         if( i == 0 ) { rgb[0] = hsv[2]; rgb[1] = t;      rgb[2] = p;      }
    else if( i == 1 ) { rgb[0] = q;      rgb[1] = hsv[2]; rgb[2] = p;      }
    else if( i == 2 ) { rgb[0] = p;      rgb[1] = hsv[2]; rgb[2] = t;      }
    else if( i == 3 ) { rgb[0] = p;      rgb[1] = q;      rgb[2] = hsv[2]; }
    else if( i == 4 ) { rgb[0] = t;      rgb[1] = p;      rgb[2] = hsv[2]; }
    else              { rgb[0] = hsv[2]; rgb[1] = p;      rgb[2] = q;      }

    return rgb;
}
