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
#ifndef __NWcolorWidget_h__
#define __NWcolorWidget_h__

#include <QtGui/QWidget>

class NWplug;

//! \brief  Piece of GUI controlling a CFcolor
//! \author bEngo
//! \date   2008 April 11
//!
class NWcolorWidget : public QWidget
{
    Q_OBJECT

public:
                            NWcolorWidget               ( NWplug*           colorPlug,
                                                          QWidget*          parent      = 0 );

    virtual QSize           sizeHint                    ( void                              ) const;

    virtual void            resizeEvent                 ( QResizeEvent*     event           );
    virtual void            paintEvent                  ( QPaintEvent*      event           );
    virtual void            mousePressEvent             ( QMouseEvent*      event           );

protected:

    NWplug*                 m_colorPlug;
    QRect                   m_drawingArea;
};

#endif //__NWcolorWidget_h__
