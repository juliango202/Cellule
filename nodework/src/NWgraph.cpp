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
#include "NWgraph.h"

#include "NWnode.h"
#include "NWselection.h"
#include "NWnodeLibrary.h"

#include "CFlog.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>


NWgraph::NWgraph( QObject* parent ) : QObject( parent )
{
    m_rootNode.setObjectName( "root" );
}

NWnode* NWgraph::findNode( const QString& name ) const
{
    QMap<QString, NWnode*>::const_iterator nodeNameIt = m_nodes.find( name );

    if( nodeNameIt == m_nodes.end() ) return 0;

    return nodeNameIt.value();
}

NWnode* NWgraph::findNode( const CFtype& type ) const
{
    Nodes::const_iterator nodeIt( m_nodes.begin() );

    for( ; nodeIt != m_nodes.end(); nodeIt++ )
    {
        if( nodeIt.value()->type() == type ) return nodeIt.value();
    }

    return 0;
}

QString NWgraph::newUniqueName( const QString& proposition ) const
{
    QString newName( proposition );
    QString format ( "%1_%2"     );

    unsigned int index = 1;

    while( findNode( newName ) )
    {
        newName = format.arg( proposition ).arg( index++ );
    }

    return newName;
}

NWnode* NWgraph::createNode( CFtype type, const QString& name, NWnode* parent, bool isMandatory, bool isSingleton )
{
    //  Ensure NO singleton of that type
    //  has been created before
    //
    if( m_singletonTypes.find( type ) != m_singletonTypes.end() )
    {
        CFlog::instance()->addMessage( QString( "There can be only one " ) + type.name(), CFlog::kError );
        return 0;
    }

    //  When creating a singleton node
    //  ensure NO other node of that type
    //  has been created before
    //
    if( isSingleton && findNode( type ) )
    {
        CFlog::instance()->addMessage( QString( "Can not create a " ) + type.name() + " as singleton. Others already exists.", CFlog::kError );
        return 0;
    }

    //  Create a node instance of the given type
    //
    NWnode* node = m_nodeLibrary.createNode( type );

    if( ! node )
    {
        CFlog::instance()->addMessage( QString( "Unknown node type: " ) + type.name(), CFlog::kError );
        return 0;
    }

    node->m_graph       = this;
    node->m_isMandatory = isMandatory;
    node->m_isSingleton = isSingleton;
    
    //  Register the type as a singleton type
    //
    if( isSingleton ) m_singletonTypes.insert( type );

    //  Name the node and update the name map
    //
    QString uniqueName( newUniqueName( name.isEmpty() ? type.name() : name ) );

    node->setObjectName( uniqueName );

    m_nodes.insert( uniqueName, node );

    //  Parent the node
    //
    node->setParent( parent ? parent : &m_rootNode );

    //  Clear selection and select the new node
    //
    m_selection.set( node );

    //
    //
    connect( node, SIGNAL( incomingConnectionMade  ( void ) ), this, SIGNAL( incomingConnectionMade  ( void ) ) );
    connect( node, SIGNAL( incomingConnectionBroken( void ) ), this, SIGNAL( incomingConnectionBroken( void ) ) );

    emit nodeAdded( node );

    return node;
}

bool NWgraph::renameNode( NWnode* node, const QString& newName, bool autoFixBadName )
{
    if( node->name() == newName ) return true;

    if( node->isMandatory() )
    {
        CFlog::instance()->addMessage( "Mandatory nodes can't be renamed.", CFlog::kError );
        return false;
    }

    //  Check that given name is correct and unique
    //
    QString goodNewName( newName );

    if( goodNewName.isEmpty() )
    {
        if( autoFixBadName ) goodNewName = "unnamed";
        else
        {
            CFlog::instance()->addMessage( "name is invalid.", CFlog::kError );
            return false;
        }
    }

    if( m_nodes.find( goodNewName ) != m_nodes.end() )
    {
        if( autoFixBadName ) goodNewName = newUniqueName( goodNewName );
        else
        {
            CFlog::instance()->addMessage( newName + " is not an unique name.", CFlog::kError );
            return false;
        }
    }

    //  Update name map
    //
    int howManyRemoved = m_nodes.remove( node->name() ); assert( howManyRemoved == 1 );

    m_nodes.insert( goodNewName, node );

    //  Rename the node
    //
    node->setObjectName( goodNewName );

    return true;
}

void NWgraph::clear( void )
{
    m_singletonTypes.clear();
    
    //  Delete all nodes
    //
    const QObjectList& children( m_rootNode.children() );

    int i; for( i = 0; i < children.size(); ++i )
    {
        children.at( i )->deleteLater();
    }

    m_nodes.clear();
}

void NWgraph::saveTo( CFxmlOut& xml ) const
{
    //  Write nodes
    //
    const QObjectList& children( m_rootNode.children() );

    int i; for( i = 0; i < children.size(); ++i )
    {
        NWnode* node( static_cast<NWnode*>( children.at( i ) ) );

        node->saveTo( xml );
    }

    //  Write connections

    //  For each node
    //
    Nodes::const_iterator nodeIt( m_nodes.begin() );

    for( ; nodeIt != m_nodes.end(); nodeIt++ )
    {
        NWnode* node( nodeIt.value() );

        //  For each input
        //
        NWnode::PlugIt inputIt( node->inputs() );
    
        while( inputIt.hasNext() )
        {
            const NWplug* destination( inputIt.next()                    );
            NWplug*       source     ( destination->incomingConnection() );

            if( source )
            {
                xml.newLine();
                
                xml.stream() << "<connection";

                xml.stream() << " srcNode=\""  << source->node()->name().toStdString()          << "\"";
                xml.stream() << " srcAttr=\""  << source->attribute()->name().toStdString()     << "\"";

                xml.stream() << " destNode=\"" << node->name().toStdString()                     << "\"";
                xml.stream() << " destAttr=\"" << destination->attribute()->name().toStdString() << "\"";

                xml.stream() << " />";
            }
        }
    }
}

void NWgraph::loadFrom( CFxmlIn& xml )
{
    while( ! xml.atEnd() )
    {
        CFxmlIn::TokenType tokenType( xml.readAfterWhitespaces() );

        if( tokenType == CFxmlIn::StartElement )
        {
            QXmlStreamAttributes attributes( xml.attributes() );

            if( xml.name() == "scene" )
            {
            }
            else if( xml.name() == "node" )
            {
                QString nodeName( attributes.value( "name" ).toString() );
                
                NWnode* node = findNode( nodeName );

                if( ! node ) node = createNode
                (
                    m_nodeLibrary.findNodeType( attributes.value( "type" ).toString() ),
                    nodeName
                );
            
                if( node ) node->loadFrom( xml );
            }
            else if( xml.name() == "connection" )
            {
                NWnode* srcNode ( findNode( attributes.value( "srcNode"  ).toString() ) );
                NWnode* destNode( findNode( attributes.value( "destNode" ).toString() ) );

                if( srcNode && destNode )
                {
                    NWplug* srcPlug ( srcNode ->findPlug( attributes.value( "srcAttr"  ).toString() ) );
                    NWplug* destPlug( destNode->findPlug( attributes.value( "destAttr" ).toString() ) );

                    if( srcPlug && destPlug )
                    {
                        bool success = destPlug->connectSource( *srcPlug ); assert( success );
                    }
                }
            }
            else if( xml.name() == "cells" )
            {
                break;
            }
        }
    }
    if( xml.hasError() )
    {
        CFlog::instance()->addMessage( xml.errorString(), CFlog::kError );
    }
}
