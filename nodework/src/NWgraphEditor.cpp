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
#include "NWgraphEditor.h"

#include "NWgraphEditorNode.h"
#include "NWnode.h"
#include "NWselection.h"

#include "CFindexedAction.h"
#include "CFcamera2D.h"

#include <QtGui/QMenu>

NWgraphEditor::NWgraphEditor( QWidget* parentWidget, NWgraph* graph )
:
    CFview2D             ( parentWidget, 0 ),
    m_graph              ( graph           ),
    m_controlMode        ( kNone           ),
    m_controlNode        ( 0               )
{
    m_camera = new CFcamera2D( this );

    connect( m_graph, SIGNAL( incomingConnectionMade  ( void ) ), this, SLOT( update( void ) ) );
    connect( m_graph, SIGNAL( incomingConnectionBroken( void ) ), this, SLOT( update( void ) ) );
}

NWgraphEditor::~NWgraphEditor()
{
    clearContent();
}

QSize NWgraphEditor::sizeHint( void ) const
{
    return QSize( 400, 260 );
}

void NWgraphEditor::paintGL( void )
{
    if( ! m_graph ) return;
    
    CFview2D::paintGL();
    
    //  Draw nodes
    //
    int i; for( i = 0; i < m_content.size(); i++ )
    {
        NWgraphEditorNode* graphEdNode( m_content[ i ]      );
        NWnode*            node       ( graphEdNode->node() );

        if( ! node )
        {
            delete graphEdNode;
            m_content.removeAt( i-- );
        }
        else graphEdNode->draw( this, m_graph->getSelection().has( node ) );
    }

    //  While selecting draw selection frame
    //
    if( m_controlMode == kSelect )
    {
        glColor3f( 0.5f, 0.5f, 0.5f );
        
        glLineStipple( 1, 0xAAAA );
        
        glEnable( GL_LINE_STIPPLE ); glBegin( GL_LINE_LOOP );
        
        glVertex2f( m_clickPosition[0], m_clickPosition[1] );
        glVertex2f( m_mousePosition[0], m_clickPosition[1] );
        glVertex2f( m_mousePosition[0], m_mousePosition[1] );
        glVertex2f( m_clickPosition[0], m_mousePosition[1] );
        
        glEnd(); glDisable( GL_LINE_STIPPLE );
    }

    //  While connecting draw future connection
    //
    else if( m_controlMode == kConnect )
    {
        glColor3f( 1.0f, 0.7f, 0.3f );
        
        glBegin( GL_LINES );
        
        glVertex2f( m_clickPosition[0], m_clickPosition[1] );
        glVertex2f( m_mousePosition[0], m_mousePosition[1] );
        
        glEnd();
    }
}

void NWgraphEditor::mouseClick( void )
{
    if( ! m_graph ) return;

    //  Find which node was clicked on
    //
    m_controlNode = findNode( m_mousePosition );

    if( m_mouseButton == Qt::LeftButton )
    {
        //  Select the clicked node
        //  and activate move-nodes-by-dragging mode
        //
        if( m_controlNode )
        {
            NWselection& selection( m_graph->getSelection()  );
            NWnode*      node     ( m_controlNode->node() );
    
            if( ! selection.has( node ) ) selection.set( node );
            
            m_controlMode  = kMoveSelection;
        }

        //  No clicked node
        //  activate the frame selection mode
        //
        else m_controlMode = kSelect;
    }
    else if( m_mouseButton == Qt::RightButton )
    {
        if( m_controlNode )
        {
            QMenu menu( "Connect" );
                
            //  Connection source popup menu
            //
            if( m_controlMode == kNone )
            {
                NWnode::Plugs& outputs( m_controlNode->node()->outputs() );
    
                int i; for( i = 0; i < outputs.size(); i++ )
                {
                    CFindexedAction* selectSource( new CFindexedAction( outputs[i]->name(), i, &menu ) );

                    connect( selectSource, SIGNAL( triggered( unsigned int ) ), SLOT( beginConnection( unsigned int ) ) );

                    menu.addAction( selectSource );
                }

                if( ! menu.isEmpty() ) menu.exec( m_mouseScreenPos );
            }

            //  Connection destination popup menu
            //
            else if( m_controlMode == kConnect )
            {
                //  For each input plug ( connection potential destination ) of the clicked node
                //
                NWnode::Plugs& inputs( m_controlNode->node()->inputs() );
    
                int i; for( i = 0; i < inputs.size(); i++ )
                {
                    //  Don't create a menu entry for input plugs incompatible with the connection source
                    //
                    if( ! inputs[i]->isCompatible( *m_connectionSource ) ) continue;

                    //  Create a menu entry for the input plug
                    //
                    CFindexedAction* selectDestination( new CFindexedAction( inputs[i]->name(), i, &menu ) );

                    connect( selectDestination, SIGNAL( triggered( unsigned int ) ), SLOT( endConnection( unsigned int ) ) );

                    //  Disable the menu entry if the input plug already receive a connection
                    //
                    if( inputs[i]->incomingConnection() ) selectDestination->setEnabled( false );

                    //  Populate the menu
                    //
                    menu.addAction( selectDestination );
                }

                if( ! menu.isEmpty() ) menu.exec( m_mouseScreenPos );
            }
        }
    }
}

void NWgraphEditor::mouseMove( void )
{
}

void NWgraphEditor::mouseDrag( void )
{
    if( ! m_graph ) return;

    if( m_controlMode == kMoveSelection )
    {
        foreach( NWgraphEditorNode* graphEdNode, m_content )
        {
            if( graphEdNode->node() && m_graph->getSelection().has( graphEdNode->node() ) )
            {
                graphEdNode->move( m_mousePosition - m_mousePrevPos );
            }
        }
    }
}

void NWgraphEditor::mouseUnclick( void )
{
    if( m_graph && ( m_controlMode == kSelect ) )
    {
        m_graph->getSelection().clear();

        foreach( NWgraphEditorNode* graphEdNode, findNodes( m_clickPosition, m_mousePosition ) )
        {
            m_graph->getSelection().add( graphEdNode->node() );
        }
    }

    m_controlMode = kNone;
}

NWgraphEditorNode* NWgraphEditor::findNode( const NWnode* node )
{
    foreach( NWgraphEditorNode* graphEdNode, m_content )
    {
        if( graphEdNode->node() == node )
        {
            return graphEdNode;
        }
    }

    return 0;
}

NWgraphEditorNode* NWgraphEditor::findNode( const CFpoint& position )
{
    foreach( NWgraphEditorNode* graphEdNode, m_content )
    {
        if( graphEdNode->node() && graphEdNode->intersect( position ) )
        {
            return graphEdNode;
        }
    }

    return 0;
}

QList<NWgraphEditorNode*> NWgraphEditor::findNodes( const CFpoint& corner1, const CFpoint& corner2 )
{
    QList<NWgraphEditorNode*> nodes;

    foreach( NWgraphEditorNode* graphEdNode, m_content )
    {
        if( graphEdNode->node() && graphEdNode->intersect( corner1, corner2 ) )
        {
            nodes.append( graphEdNode );
        }
    }

    return nodes;
}

void NWgraphEditor::clearContent( void )
{
    foreach( NWgraphEditorNode* graphEdNode, m_content ) delete graphEdNode;
    
    m_content.clear();
}

void NWgraphEditor::setContent( NWnode* node )
{
    clearContent();
    addContent( node );
}

void NWgraphEditor::addContent( NWnode* node )
{
    m_content.append( new NWgraphEditorNode( node ) );
}

void NWgraphEditor::beginConnection( unsigned int index )
{
    m_controlMode = kConnect;

    m_connectionSource = m_controlNode->node()->outputs()[ index ];
}

void NWgraphEditor::endConnection( unsigned int index )
{
    m_controlNode->node()->inputs()[ index ]->connectSource( *m_connectionSource );

    m_controlMode = kNone;
}
