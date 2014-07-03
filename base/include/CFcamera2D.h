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
#ifndef __CFcamera2D_h__
#define __CFcamera2D_h__

#include "CFcircle.h"
#include "CFpoint.h"
#include "CFuvMesh.h"
#include "CFcolor.h"

#include <QtCore/QObject>

//! \brief  Orthographic OpenGL camera
//! \author bEngo
//! \date   2008 March 24
//!
class CFcamera2D : public QObject
{
    Q_OBJECT
public:

                                CFcamera2D          ( QObject*          parent          );

    void                        Resize              ( unsigned int      xmax,
                                                      unsigned int      ymax            );

    void                        pan                 ( const CFpoint&    position        )       { m_position = position;        }
    const CFpoint&              pan                 ( void                              ) const { return m_position;            }

    void                        zoom                ( CFfloat           scale           )       { m_scale = scale;              }
    CFfloat                     zoom                ( void                              ) const { return m_scale;               }

    void                        roll                ( CFfloat           orientation     )       { m_orientation = orientation;  }
    CFfloat                     roll                ( void                              ) const { return m_orientation;         }

    CFpoint                     ScreenToWorld       ( const CFpoint&    screenPoint     ) const;
    CFpoint                     WorldToScreen       ( const CFpoint&    worldPoint      ) const;
    CFcircle                    ScreenToWorld       ( const CFcircle&   screenCircle    ) const;
    CFcircle                    WorldToScreen       ( const CFcircle&   worldCircle     ) const;

    void                        WorldView           ( void ) const;
    void                        ScreenView          ( void ) const;

protected:

    unsigned int                m_xmax, m_ymax;         // Window size
    CFpoint                     m_halfScreenSize;
    CFfloat                     m_aspectRatio;
    CFpoint                     m_worldSize;

    CFpoint                     m_position;
    CFfloat                     m_orientation;
    CFfloat                     m_scale;
};

#endif //__CFcamera2D_h__
