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
#include "NWdisconnectButton.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

NWdisconnectButton::NWdisconnectButton( NWplug* plug, QWidget* parent )
:
    QAbstractButton( parent                         ),
    m_plug         ( plug                           ),
    m_icon         ( "icons/22x22/dialog-close.png" )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    connect( this, SIGNAL( clicked() ), SLOT( doDisconnect() ) );
}

void NWdisconnectButton::paintEvent( QPaintEvent* event )
{
    QPainter painter( this );

    painter.drawPixmap( event->rect(), m_icon, m_icon.rect() );
}

void NWdisconnectButton::doDisconnect( void )
{
    m_plug->disconnectSource();
}
