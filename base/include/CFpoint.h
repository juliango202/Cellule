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
#ifndef __CFpoint_h__
#define __CFpoint_h__

#include "CFtypes.h"
#include <iomanip>

class CFpoint
{
public:
                                CFpoint         ( CFfloat       x = 0.0f,
                                                  CFfloat       y = 0.0f )              { m_x = x; m_y = y; }

    CFfloat                     Distance2       ( const CFpoint& point  )       const;
    CFfloat                     Distance        ( const CFpoint& point  )       const;
    CFfloat                     Angle           ( const CFpoint& vect   )       const;
    CFfloat                     Length2         ( void )                        const;
    CFfloat                     Length          ( void )                        const;
    CFpoint&                    Normalize       ( void );

    inline void                 SetPosition     ( const CFpoint&    position    )       { m_x = position[0]; m_y = position[1];         }

    void                        ToCartesian     ( void );
    void                        ToPolar         ( void );

    void                        Rotate          ( const CFfloat     angle       );

    inline const CFfloat&       operator[]      ( const unsigned i )            const   { CFassert(i<2,""); if( i ) return m_y; return m_x; }
    inline CFfloat&             operator[]      ( const unsigned i )                    { CFassert(i<2,""); if( i ) return m_y; return m_x; }

    void                        Draw            () const;

    inline CFpoint&             operator=       ( const CFpoint &point )                { m_x = point.m_x; m_y = point.m_y; return *this; }

    CFpoint                     operator*       ( const CFfloat factor )        const;
    CFpoint                     operator/       ( const CFfloat factor )        const;

    CFpoint                     operator+       ( const CFpoint &point )        const;

    inline CFfloat              operator*       ( const CFpoint &point )        const   { return m_x * point.m_x + m_y * point.m_y; }
    inline CFfloat              operator^       ( const CFpoint &point )        const   { return m_x * point.m_y - m_y * point.m_x; }

    void                        operator+=      ( const CFpoint &point );
    void                        operator-=      ( const CFpoint &point );
    void                        operator/=      ( const CFfloat factor );
    void                        operator*=      ( const CFfloat factor );
    void                        operator/=      ( const CFpoint &byAxisFactors );
    void                        operator*=      ( const CFpoint &byAxisFactors );

protected:

    CFfloat                     m_x;
    CFfloat                     m_y;
};

typedef CFpoint CFvector;

inline const CFpoint operator-( const CFpoint& left, const CFpoint& right )
{
    return CFpoint( left[0] - right[0], left[1] - right[1] );
}

inline std::ostream& operator<<( std::ostream& os, const CFpoint& point )
{
    os << point[0] << " " << point[1];

    return os;
}

inline std::istream& operator>>( std::istream& is, CFpoint& point )
{
    is >> point[0] >> point[1];

    return is;
}


#endif //__CFpoint_h__
