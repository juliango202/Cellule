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
#ifndef __CFtopoTriangle_h__
#define __CFtopoTriangle_h__

#include "CFtopoPoint.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFtopoTriangle
//
//  Author: bEnGo
//  Start:  12 Dec 2004
//
//  Desc:   C'est un triangle formé par 3 points du graphe de Delauney.
//
//          Ce triangle correspond à un point du graphe de Voronoi,
//          localisé au centre du cercle circonscrit à ce triangle.
//          Chaque point de ce triangle correspond à 1 des 3 cellules
//          de Voronoi autour de ce point de Voronoi.
//          Chaque edge de ce triangle correspond à 1 des 3 edges
//          de Voronoi qui partent de ce point de Voronoi.
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CFtopoTriangle
{
public:

    CFcircle            GetCircum       ( void )    const   { return CFcircle( *p1, *p2, *p3 );     }
    CFpoint             GetCenter       ( void )    const   { return ( *p1 + *p2 + *p3 ) / 3.0f;    }

    //  Get neighbor cell opposite to the given point in the triangle
    //
    CFtopoTriangle      GetOpposite     ( CFtopoPoint* from )   const
    {
        CFtopoTriangle neighbor = *this;

        if      ( from == p1 )
        {
            neighbor.p1 = p2->FindOpposite( p3, p1 );
        }
        else if ( from == p2 )
        {
            neighbor.p2 = p1; 
            neighbor.p1 = p1->FindOpposite( p3, p2 );
        }
        else
        {
            neighbor.p3 = p1;
            neighbor.p1 = p1->FindOpposite( p2, p3 );

            CFassert( from == p3, "from parameter is junk :(" )
        }

        return neighbor;
    }

public:
    CFtopoPoint*        p1;
    CFtopoPoint*        p2;
    CFtopoPoint*        p3;
};

#endif //__CFtopoTriangle_h__
