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
#include "CAemitterTool.h"

#include "CAapplication.h"
#include "CWscene.h"

void CAemitterTool::click( const CFpoint& position )
{
#ifdef WIN32
#pragma message("assuming the whole mesh is smaller than that !")
#else
#warning "assuming the whole mesh is smaller than that !"
#endif // WIN32

    m_prevPos    = CFpoint( 10.0, 10.0 );
    m_generator  = new CWgenerator( CAapplication::instance()->getScene(), CWgenerator::kBranch );

    drag( position );
}

void CAemitterTool::drag( const CFpoint& position )
{
    move( position );

    m_generator->grow();

    if( ( m_shape - m_prevPos ).Length() > 2.5 * m_shape.GetRadius() )
    {
        CWcell* newCell = CAapplication::instance()->getScene().createCell( m_shape, m_color );

        if( newCell )   
        {
            m_generator->AddSeed( newCell );
            
            m_prevPos = m_shape;
        }
    }
}

void CAemitterTool::unclick( const CFpoint& position )
{
    delete m_generator;
    m_generator = 0;
}
