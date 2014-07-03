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
#ifndef __CFview2D_h__
#define __CFview2D_h__

#include "CFpoint.h"

#include <QtOpenGL/QGLWidget>

class CFcamera2D;

//! \brief  OpenGL drawable area with an associated and manipulable orthographic camera
//! \author bEngo
//! \date   2008 March 24
//!
class CFview2D : public QGLWidget
{
    Q_OBJECT

public:
                                CFview2D                ( QWidget*        parent,
                                                          CFcamera2D*     camera    );

    QSize                       sizeHint                ()                              const;

    virtual void                mouseClick              ( void                      ) = 0;
    virtual void                mouseMove               ( void                      ) = 0;
    virtual void                mouseDrag               ( void                      ) = 0;
    virtual void                mouseUnclick            ( void                      ) = 0;

    bool                        getAbortState           ( void                      ) { return m_abortState; }
    void                        resetAbortState         ( void                      ) { m_abortState = false; }

protected:
    
    //----------    OpenGL    -----------
    //
    void                        initializeGL            ();

    void                        resizeGL                ( int             w,
                                                          int             h         );

    void                        paintGL                 ();

    //----------    Events    -----------
    //
    void                        mousePressEvent         ( QMouseEvent*    event     );
    void                        mouseMoveEvent          ( QMouseEvent*    event     );
    void                        mouseReleaseEvent       ( QMouseEvent*    event     );
    void                        keyPressEvent           ( QKeyEvent*      event     );

protected:

    enum ControlMode
    {
        kNone,
        kCameraPan,
        kCameraRoll,
        kCameraZoom
    };

protected:

    CFcamera2D*                 m_camera;

    Qt::MouseButton             m_mouseButton;      //  Button previously clicked
    CFpoint                     m_clickPosition;    //  Previous click position     ( graph editor space )
    QPoint                      m_mouseScreenPos;   //  Mouse position              ( screen space       )
    CFpoint                     m_mousePosition;    //  Mouse position              ( graph editor space )
    CFpoint                     m_mousePrevPos;     //  Previous mouse position     ( graph editor space )
    bool                        m_abortState;

private:

    ControlMode                 m_controlMode;

    CFpoint                     m_screenPosOrig;
    CFpoint                     m_camValueOrig;
    bool                        m_dragging;
};

#endif //__CFview2D_h__
