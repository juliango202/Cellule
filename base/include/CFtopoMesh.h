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
#ifndef __CFtopoMesh_h__
#define __CFtopoMesh_h__

#include "CFtopoPoint.h"
#include "CFtopoWeightedSet.h"
#include "CFvectorField.h"
#include "CFtopoTriangle.h"

#include <vector>

class CFcircle;

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFtopoMesh
//
//  Author: bEnGo
//  Date:   2003
//
//  Desc:   Ensemble de cercles 2D
//  ----
//          Les cercles sont chainés et définissent un maillage.
//          Cela permet le parcours spatial de l'ensemble.
//
//          - RECHERCHE:    FindTriangle( position )
//                      
//              A partir d'un cercle de départ ( point courant ), on parcourt
//              le maillage en cherchant à se rapprocher de la position donnée.
//              Lorsqu'on détecte une boucle dans le parcours, c'est que l'on vient
//              de délimiter le triangle renfermant ce point.
//
//          - INSERTION:    Insert( position )
//
//              On recherche dans le maillage le triangle où se situe position.
//              On subdivise ce triangle en 3, avec pour sommet le nouveau point.
//              On lance une réorganisation du maillage pour maintenir la contrainte
//              de Delauney pondérée par les rayons des cercles.
//              Cette opération se propage récursivement.
//
//          - MAINTENANCE:  RelaxSelection()
//
//
//  To do:  - OPTIMIZE:     move(...) et Scale(...)
//  -----
//              Faire le check de place libre pour insérer AVANT le UnLink
//              au lieu de délinker et relinker pour rien si c'est négatif
//
////////////////////////////////////////////////////////////////////////////

class CFtopoMesh
{
public:

    typedef std::set<CFtopoPoint*>              Points;
    typedef Points::iterator                    PointsIt;
    typedef Points::const_iterator              ConstPointsIt;

public:

    //  Initialisation / Destruction
    //
                            CFtopoMesh          ();
    void                    Clear               ( void );

                            ~CFtopoMesh         ();

    //  Iterators
    //
    inline PointsIt         GetPointsBegin      ( void )                                { return m_points.begin();  }
    inline ConstPointsIt    GetPointsBegin      ( void ) const                          { return m_points.begin();  }
    inline PointsIt         GetPointsEnd        ( void )                                { return m_points.end();    }
    inline ConstPointsIt    GetPointsEnd        ( void ) const                          { return m_points.end();    }

    //  Points management
    //
    inline void             SetCheck            ( bool                      onOrOff     )   { m_check = onOrOff;    }
    
    bool                    Insert              ( CFtopoPoint&              point       );

    void                    Remove              ( CFtopoPoint&              point       );

    void                    Relax               ( CFtopoWeightedSet&        points      );
    
    void                    move                ( CFtopoPoint&              point, 
                                                  const CFvector&           movement    );
    
    void                    move                ( CFtopoWeightedSet&        points, 
                                                  const CFvectorField&      vectorField );

    void                    Scale               ( CFtopoPoint&              point, 
                                                  float                     factor      );
    
    void                    Scale               ( CFtopoWeightedSet&        points      );

    //  Queries
    //
    bool                    IsEmpty             ( void                                  ) const { return m_points.empty(); }

    CFtopoTriangle          FindTriangle        ( const CFpoint&            point       ) const;

    CFtopoPoint*            FindCell            ( const CFpoint&            point,
                                                  CFfloat*                  resultDistance = 0  ) const;

    CFtopoWeightedSet       FindCells           ( const CFcircle&           area        ) const;
    
    
    bool                    IsMaster            ( const CFtopoPoint&        point       ) const;

    //  openGL drawing
    //
    void                    DrawDelauney        ( void                                  ) const;
    void                    DrawPoints          ( void                                  ) const;

private:

    //  Internal
    //
    bool                    Link                ( CFtopoPoint&              point       );

    void                    UnLink              ( CFtopoPoint&              point       );

    void                    RecurseDelauney     ( CFtopoPoint*              newPoint,
                                                  CFtopoPoint*              edgeStart,
                                                  CFtopoPoint*              edgeEnd     );

    void                    RecurseSearch       ( const CFcircle&           area,
                                                  CFtopoPoint*              start,
                                                  CFtopoWeightedSet&        result      ) const;

private:

    //  Master triangle
    //
    CFtopoPoint*            m_master1;
    CFtopoPoint*            m_master2;
    CFtopoPoint*            m_master3;

    //  Inserted points
    //
    Points                  m_points;

    //  Current location
    //
    mutable CFtopoPoint*    m_curPoint;     // for FindTriangle

    //  Control activation of penetration checking
    //  to maintain weighted Voronoi integrity
    //
    bool                    m_check;
};

#endif //__CFtopoMesh_h__
