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
#include "CFuvMesh.h"

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include "windows.h"
#endif //_WIN32
#include "GL/gl.h"
#include "assert.h"

CFuvMesh::CFuvMesh()
{
}

CFuvMesh::~CFuvMesh()
{
    Clear();
}

void CFuvMesh::Clear( void )
{
    FacesIt faceIt = m_faces.begin();

    while( faceIt != m_faces.end() )
    {
        delete *faceIt;
        faceIt++;
    }

    m_faces.clear();
}

void CFuvMesh::Draw( void ) const
{
    glColor3d( 1.0, 0.6, 0.3 );

    ConstFacesIt faceIt = m_faces.begin();

    while( faceIt != m_faces.end() )
    {
        CFuvFace*   face = *faceIt;
        
        glBegin( GL_LINE_LOOP );
        
        CFuvFace::ConstUvIdsIt uvIdIt = face->GetUvIdsBegin();

        while( uvIdIt != face->GetUvIdsEnd() )
        {
            const CFpoint& uv = m_uvs[*uvIdIt];

            glVertex2d( uv[0], uv[1] );

            uvIdIt++;
        }

        glEnd();

        
        faceIt++;
    }
}

bool CFuvMesh::LoadOBJ( const std::string& fileName )
{
    Clear();

    std::ifstream   file( fileName.c_str() );
    std::string     word;

    //  Find first "vt ..."
    //
    while( word != "vt" )
    {
        file >> word;

        if( file.eof() ) return false;
    }

    //  Read uv points array
    //
    CFpoint uv;

    while( word == "vt" )
    {
        file >> uv[0];
        file >> uv[1];

        m_uvs.push_back( uv );

        file >> word;

        if( file.eof() ) return false;
    }

    //  Find first "f ..."
    //
    while( word != "f" )
    {
        file >> word;

        if( file.eof() ) return false;
    }

    //  Read faces array
    //
    CFuvFace* face;

    while( word == "f" )
    {
        face = new CFuvFace();

        m_faces.push_back( face );

        file >> word;

        while( word != "f" )
        {
            std::string::size_type  afterFirstSlash = word.find( '/' ) + 1;
            std::string::size_type  secondSlash     = word.find( '/', afterFirstSlash );

            face->AddUvId( atoi( word.substr( afterFirstSlash, secondSlash - afterFirstSlash ).c_str() ) - 1 );

            file >> word;

            if( file.eof() ) break;
        }
    }
    
    file.close();

    return true;
}

