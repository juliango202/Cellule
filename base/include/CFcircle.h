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
#ifndef __CFcircle_h__
#define __CFcircle_h__

#include "CFpoint.h"

#include <cmath>
#include "assert.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFcircle
//
//  Author: bEnGo
//  Date:   2004
//
//  Desc:   Cercle
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CFcircle : public CFpoint
{
public:
                            CFcircle    ( CFfloat           x       = 0.0f,
                                          CFfloat           y       = 0.0f,
                                          CFfloat           radius  = 1.0f  )   : CFpoint( x, y ), m_radius2( radius * radius ) {}

                            CFcircle    ( const CFpoint&    center,
                                          CFfloat           radius  = 1.0f  )   : CFpoint( center ), m_radius2( radius * radius ) {}

                            //  Circumcircle construction
                            //
                            CFcircle    ( const CFpoint&    point1,
                                          const CFpoint&    point2,
                                          const CFpoint&    point3  );

                            //  Circumcircle of weighted points ( with radius2 equal to the power function )
                            //
                            CFcircle    ( const CFcircle&   point1,
                                          const CFcircle&   point2,
                                          const CFcircle&   point3  );
    //
    //
    inline CFfloat          GetRadius   ( void                      ) const     { return sqrt( m_radius2 ); }
    inline const CFfloat&   GetRadius2  ( void                      ) const     { return m_radius2;         }
    inline CFfloat&         GetRadius2  ( void                      )           { return m_radius2;         }
    inline void             SetRadius2  ( CFfloat           radius2 )           { m_radius2 = radius2;      }

    CFfloat                 Distance    ( const CFcircle&   circle  ) const;
    CFfloat                 Distance    ( const CFpoint&    point   ) const     { return CFpoint::Distance( point ) - GetRadius();  }

    CFfloat                 PowDist     ( const CFcircle&   circle  ) const;
    CFfloat                 PowDist     ( const CFpoint&    point   ) const     { return CFpoint::Distance2( point ) - GetRadius2(); }

    //  In-circle test
    //
    inline bool             Collide     ( const CFpoint&    point   ) const     { return CFpoint::Distance2( point ) < m_radius2;   }
    inline bool             Collide     ( const CFcircle&   circle  ) const     { return Distance( circle ) < 0.0;                  }

    //  Line or Segment intersection test
    //
    bool                    Collide     ( const CFpoint&    p1,
                                          const CFpoint&    p2,
                                          bool              segment = true  ) const;

    void                    Draw        (                                   ) const;

    inline CFcircle&        operator=   ( const CFpoint&    point   )           { m_x = point[0]; m_y = point[1]; return *this; }

    friend std::ostream& operator<<( std::ostream& os, const CFcircle& circle );
    friend std::istream& operator>>( std::istream& is, CFcircle& circle );

protected:

    CFfloat     m_radius2;      // radius * radius
};

inline std::ostream& operator<<( std::ostream& os, const CFcircle& circle )
{
    os << circle[0] << " " << circle[1] << " " << circle.m_radius2;

    return os;
}

inline std::istream& operator>>( std::istream& is, CFcircle& circle )
{
    is >> circle[0] >> circle[1] >> circle.m_radius2;

    return is;
}

#endif //__CFcircle_h__
