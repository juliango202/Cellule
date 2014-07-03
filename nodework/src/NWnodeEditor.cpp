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
#include "NWnodeEditor.h"

#include "NWnodeWidget.h"
#include "NWselection.h"

#include <QtGui/QTabWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>

#include <iostream>

NWnodeEditor::NWnodeEditor( QWidget* parent ) : QWidget( parent )
{
    m_pages = new QTabWidget( this );

    QVBoxLayout* layout = new QVBoxLayout( this );

    layout->addWidget( m_pages );
}

QSize NWnodeEditor::sizeHint( void ) const
{
    return QSize( 150, 300 );
}

void NWnodeEditor::clear( void )
{
    int i = m_pages->count();

    while( i-- ) m_pages->widget( i )->deleteLater();

    m_pages->clear();
    m_nodes.clear();
}

void NWnodeEditor::addContent( NWnode* node )
{
    QScrollArea* scrollArea = new QScrollArea;

    scrollArea->setWidget( new NWnodeWidget( node ) );

    scrollArea->setWidgetResizable( true );

    m_pages->addTab( scrollArea, node->name() );
    m_nodes.push_back( node );

    connect( node, SIGNAL( renamed( const QString& ) ), SLOT( updateTabTitles( void ) ) );
    connect( node, SIGNAL( renamed( const QString& ) ), SLOT( updateTabTitles( void ) ) );
}

void NWnodeEditor::setContent( const NWselection& selection )
{
    clear();

    NWselection::NodeConstIt nodeIt( selection.firstNodeIt() );

    while( nodeIt != selection.endNodeIt() ) addContent( *nodeIt++ );
}

void NWnodeEditor::updateTabTitles( void )
{
    unsigned int index = m_nodes.size();

    while( index-- )
    {
        m_pages->setTabText( index, m_nodes[ index ]->name() );
    }
}
