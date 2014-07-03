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
#ifndef __CFsegment_h__
#define __CFsegment_h__

#include "CFpoint.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFsegment
//
//  Author: bEnGo
//  Date:   2004
//
//  Desc:   Line segment ( is-a CFpoint : origin, with another CFpoint member : direction )
//
//  To do:  Put IntervalCollide in a CFinterval class instead
//
////////////////////////////////////////////////////////////////////////////

class CFsegment : public CFpoint
{
public:
                            CFsegment       (                               )   {}

                            CFsegment       ( CFpoint           a,
                                              CFpoint           b           )   : CFpoint( a ), m_direction( b - a )    {}

    int                     Collide         ( const CFsegment&  with,
                                              CFsegment&        where       )   const;

    inline CFsegment&       operator=       ( const CFpoint&    point       )   { m_x = point[0]; m_y = point[1]; return *this; }

    friend std::ostream&    operator<<      ( std::ostream&     os,
                                              const CFsegment&  segment     );

    friend std::istream&    operator>>      ( std::istream&     is,
                                              CFsegment&        segment     );

protected:

    //  Used by Collide(...)
    //
    static int              IntervalCollide ( CFfloat           u0,
                                              CFfloat           u1,
                                              CFfloat           v0,
                                              CFfloat           v1,
                                              CFfloat&          inter1,
                                              CFfloat&          inter2      );

protected:

    CFpoint                 m_direction;
};

inline std::ostream& operator<<( std::ostream& os, const CFsegment& segment )
{
    os << segment[0] << " " << segment[1] << " " << segment.m_direction[0] << " " << segment.m_direction[1];

    return os;
}

inline std::istream& operator>>( std::istream& is, CFsegment& segment )
{
    is >> segment[0] >> segment[1] >> segment.m_direction[0] >> segment.m_direction[1];

    return is;
}

#endif //__CFsegment_h__
