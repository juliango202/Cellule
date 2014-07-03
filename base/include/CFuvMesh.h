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
#ifndef __CFuvMesh_h__
#define __CFuvMesh_h__

#include "CFpoint.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFuvMesh
//
//  Author: bEnGo & JuL
//  Date:   03 juin 2004
//
//  Desc:   Maillage UV
//
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CFuvFace;

class CFuvMesh
{
public:
    typedef std::vector<CFpoint>        Uvs;
    typedef Uvs::iterator               UvsIt;
    typedef Uvs::const_iterator         ConstUvsIt;

    typedef std::vector<CFuvFace*>      Faces;
    typedef Faces::iterator             FacesIt;
    typedef Faces::const_iterator       ConstFacesIt;

public:
                        CFuvMesh        ();
    virtual             ~CFuvMesh       ();

    void                Clear           ( void );

    //  OpenGL
    //
    void                Draw            ( void )    const;

    //  Entr�es - Sorties
    //
    virtual bool        LoadOBJ         ( const std::string&    fileName    );

private:

    Uvs                 m_uvs;
    Faces               m_faces;
};

class CFuvFace
{
public:
    typedef std::vector<unsigned int>   UvIds;
    typedef UvIds::iterator             UvIdsIt;
    typedef UvIds::const_iterator       ConstUvIdsIt;

    //  It�rateurs
    //
    inline UvIdsIt      GetUvIdsBegin   ( void )                                { return m_uvIds.begin();       }
    inline UvIdsIt      GetUvIdsEnd     ( void )                                { return m_uvIds.end();         }

    //  Creation
    //
    inline void         AddUvId         ( unsigned int  uvId )                  { m_uvIds.push_back( uvId );    }

private:

    UvIds               m_uvIds;
};

#endif //__CFuvMesh_h__
