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
#ifndef __CFtopoPoint_h__
#define __CFtopoPoint_h__

#include "CFcircle.h"
#include "CFtopoDirection.h"
#include "CFbspline.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFtopoPoint
//
//  Author: bEnGo
//  Date:   2003
//
//  Desc:   Point 2D chainé à 0 ou plusieurs autres.
//              On peut le ranger dans un conteneur CFtopoMesh
//              pour lequel il devient un point du graph de Delauney.
//              Ses liens vers ses voisins sont appelés "directions".
//
//              - AJOUT / LIEN:     link( point )
//
//                  On ajoute la direction allant à point dans la liste des directions.
//
//              - RECHERCHE:        findDirection( position )
//
//                  On recherche la direction la plus proche de celle permettant d'atteindre
//                  la position donnée
//
//          Leur taille influance le maillage resultant ( CFtopoMesh )
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////


class CFtopoPoint : public CFcircle
{
public:
    typedef std::vector<CFtopoDirection>    Dirs;
    typedef Dirs::iterator                  DirsIt;
    typedef Dirs::const_iterator            ConstDirsIt;

public:
                                    CFtopoPoint         ( const CFcircle&       circle      )   : CFcircle( circle )               {}

                                    CFtopoPoint         ( const CFfloat&        x,
                                                          const CFfloat&        y,
                                                          const CFfloat&        radius      )   : CFcircle( x, y, radius )         {}

    inline DirsIt                   GetDirsBegin        ( void )                                    { return m_directions.begin();  }
    inline ConstDirsIt              GetDirsBegin        ( void ) const                              { return m_directions.begin();  }
    inline DirsIt                   GetDirsEnd          ( void )                                    { return m_directions.end();    }
    inline ConstDirsIt              GetDirsEnd          ( void ) const                              { return m_directions.end();    }

    void                            Clear              ( void );

    void                            Link                ( CFtopoPoint*          neighbor,
                                                          bool                  bothSide = true );

    void                            UnLink              ( CFtopoPoint*          neighbor,
                                                          bool                  bothSide = true ); 

    DirsIt                          FindDirection       ( const CFpoint&        position        );

    const std::vector<CFpoint>&     VoronoiVertices     ( void                                  ) const;
    const CFbspline&                BSplineContour      ( CFfloat               smoothness      ) const;


    //  Given an edge and the third vertex of the triangle,
    //  return the opposite triangle ( sharing the given edge ).
    //  If only an edge is given ( neighbor = 0 ) then return randomly one of the 2 triangles.
    //
    CFtopoPoint*                    FindOpposite        ( const CFtopoPoint*    edgeEnd,
                                                          const CFtopoPoint*    neighbor = 0    );

    const CFtopoDirection&          GetDirection        ( int                   index           ) const { return m_directions[ index ]; }


    //  Given a point in the cell, find its projection on the cell contour and return the normalized distance to the cell center
    CFfloat                         DistanceToCenter    ( const CFpoint&        p,
                                                          CFfloat               smoothness      ) const;


    friend std::ostream& operator<<( std::ostream& os, const CFtopoPoint& point );
    friend std::istream& operator>>( std::istream& is, CFtopoPoint& point );


private:
    Dirs                                m_directions;

    mutable CFbspline                   m_bspline;
    mutable std::vector<CFpoint>        m_voronoi;
};

inline std::ostream& operator<<( std::ostream& os, const CFtopoPoint& point )
{
    os << point[0] << " " << point[1] << " " << point.m_radius2;

    return os;
}

inline std::istream& operator>>( std::istream& is, CFtopoPoint& point )
{
    is >> point[0] >> point[1] >> point.m_radius2;

    return is;
}

#endif //__CFtopoPoint_h__
