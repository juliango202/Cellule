//	(C) Copyright 2005-2008 Benjamin Godon and contributors.
//
//	This file is part of Cellule.
//
//	Cellule is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	Cellule is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with Cellule; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "CWcell.h"

void CWcell::Draw( const CWsurfaceShader* defaultShader, const DrawStyle& style ) const
{
    const CWsurfaceShader* shader( m_shader );

    if( ! shader ) shader = defaultShader;

    const CFcolor& shaderColor( shader->input( 2 ).value<CFcolor>() );
    const CFfloat  smoothness = shader->input( CWsurfaceShader::kSmoothness ).value<CFfloat>();

    shaderColor.Draw();

    //  Draw shaded cell
    //
    if( style.drawShading )
    {
        //  Begin from the center
        //
        glBegin( GL_TRIANGLE_FAN );

        CFpoint::Draw();

        //  Add the first vertex to the drawing
        //
        const std::vector<CFpoint>& voronoiVertices = VoronoiVertices();

        voronoiVertices[0].Draw();

        //  Add every other vertices, finishing by drawing the first vertex again
        //  to close the OpenGL drawing loop
        //
        int i = voronoiVertices.size();

        while( i-- ) voronoiVertices[i].Draw();

        glEnd();
        
        //  Switch to gray to see center, seed and contour
        //
        glColor3f( 0.5f, 0.5f, 0.5f );
    }
    
    if( style.drawCenter )
    {
        glBegin( GL_POINTS );
        
        CFpoint::Draw();
        
        glEnd();
    }
        
    if( style.drawSeed )
    {
        CFcircle::Draw();
    }
    
    if( style.drawContour || ( style.drawBSpline && smoothness == 0) )
    {
        glBegin( GL_LINE_LOOP );

        const std::vector<CFpoint>& voronoiVertices = VoronoiVertices();

        int i = voronoiVertices.size();
        while( i-- ) voronoiVertices[i].Draw();

        glEnd();
    }
    else if( style.drawBSpline )
    {
           const CFbspline& bspline = BSplineContour(smoothness);
           bspline.draw();
    }
}


