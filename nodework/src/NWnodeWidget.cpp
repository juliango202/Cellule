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
#include "NWnodeWidget.h"

#include "NWnode.h"
#include "NWnameEditor.h"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

NWnodeWidget::NWnodeWidget( NWnode* node, QWidget* parent )
:
    QWidget( parent ),
    m_node ( node   )
{
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

    //  Layout
    //
    m_layout = new QGridLayout( this );

    //  Populate with sub-widgets
    //
    rebuild();

    //  Connect signals / slots
    //
    connect( node, SIGNAL( incomingConnectionMade  ( void ) ), SLOT( rebuild( void ) ) );
    connect( node, SIGNAL( incomingConnectionBroken( void ) ), SLOT( rebuild( void ) ) );
}

void NWnodeWidget::rebuild( void )
{
    QListIterator<QObject*> childIt( children() );

    while( childIt.hasNext() )
    {
        QObject* child( childIt.next() );

        if( child->isWidgetType() ) child->deleteLater();
    }

    //  Name
    //
    QLabel*       label      = new QLabel      ( "name", this );
    NWnameEditor* nameEditor = new NWnameEditor( m_node, this );

    m_layout->addWidget( label,      0, 0 );
    m_layout->addWidget( nameEditor, 0, 1 );

    //  Inputs
    //
    QListIterator<NWplug*> plugIt( m_node->inputs() );

    while( plugIt.hasNext() ) plugIt.next()->createGUI( m_layout );
}
