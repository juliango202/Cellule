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
#ifndef __NWcolorGradientWidget_h__
#define __NWcolorGradientWidget_h__

#include "CFtypes.h"

#include <QtCore/QPoint>
#include <QtGui/QWidget>

#include <vector>

class NWplug;

class QMenu;
class QAction;

//! \brief  Widget for color gradient edition
//! \author bEngo
//! \date   2008 March 25
//!
class NWcolorGradientWidget : public QWidget
{
    Q_OBJECT

public:
                            NWcolorGradientWidget       ( NWplug*           gradientPlug,
                                                          QWidget*          parent      = 0 );

    virtual QSize           sizeHint                    ( void                              ) const;

    virtual void            resizeEvent                 ( QResizeEvent*     event           );
    virtual void            paintEvent                  ( QPaintEvent*      event           );
    virtual void            mousePressEvent             ( QMouseEvent*      event           );
    virtual void            mouseMoveEvent              ( QMouseEvent*      event           );
    virtual void            mouseReleaseEvent           ( QMouseEvent*      event           );
    virtual void            mouseDoubleClickEvent       ( QMouseEvent*      event           );

public slots:

    void                    setConstantInterp           ( void                              );
    void                    setLinearInterp             ( void                              );
    void                    setSmoothInterp             ( void                              );

    void                    changeColor                 ( void                              );
    void                    deleteColor                 ( void                              );

protected:

    CFfloat                 toGradientSpace             ( int               position        ) const;
    int                     toWidgetSpace               ( CFfloat           position        ) const;

protected:

    NWplug*                 m_gradientPlug;

    static const int        m_handleRadius;
    static const int        m_handleSize;
    static const int        m_handleAreaSize;

    QRect                   m_gradiantArea;
    std::vector<QPoint>     m_handlePositions;
    int                     m_selectedHandle;

    bool                    m_dragging;

    QMenu*                  m_interpolationMenu;

    QMenu*                  m_handleMenu;
    QAction*                m_deleteColorAction;
};

#endif //__NWcolorGradientWidget_h__
