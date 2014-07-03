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
#ifndef __CAview2D_h__
#define __CAview2D_h__

#include "CWcell.h"

#include "CFview2D.h"
#include "CFpoint.h"

class CArenderArea;
class CWcamera2D;


class CAview2D : public CFview2D
{
    Q_OBJECT
    Q_PROPERTY( QObject* renderArea READ renderArea )

public:
                                CAview2D                ( QWidget*        parent,
                                                          CWcamera2D*     camera   );

    CArenderArea*               getRenderArea           ( void                     )    { return m_renderArea; }
    
    void                        paintGL                 ();

    virtual void                mouseClick              ( void                     );
    virtual void                mouseMove               ( void                     );
    virtual void                mouseDrag               ( void                     );
    virtual void                mouseUnclick            ( void                     );

    void                        wheelEvent              ( QWheelEvent*    event    );

    QObject*                    renderArea              ( void                     );

public slots:

    void                        setDisplayLevelPoint    ( void                     );
    void                        setDisplayLevelWireframe( void                     );
    void                        setDisplayLevelShaded   ( void                     );
    void                        setDisplayLevelBSpline  ( void                     );

    void                        displaySeeds            ( bool            onOff    )   { m_cellDrawStyle.drawSeed  = onOff; }
    void                        displayDelauney         ( bool            onOff    )   { m_displayDelauney         = onOff; }

protected:
    
    CWcell::DrawStyle           m_cellDrawStyle;
    bool                        m_displayDelauney;

    CArenderArea*               m_renderArea;
};

#endif //__CAview2D_h__
