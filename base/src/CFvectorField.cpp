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
#include "CFvectorField.h"

#include "CFtopoPoint.h"
#include "CFtopoWeightedSet.h"

CFvector CFradialField::Sample( const CFpoint& position ) const
{
    return ( position - m_center ) * 0.1f * GetIntensity();
}

//////////////////////////////////////////////////////////////////////////
/*
CFrelaxField::CFrelaxField( const CFtopoWeightedSet& points )
{
    int             nbDist      = 0;
    CFfloat         distances   = 0.0;
    CFtopoPoint*    point;

    CFtopoWeightedSet::ConstElemsIt elemIt = points.GetElemsBegin();
    CFtopoPoint::DirsIt             dirIt;

    while( elemIt != points.GetElemsEnd() )
    {
        point = elemIt->GetPoint();

        dirIt = point->GetDirsBegin();
        while( dirIt != point->GetDirsEnd() )
        {
            distances += dirIt->GetPoint()->Distance( *point ); 
            nbDist++;

            dirIt++;
        }
        elemIt++;
    }

    m_avgDistance = distances / CFfloat( nbDist );
}

CFvector CFrelaxField::Sample( const CFpoint& position  ) const
{
    const CFtopoPoint*  point           = static_cast<const CFtopoPoint*>( &position );
    CFvector            movement        ( 0.0, 0.0 );
    CFvector            toNeighbour;
    CFfloat             distanceDif;

    CFtopoPoint::ConstDirsIt dirIt = point->GetDirsBegin();

    while( dirIt != point->GetDirsEnd() )
    {
        const CFtopoPoint& neighbor = *( dirIt->GetPoint() );

        toNeighbour = neighbor - ( *point );
        distanceDif = point->Distance( neighbor ) - m_avgDistance;

        movement += toNeighbour.Normalize() * distanceDif;

        dirIt++;
    }
    
    return movement * 0.1f * GetIntensity();    
}
*/
//////////////////////////////////////////////////////////////////////////

