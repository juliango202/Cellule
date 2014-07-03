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
#include "CAview2D.h"

#include "CAapplication.h"
#include "CArenderArea.h"
#include "CAtoolManager.h"
#include "CAtool.h"

#include "CWscene.h"
#include "CWcamera2D.h"

#include <QtCore/QObject>
#include <QtGui/QWheelEvent>

CAview2D::CAview2D( QWidget* parent, CWcamera2D* camera )
:
    CFview2D           ( parent, camera           ),
    m_displayDelauney  ( false                    ),
    m_renderArea       ( new CArenderArea( this ) )
{
}

void CAview2D::paintGL()
{
    CFview2D::paintGL();

    //  Draw the world
    //
    CAapplication::instance()->getScene().Draw( m_cellDrawStyle, m_displayDelauney );

    //  Draw render area
    //
    m_renderArea->draw();

    //  Draw current tool
    //
    CAtool* tool( CAapplication::instance()->getToolManager().getCurrentTool() );

    tool->Draw();
}

void CAview2D::mouseClick( void )
{
    CAapplication::instance()->getToolManager().clickCurrent( m_mousePosition );
}

void CAview2D::mouseMove( void )
{
    CAapplication::instance()->getToolManager().moveCurrent( m_mousePosition );
}

void CAview2D::mouseDrag( void )
{
    CAapplication::instance()->getToolManager().dragCurrent( m_mousePosition );
}

void CAview2D::mouseUnclick( void )
{
    CAapplication::instance()->getToolManager().unclickCurrent( m_mousePosition );
}

void CAview2D::wheelEvent( QWheelEvent* event )
{
    if( ! m_camera ) return;

    int     degrees = event->delta() / 8;
    float   spin    = degrees / 360.0f;

    CAtool* currentTool = CAapplication::instance()->getToolManager().getCurrentTool();

    if( event->modifiers().testFlag( Qt::ControlModifier ) )
    {
        currentTool->intensity() += spin * 2.0f;
    }
    else currentTool->shape().GetRadius2() *= 1.0f + spin * 4.0f;
    
    updateGL();
}

QObject* CAview2D::renderArea( void )
{
    return m_renderArea;
}

void CAview2D::setDisplayLevelPoint( void )
{
    m_cellDrawStyle.drawCenter  = true;
    m_cellDrawStyle.drawContour = false;
    m_cellDrawStyle.drawShading = false;
    m_cellDrawStyle.drawBSpline = false;
}

void CAview2D::setDisplayLevelWireframe( void )
{
    m_cellDrawStyle.drawCenter  = false;
    m_cellDrawStyle.drawContour = true;
    m_cellDrawStyle.drawShading = false;
    m_cellDrawStyle.drawBSpline = false;
}

void CAview2D::setDisplayLevelShaded( void )
{
    m_cellDrawStyle.drawCenter  = false;
    m_cellDrawStyle.drawContour = true;
    m_cellDrawStyle.drawShading = true;
    m_cellDrawStyle.drawBSpline = false;
}

void CAview2D::setDisplayLevelBSpline( void )
{
    m_cellDrawStyle.drawCenter  = false;
    m_cellDrawStyle.drawContour = false;
    m_cellDrawStyle.drawShading = false;
    m_cellDrawStyle.drawBSpline = true;
}
