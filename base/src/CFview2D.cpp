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
#include "CFview2D.h"

#include "CFcamera2D.h"
#include "CFlog.h"

#include <QtGui/QKeyEvent>

CFview2D::CFview2D( QWidget* parent, CFcamera2D* camera )
    :
    QGLWidget           ( parent       ),
    m_camera            ( camera       ),
    m_mouseButton       ( Qt::NoButton ),
    m_abortState        ( false        ),
    m_controlMode       ( kNone        ),
    m_screenPosOrig     ( 0.0, 0.0     ),
    m_dragging          ( false        )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    setFocusPolicy( Qt::StrongFocus );
    
    setMouseTracking( true );
}

QSize CFview2D::sizeHint() const
{
    return QSize( 720, 576 );
}

void CFview2D::initializeGL()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    
    glEnable( GL_TEXTURE_2D );
}

void CFview2D::resizeGL( int w, int h )
{
    if( m_camera ) m_camera->Resize( w, h );
}

void CFview2D::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    if( ! m_camera )
    {
        CFlog::instance()->addMessage( "2D view has no camera.", CFlog::kWarning );
        return;
    }

    //  Setup view
    //
    m_camera->WorldView();
}

void CFview2D::mousePressEvent( QMouseEvent* event )
{
    if( ! m_camera ) return;

    if( m_controlMode != kNone ) return;

    //  Analyze keyboard and mouse context
    //
    m_mouseButton    = event->button();
    m_mouseScreenPos = event->globalPos();
    m_clickPosition  = m_camera->ScreenToWorld( CFpoint( event->x(), event->y() ) );
    m_mousePosition  = m_clickPosition;
    m_mousePrevPos   = m_mousePosition;

    Qt::KeyboardModifiers specialKeys ( event->modifiers() );

    if( specialKeys.testFlag( Qt::AltModifier ) )
    {
             if( m_mouseButton == Qt::LeftButton  ) m_controlMode = kCameraPan;
        else if( m_mouseButton == Qt::MidButton   ) m_controlMode = kCameraZoom;
        else if( m_mouseButton == Qt::RightButton ) m_controlMode = kCameraRoll;
    }

    //  Pass the event to the derived class
    //
    if( m_controlMode == kNone ) mouseClick();

    //  Control the camera
    //
    else
    {
        //  Save click position
        //
        m_screenPosOrig[0] = event->x();
        m_screenPosOrig[1] = event->y();

        //  Save camera position
        //
             if( m_controlMode == kCameraPan  ) m_camValueOrig    = m_camera->pan();
        else if( m_controlMode == kCameraZoom ) m_camValueOrig[0] = m_camera->zoom();
        else if( m_controlMode == kCameraRoll ) m_camValueOrig[0] = m_camera->roll();
    }

    updateGL();
}

void CFview2D::mouseMoveEvent( QMouseEvent* event )
{
    if( ! m_camera ) return;

    m_mouseScreenPos = event->globalPos();
    m_mousePrevPos   = m_mousePosition;
    m_mousePosition  = m_camera->ScreenToWorld( CFpoint( event->x(), event->y() ) );

    CFpoint mouseDelta( event->x(), event->y() ); mouseDelta -= m_screenPosOrig;

    //  Control the camera
    //
    if( m_controlMode == kCameraPan )
    {
        mouseDelta[1] *= -1.0;
        mouseDelta.Rotate( -m_camera->roll() );
        mouseDelta /= 1000.0 * m_camera->zoom();

        m_camera->pan( m_camValueOrig + mouseDelta );
    }
    else if( m_controlMode == kCameraZoom )
    {
        m_camera->zoom( m_camValueOrig[0] * ( 1.0 + mouseDelta[0] / 1000.0 ) );
    }
    else if( m_controlMode == kCameraRoll )
    {
        m_camera->roll( m_camValueOrig[0] + mouseDelta[0] / 600.0 );
    }

    //  Pass the event to the derived class
    //
    else
    {
        if( m_mouseButton == Qt::NoButton ) mouseMove();
        else                                mouseDrag();
    }

    updateGL();
}

void CFview2D::mouseReleaseEvent( QMouseEvent* event )
{
    if( ! m_camera ) return;

    //  Pass the event to the derived class
    //
    if( m_controlMode == kNone && m_mouseButton != Qt::NoButton ) mouseUnclick();

    m_controlMode = kNone;
    m_mouseButton = Qt::NoButton ;

    updateGL();
}

void CFview2D::keyPressEvent( QKeyEvent* event )
{
     switch( event->key() ) {
    case Qt::Key_Escape:
        m_abortState = true;
        break;
    
    default:
        QGLWidget::keyPressEvent(event);
    }
}


