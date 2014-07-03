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
#ifndef __CFcolor_h__
#define __CFcolor_h__

#include "CFtypes.h"

#include <QtGui/QColor>

#include <cmath>

class CFcolor
{
public:

    enum Space
    {
        kRGB,
        kHSV
    };

    static const CFcolor kBlack;
    static const CFcolor kRed;
    static const CFcolor kGreen;
    static const CFcolor kBlue;
    static const CFcolor kGray;
    static const CFcolor kWhite;

public:
                        CFcolor     ();

                        CFcolor     ( const CFfloat3&   components,
                                      Space             space           );

                        CFcolor     ( CFfloat           component1,
                                      CFfloat           component2,
                                      CFfloat           component3,
                                      Space             space           );

                        CFcolor     ( const QColor&     color           );

    const CFfloat3&     asRGB       ( void                              ) const { return m_rgb;             }
    CFfloat3            asHSV       ( void                              ) const { return RGBtoHSV( m_rgb ); }

    QColor              asQColor    ( void                              ) const;

    inline void         Draw        ( void                              ) const { glColor3f( m_rgb[0], m_rgb[1], m_rgb[2] ); }
    
    void                setRGB      ( CFfloat           r,
                                      CFfloat           g,
                                      CFfloat           b               )       { m_rgb[0] = r; m_rgb[1] = g; m_rgb[2] = b; }

    void                ClampRGB    ( CFfloat           min = 0.0,
                                      CFfloat           max = 1.0       );
    
    CFcolor             Blend       ( const CFcolor&    color,
                                      CFfloat           parameter = 0.5 ) const;

    CFcolor             operator*   ( CFfloat           factor          ) const { return CFcolor( m_rgb * factor, kRGB ); }
    CFcolor             operator/   ( CFfloat           factor          ) const { return CFcolor( m_rgb / factor, kRGB ); }
    void                operator*=  ( CFfloat           factor          )       { m_rgb *= factor; }
    void                operator/=  ( CFfloat           factor          )       { m_rgb /= factor; }

    CFcolor             operator+   ( const CFcolor&    color           ) const { return CFcolor( m_rgb + color.m_rgb, kRGB ); }
    void                operator+=  ( const CFcolor&    color           )       { m_rgb += color.m_rgb;                        }

    CFcolor             operator*   ( const CFcolor&    color           ) const;
    
    CFfloat3            RGBtoHSV   ( const CFfloat3&    rgb             ) const;
    CFfloat3            HSVtoRGB   ( const CFfloat3&    hsv             ) const;

    void                saveTo     ( std::ostream&      stream          ) const { stream << m_rgb[0] << " " << m_rgb[1] << " " << m_rgb[2]; }
    void                loadFrom   ( std::istream&      stream          )       { stream >> m_rgb[0]        >> m_rgb[1]        >> m_rgb[2]; }

protected:

    CFfloat3            m_rgb;
};

inline std::ostream& operator<<( std::ostream& stream, const CFcolor& color ) { color.saveTo  ( stream ); return stream; }
inline std::istream& operator>>( std::istream& stream,       CFcolor& color ) { color.loadFrom( stream ); return stream; }

#endif //__CFcolor_h__
