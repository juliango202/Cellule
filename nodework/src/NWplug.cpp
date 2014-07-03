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
#include "NWplug.h"

#include "NWnode.h"
#include "NWinputAttribute.h"
#include "NWoutputAttribute.h"

#include "NWdisconnectButton.h"
#include "NWboolWidget.h"
#include "NWrealWidget.h"
#include "NWintWidget.h"
#include "NWenumWidget.h"
#include "NWcolorWidget.h"
#include "NWcolorGradientWidget.h"

#include "CFenum.h"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <sstream>

NWplug::NWplug( NWnode* node, const NWinputAttribute& attribute )
:
    m_node      ( node                            ),
    m_attribute ( &attribute                      ),
    m_direction ( kInput                          ),
    m_data      ( attribute.initialValue().copy() ),
    m_source    ( 0                               )
{
}

NWplug::NWplug( NWnode* node, const NWoutputAttribute& attribute )
:
    m_node      ( node             ),
    m_attribute ( &attribute       ),
    m_direction ( kOutput          ),
    m_data      ( attribute.type() ),
    m_source    ( 0                )
{
}

bool NWplug::isCompatible( const NWplug& plug ) const
{
    return m_data.type() == plug.m_data.type();
}

bool NWplug::connectSource( NWplug& sourcePlug )
{
    //  Check that such a connection is possible
    //
    if( m_direction != kInput  ||
        incomingConnection()   ||
        ! isCompatible( sourcePlug )       ) return false;

    //  Connect
    //
    sourcePlug.m_destinations.push_back( this );

    m_source = &sourcePlug;

    notifyChange();

    emit incomingConnectionMade();

    return true;
}

void NWplug::disconnectSource( void )
{
    if( ! m_source ) return;

    std::vector<NWplug*>& srcDestinations( m_source->m_destinations );

    std::vector<NWplug*>::iterator plugIt = std::find
    (
        srcDestinations.begin(),
        srcDestinations.end(),
        this
    );

    if( plugIt != srcDestinations.end() ) srcDestinations.erase( plugIt );

    m_source = 0;

    notifyChange();

    emit incomingConnectionBroken();
}

void NWplug::notifyChange( void )
{
    if( m_direction == kInput )
    {
        const NWinputAttribute* attr( static_cast<const NWinputAttribute*>( m_attribute ) );
        
        if( attr->level() == NWinputAttribute::kConfiguration )
        {
            m_node->downgradeState( NWnode::kAltered );
        }
        else
        {
            assert( attr->level() == NWinputAttribute::kEvaluation );

            m_node->downgradeState( NWnode::kConfigured );
        }
    }

    std::vector<NWplug*>::iterator destinationIt( m_destinations.begin() );

    for( ; destinationIt != m_destinations.end(); destinationIt++ )
    {
        NWplug* destination( *destinationIt );

        destination->notifyChange();
    }
}

void NWplug::createGUI( QGridLayout* gridLayout )
{
    assert( Direction() == NWplug::kInput );

    QWidget* parent   = gridLayout->parentWidget();
    int      rowIndex = gridLayout->rowCount();

    QLabel*             name = new QLabel( m_attribute->name(), parent );
    QWidget*            widget;
    NWdisconnectButton* disconnectButton = 0;

    if( incomingConnection() )
    {
        QPalette redText; redText.setColor( QPalette::WindowText, Qt::red );
        name->setPalette( redText );

        widget = m_source->createWidget( parent );

        widget->setEnabled( false );

        disconnectButton = new NWdisconnectButton( this, parent );
    }
    else widget = createWidget( parent );

    //  Layout widgets
    //
    gridLayout->addWidget( name,   rowIndex, 0 );
    gridLayout->addWidget( widget, rowIndex, 1 );

    if( disconnectButton )
    {
        gridLayout->addWidget( disconnectButton, rowIndex, 2 );
    }
}

QWidget* NWplug::createWidget( QWidget* parent )
{
    if( m_data.type() == CFdata::type<bool           >() ) return new NWboolWidget         ( this, parent );
    if( m_data.type() == CFdata::type<int            >() ) return new NWintWidget          ( this, parent );
    if( m_data.type() == CFdata::type<CFfloat        >() ) return new NWrealWidget         ( this, parent );
    if( m_data.type() == CFdata::type<CFenum         >() ) return new NWenumWidget         ( this, parent );
    if( m_data.type() == CFdata::type<CFcolor        >() ) return new NWcolorWidget        ( this, parent );
    if( m_data.type() == CFdata::type<CFcolorGradient>() ) return new NWcolorGradientWidget( this, parent );

    assert(0);
    return new QWidget( parent );
}

void NWplug::saveTo( CFxmlOut& xml ) const
{
    assert( m_direction != kOutput );

    xml.newLine();

    xml.stream() << "<plug" << " name=\"" << m_attribute->name().toStdString() << "\">";

    m_data.saveTo( xml.stream() );

    xml.stream() << "</plug>";
}

void NWplug::loadFrom( CFxmlIn& xml )
{
    CFxmlIn::TokenType tokenType = xml.readAfterWhitespaces();

    assert( tokenType == QXmlStreamReader::Characters );

    std::string         valueAsText      ( xml.text().toString().toStdString() );
    std::istringstream  valueAsTextStream( valueAsText );

    m_data.loadFrom( valueAsTextStream );
}

void NWplug::evaluate( void ) const
{
    if( m_direction == kOutput )
    {
        m_node->evaluate();
    }
}
