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
#include "CAshaderTool.h"
#include "CAapplication.h"
#include "CWscene.h"
#include "NWselection.h"
#include "CFtopoWeightedSet.h"

void CAshaderTool::activate( void )
{
    NWselection& selection( CAapplication::instance()->getScene().getSelection() );

    connect( &selection, SIGNAL( changed( const NWselection& ) ), SLOT( setShader( const NWselection& ) ) );

    setShader( selection );
}

void CAshaderTool::deactivate( void )
{
    NWselection& selection( CAapplication::instance()->getScene().getSelection() );

    selection.disconnect( SIGNAL( changed( const NWselection& ) ), this, SLOT( setShader( const NWselection& ) ) );
}

void CAshaderTool::setShader( const NWselection& nodes )
{
    //  Find the first shader in the given selection
    //
    QList<CWsurfaceShader*> shaders = nodes.findNodes<CWsurfaceShader*>();

    if( ! shaders.size() ) m_shader = 0;
    else                   m_shader = shaders.at( 0 );
}

void CAshaderTool::click( const CFpoint& position )
{
    move( position );

    if( ! m_shader ) return;

    CWscene& scene( CAapplication::instance()->getScene() );
    
    CFtopoWeightedSet cells( scene.getCells().FindCells( m_shape ) );
    
    CFtopoWeightedSet::ElemsIt cellIt( cells.GetElemsBegin() );
    
    for( ; cellIt != cells.GetElemsEnd(); cellIt++ )
    {
        CWcell& cell = *( static_cast<CWcell*>( cellIt->GetPoint() ) );
        
        cell.setShader( m_shader );
    }
}

void CAshaderTool::drag( const CFpoint& position )
{
    click( position );
}

void CAshaderTool::Draw( void ) const
{
    if( m_shader ) CAtool::Draw();
    else
    {
        glColor3f( 1.0f, 0.0f, 0.0f );
        
        glBegin( GL_LINES );
    
            glVertex2d( m_shape[0] - m_shape.GetRadius(), m_shape[1] + m_shape.GetRadius() );
            glVertex2d( m_shape[0] + m_shape.GetRadius(), m_shape[1] - m_shape.GetRadius() );
    
            glVertex2d( m_shape[0] - m_shape.GetRadius(), m_shape[1] - m_shape.GetRadius() );
            glVertex2d( m_shape[0] + m_shape.GetRadius(), m_shape[1] + m_shape.GetRadius() );
    
        glEnd();
    }
}
