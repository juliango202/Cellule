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
#include "CArenderTool.h"

#include "CAapplication.h"
#include "CAview2D.h"
#include "CArenderArea.h"

#include "CWscene.h"
#include "CWcamera2D.h"

#include "CFvectorField.h"

#include <cmath>

void CArenderTool::click( const CFpoint& position )
{
    move( position );

    CWcamera2D* camera2D = CAapplication::instance()->getScene().getDefaultCamera2D();

    m_corner1 = camera2D->WorldToScreen( position );
    m_corner2 = m_corner1;

    m_visible = true;
 }

void CArenderTool::drag( const CFpoint& position )
{
    move( position );

    m_corner2 = CAapplication::instance()->getScene().getDefaultCamera2D()->WorldToScreen( position );
}

void CArenderTool::unclick( const CFpoint& position )
{
    drag( position );

    m_visible = false;

    CAview2D* view2D = qobject_cast<CAview2D*>( CAapplication::instance()->focusWidget() );
    
    if( view2D )
    {
        //view2D->getRenderArea()->setGeometry( CFpoint(100,140), CFpoint(300,340) );
        view2D->getRenderArea()->setGeometry( m_corner1, m_corner2 );
        view2D->getRenderArea()->render();
    }
}

void CArenderTool::Draw( void ) const
{
    if( ! m_visible ) return;

    //  Draw the frame
    //
    CWcamera2D* camera2D = CAapplication::instance()->getScene().getDefaultCamera2D();

    camera2D->ScreenView();
    glColor3f( 0.6f, 0.9f, 0.6f );

    glBegin( GL_LINE_STRIP );

        CFpoint corner( m_corner1 );    corner.Draw();
        corner[0] = m_corner2[0];       corner.Draw();
        corner[1] = m_corner2[1];       corner.Draw();
        corner[0] = m_corner1[0];       corner.Draw();
        corner[1] = m_corner1[1];       corner.Draw();

    glEnd();

    camera2D->WorldView();
}
