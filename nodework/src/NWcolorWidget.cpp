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
#include "NWcolorWidget.h"

#include "NWplug.h"

#include <QtGui/QPainter>
#include <QtGui/QColorDialog>

NWcolorWidget::NWcolorWidget( NWplug* colorPlug, QWidget* parent )
:
    QWidget    ( parent    ),
    m_colorPlug( colorPlug )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

QSize NWcolorWidget::sizeHint( void ) const
{
    return QSize( 55, 25 );
}

void NWcolorWidget::resizeEvent( QResizeEvent* event )
{
    m_drawingArea = rect();

    m_drawingArea.adjust( 5, 1, -2, -2 );
}

void NWcolorWidget::paintEvent( QPaintEvent* event )
{
    QPainter painter( this );
    
    painter.fillRect( m_drawingArea, m_colorPlug->value<CFcolor>().asQColor() );
    painter.setPen  ( Qt::black );
    painter.drawRect( m_drawingArea );
}

void NWcolorWidget::mousePressEvent( QMouseEvent* event )
{
    QColor newColor
    (
        QColorDialog::getColor( m_colorPlug->value<CFcolor>().asQColor(), this )
    );

    if( newColor.isValid() ) m_colorPlug->setValue<CFcolor>( newColor );

    update();
}
