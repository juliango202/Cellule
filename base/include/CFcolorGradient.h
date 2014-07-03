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
#ifndef __CFcolorGradient_h__
#define __CFcolorGradient_h__

#include "CFkey1DList.h"
#include "CFcolor.h"

//! \brief  Compute a color gradient by interpolation in a list of colors
//! \author bEngo
//! \date   2008 March 26
//!
class CFcolorGradient : public CFkey1DList<CFcolor>
{
public:

    enum Interpolation
    {
        kConstant,
        kLinear,
        kSmooth
    };

public:
                        CFcolorGradient     ( Interpolation         interpolation = kLinear );

    Interpolation       interpolation       ( void                                          ) const { return m_interpolation;          }
    void                setInterpolation    ( Interpolation         interpolation           )       { m_interpolation = interpolation; }

    CFcolor             evaluate            ( CFfloat               position                ) const;

    CFcolor             constant            ( CFfloat               position                ) const;
    CFcolor             linear              ( CFfloat               position                ) const;
    CFcolor             smooth              ( CFfloat               position                ) const;

    void                saveTo              ( std::ostream&         stream                  ) const;
    void                loadFrom            ( std::istream&         stream                  );

protected:

    Interpolation       m_interpolation;
};

inline std::ostream& operator<<( std::ostream& stream, const CFcolorGradient& gradient ) { gradient.saveTo  ( stream ); return stream; }
inline std::istream& operator>>( std::istream& stream,       CFcolorGradient& gradient ) { gradient.loadFrom( stream ); return stream; }

#endif //__CFcolorGradient_h__
