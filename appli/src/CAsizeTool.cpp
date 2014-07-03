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
#include "CAsizeTool.h"

#include "CAapplication.h"
#include "CWscene.h"
#include "CFtopoMesh.h"


void CAsizeTool::click( const CFpoint& position )
{
    move( position );
    
    CFtopoMesh& cellWorld = CAapplication::instance()->getScene().getCells();
    
    CFtopoWeightedSet brushCells( cellWorld.FindCells( m_shape ) );
    
    CFtopoWeightedSet::ElemsIt cellIt = brushCells.GetElemsBegin();
    
    while( cellIt != brushCells.GetElemsEnd() )
    {
        CFtopoWeightedElem& elem = const_cast<CFtopoWeightedElem&>( *cellIt );
        
        elem.SetWeight( 1.0 + 0.1f * intensity() * elem.GetWeight() );
        
        cellIt++;
    }
    
    cellWorld.Scale( brushCells );
    cellWorld.Relax( brushCells );
}

void CAsizeTool::drag( const CFpoint& position )
{
    click( position );
}
