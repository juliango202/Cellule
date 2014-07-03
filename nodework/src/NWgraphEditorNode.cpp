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
#include "NWgraphEditorNode.h"

#include "NWgraph.h"
#include "NWgraphEditor.h"
#include "NWselection.h"

NWgraphEditorNode::NWgraphEditorNode( NWnode* node )
:
    m_node      ( node            ),
    m_drawStyle ( kMinimized      ),
    m_size      ( 0.5, 0.15       ),
    m_color     ( CFcolor::kGray  )
{
    if( node->isMandatory() ) m_color.setRGB( 0.65, 0.5, 0.3 );
}

NWgraphEditorNode::~NWgraphEditorNode()
{
}

void NWgraphEditorNode::move( const CFpoint& delta )
{
    m_node->visualMove( delta );
}

bool NWgraphEditorNode::intersect( const CFpoint& position ) const
{
    const CFpoint& nodePos( m_node->visualPosition() );

    return
    (
        position[0] > nodePos[0]             &&
        position[1] > nodePos[1]             &&
        position[0] < nodePos[0] + m_size[0] &&
        position[1] < nodePos[1] + m_size[1]
    );
}

bool NWgraphEditorNode::intersect( const CFpoint& corner1, const CFpoint& corner2 ) const
{
    if( ! m_node ) return false;

    const CFpoint& nodePos( m_node->visualPosition() );

    CFpoint corner( nodePos + m_size );

    if( corner1[0] < nodePos[0] && corner2[0] < nodePos[0] ) return false;
    if( corner1[0] > corner[0]  && corner2[0] > corner[0]  ) return false;
    if( corner1[1] < nodePos[1] && corner2[1] < nodePos[1] ) return false;
    if( corner1[1] > corner[1]  && corner2[1] > corner[1]  ) return false;

    return true;
}

void NWgraphEditorNode::draw( NWgraphEditor* graphEditor, bool isSelected ) const
{
    if( ! m_node ) return;

    const CFpoint& nodePos( m_node->visualPosition() );

    CFcolor color( m_color * ( isSelected ? 1.4 : 1.0 ) );

    color.Draw();

    glBegin( GL_QUADS );

    glVertex2f( nodePos[0],             nodePos[1]             );
    glVertex2f( nodePos[0],             nodePos[1] + m_size[1] );
    glVertex2f( nodePos[0] + m_size[0], nodePos[1] + m_size[1] );
    glVertex2f( nodePos[0] + m_size[0], nodePos[1]             );

    glEnd();

    color *= 1.4; color.Draw();

    glBegin( GL_LINE_LOOP );

    glVertex2f( nodePos[0],             nodePos[1]             );
    glVertex2f( nodePos[0],             nodePos[1] + m_size[1] );
    glVertex2f( nodePos[0] + m_size[0], nodePos[1] + m_size[1] );
    glVertex2f( nodePos[0] + m_size[0], nodePos[1]             );

    glEnd();

    glColor3f( 1.0f, 1.0f, 1.0f );

    graphEditor->renderText( nodePos[0] + 0.1, nodePos[1] + 0.05, 0.0, m_node->name() );

    //  Draw incoming connections
    //
    //  drawing incoming connections only is enough ( and simpler )
    //  we get connections from both outputs and other inputs too
    //
    NWnode::PlugIt inputIt( m_node->inputs() );
    
    while( inputIt.hasNext() )
    {
        const NWplug* plug( inputIt.next() );

        assert( plug->direction() == NWplug::kInput );

        CFpoint destinationAnchor( anchorPosition( *plug ) );
    
        NWplug* source( plug->incomingConnection() );
    
        if( source )
        {
            //  Look if the node where the connection come from
            //  is in the graph editor node
            //
            NWgraphEditorNode* sourceNode( graphEditor->findNode( source->node() ) );
    
            if( sourceNode )
            {
                CFpoint sourceAnchor( sourceNode->anchorPosition( *source ) );

                glColor3f( 0.9f, 0.9f, 0.4f );
                
                glBegin( GL_LINES );
                
                sourceAnchor.Draw();
                destinationAnchor.Draw();
                
                glEnd();
            }
        }
    }

}

CFpoint NWgraphEditorNode::anchorPosition( const NWplug& plug ) const
{
    assert( m_node );

    const CFpoint& nodePos( m_node->visualPosition() );

    return CFpoint
    (
        nodePos[0] + m_size[0] * 0.5,
        nodePos[1] + ( plug.direction() == NWplug::kInput ? m_size[1] : 0.0 )
    );
}
