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
#ifndef __CFtopoDirection_h__
#define __CFtopoDirection_h__

#include "CFtypes.h"

#include "GL/gl.h"

#include <functional>

class CFtopoPoint;

class CFtopoDirection
{
public:
                                CFtopoDirection ( CFfloat                   angle,
                                                  CFtopoPoint*              point   )   : m_angle( angle ), m_point( point ) {}

    inline const CFtopoPoint*   GetPoint        ( void ) const                              { return m_point; }
    inline CFtopoPoint*         GetPoint        ( void )                                    { return m_point; }

    inline bool                 operator<       ( CFfloat                   angle   ) const { return m_angle < angle; }
    inline bool                 operator<       ( const CFtopoDirection&    d2      ) const { return m_angle < d2.m_angle; }

private:

    CFfloat                     m_angle;
    CFtopoPoint*                m_point;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//          FUNCTORS

struct CFtopoDirectionMatchPoint : std::unary_function<CFtopoDirection, bool>
{
                            CFtopoDirectionMatchPoint   ( const CFtopoPoint*        point       )   : m_point( point ) {}

    inline bool             operator()                  ( const CFtopoDirection&    direction   ) const
    {
        return direction.GetPoint() == m_point;
    }

    const CFtopoPoint*  m_point;
};

#endif //__CFtopoDirection_h__
