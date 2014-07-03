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
#include "NWnode.h"

#include "NWgraph.h"
#include "NWinputAttribute.h"
#include "NWoutputAttribute.h"

#include <assert.h>

#include <sstream>

NWnode::NWnode()
:
    m_state         ( kAltered ),
    m_graph         ( 0        ),
    m_isMandatory   ( false    ),
    m_isSingleton   ( false    ),
    m_visualPosition( 0.0, 0.0 )
{
}

NWnode::~NWnode()
{
}

bool NWnode::rename( const QString& newName, bool autoFixBadName )
{
    if( m_graph->renameNode( this, newName, autoFixBadName ) )
    {
        emit renamed( name() );

        return true;
    }
    return false;
}

void NWnode::addInput( const NWinputAttribute& attribute )
{
    NWplug* newPlug = new NWplug( this, attribute );

    m_inputs.push_back( newPlug );

    connect( newPlug, SIGNAL( incomingConnectionMade  ( void ) ), this, SIGNAL( incomingConnectionMade  ( void ) ) );
    connect( newPlug, SIGNAL( incomingConnectionBroken( void ) ), this, SIGNAL( incomingConnectionBroken( void ) ) );

    //  Store the index ( one-based, positive ) of the new plug
    //  for futur retrieval given the attribute name
    //
    m_plugIndex.insert( attribute.name(), m_inputs.size() );
}

void NWnode::addOutput( const NWoutputAttribute& attribute )
{
    m_outputs.push_back( new NWplug( this, attribute ) );

    //  Store the index ( one-based, negative ) of the new plug
    //  for futur retrieval given the attribute name
    //
    m_plugIndex.insert( attribute.name(), -m_outputs.size() );
}

NWplug* NWnode::findPlug( const QString& attributeName )
{
    QMap<QString,int>::iterator indexIt( m_plugIndex.find( attributeName ) );

    if( indexIt == m_plugIndex.end() ) return 0;

    int plugIndex = indexIt.value();

    if( plugIndex > 0 ) return m_inputs[ plugIndex - 1 ];

    return m_outputs[ -plugIndex - 1 ];
}

void NWnode::downgradeState( State state )
{
    if( state < m_state ) m_state = state;
}

void NWnode::configure( void )
{
    if( m_state != kAltered ) return;

    m_state = kConfigured;
}

void NWnode::evaluate( void )
{
    if( m_state == kEvaluated  ) return;
    if( m_state <  kConfigured ) configure();

    m_state = kEvaluated;
}

void NWnode::saveTo( CFxmlOut& xml ) const
{
    //  Open node tag
    //
    xml.newLine();

    xml.stream() << "<node";
    xml.stream() << " type=\"" << type().name().toStdString() << "\"";
    xml.stream() << " name=\"" << name().toStdString() << "\"";
    xml.stream() << " x=\""    << m_visualPosition[0] << "\"";
    xml.stream() << " y=\""    << m_visualPosition[1] << "\"";
    xml.stream() << ">";

    xml.indent();

    //  Save input plugs
    //
    xml.newLine(); xml.stream() << "<inputs>"; xml.indent();

    PlugIt inputIt( inputs() );

    while( inputIt.hasNext() )
    {
        //  if there is an incoming connection,
        //  plug value need not be saved
        //
        //if( inputIt->incomingConnection() ) continue;

        inputIt.next()->saveTo( xml );
    }

    xml.unIndent(); xml.newLine(); xml.stream() << "</inputs>"; 

    //  Save children
    //
    xml.newLine(); xml.stream() << "<children>"; xml.indent();
    
    const QObjectList& children( QObject::children() );

    int i; for( i = 0; i < children.size(); ++i )
    {
        NWnode* node( static_cast<NWnode*>( children.at( i ) ) );

        node->saveTo( xml );
    }

    xml.unIndent(); xml.newLine(); xml.stream() << "</children>";

    //  Close node tag
    //
    xml.unIndent(); xml.newLine(); xml.stream() << "</node>";
}

void NWnode::loadFrom( CFxmlIn& xml )
{
    //  Read visual position
    //
    m_visualPosition[0] = xml.attributes().value( "x" ).toString().toDouble();
    m_visualPosition[1] = xml.attributes().value( "y" ).toString().toDouble();

    //  Read plugs
    //
    CFxmlIn::TokenType tokenType = xml.readAfterWhitespaces();

    assert( tokenType  == QXmlStreamReader::StartElement );
    assert( xml.name() == "inputs" );

    while( ! xml.atEnd() )
    {
        //  Read plug infos
        //
        tokenType = xml.readAfterWhitespaces();

        if( tokenType != QXmlStreamReader::StartElement || xml.name() != "plug" ) break;

        QXmlStreamAttributes attributes( xml.attributes() );

        //  Find plug on the node from attribute name
        //
        NWplug* plug( findPlug( attributes.value( "name" ).toString() ) );

        assert( plug );

        //  Read and set value
        //
        plug->loadFrom( xml );

        //  Plug closing tag
        //
        tokenType = xml.readAfterWhitespaces();

        assert( tokenType  == QXmlStreamReader::EndElement );
        assert( xml.name() == "plug" );
    }
}
