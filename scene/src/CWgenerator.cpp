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
#include "CWscene.h"
#include "CWgenerator.h"

CWgenerator::CWgenerator( CWscene& scene, Type type ) :
    m_scene( scene )
{
    
}

void CWgenerator::AddSeed( CWcell* cell )
{
    m_seeds.Add( cell, 0.0 );
}

void CWgenerator::grow( void )
{
    CFtopoWeightedVect::ElemsIt seedIt = m_seeds.GetElemsBegin();

    while( seedIt != m_seeds.GetElemsEnd() )
    {
        CWcell& seed = *static_cast<CWcell*>( seedIt->GetPoint() );

        CFpoint randVec( rand(), 1.0 );
        randVec.ToCartesian();

        CFfloat newRadius = seed.GetRadius() * 0.7;

        if( newRadius > 0.005 )
        {
            CFpoint newCenter = seed + randVec * ( 1.8 * seed.GetRadius() );

            CWcell* newCell = m_scene.createCell( CFcircle( newCenter, newRadius ), seed.color() );

            //  The new cell become a seed instead of the previous one
            //
            if( newCell ) seedIt->SetPoint( newCell );
        }
        seedIt++;
    }
}
