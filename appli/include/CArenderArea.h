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
#ifndef __CArenderArea_h__
#define __CArenderArea_h__

#include "CFpoint.h"
#include "CFglImage.h"

#include <QtCore/QObject>

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CArenderArea
//
//  Author: bEnGo
//  Date:   2006
//
//  Desc:
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CArenderArea : public QObject
{
    Q_OBJECT

public:
                    CArenderArea      ( QObject*          parent = 0       );
    
    void            show              ( void                               )  { m_visible = true;        }
    void            hide              ( void                               )  { m_visible = false;       }
    void            toggleVisibility  ( void                               )  { m_visible = ! m_visible; }
    
    // SetGeometry: define rendered rectangle from two ( Screen View ) coordinates
    //
    void            setGeometry       ( const CFpoint&    c1,
                                        const CFpoint&    c2               );
    
    void            draw              ( void                               ) const;

public slots:

    void            render            ( void                               );
    void            saveAs            ( const QString*    fileName         ) const;

private:

    bool            m_visible;

    unsigned int    m_resolutionX;
    unsigned int    m_resolutionY;
    CFpoint         m_worldTopLeftCorner;
    CFpoint         m_worldTopRightCorner;
    CFpoint         m_worldBottomRightCorner;
    CFpoint         m_worldBottomLeftCorner;

    CFglImage*      m_image;
};


#endif //__CArenderArea_h__
