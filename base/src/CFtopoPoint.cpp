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
#include "CFtopoPoint.h"
#include "CFcircle.h"

#include "assert.h"

#include <algorithm>

void CFtopoPoint::Clear( void )
{
    m_directions.clear();
    m_voronoi.clear();
    m_bspline.clear();
}

void CFtopoPoint::Link( CFtopoPoint* neighbor, bool bothSide )
{   
    CFpoint vec( *neighbor - *this );
    vec.ToPolar();

    m_directions.push_back( CFtopoDirection( vec[0], neighbor ) );

    std::sort( m_directions.begin(), m_directions.end() );

    if( bothSide )
    {
        neighbor->Link( this, false );
    }

    m_bspline.clear();
    m_voronoi.clear();
}

void CFtopoPoint::UnLink( CFtopoPoint* neighbor, bool bothSide )
{
    DirsIt otherDir = std::find_if( m_directions.begin(), m_directions.end(), CFtopoDirectionMatchPoint( neighbor ) );

    if( otherDir == m_directions.end() )
    {
        assert( 0 );
        return;
    }

    m_directions.erase( otherDir );

    if( bothSide )
    {
        neighbor->UnLink( this, false );
    }

    m_bspline.clear();
    m_voronoi.clear();
}

CFtopoPoint::DirsIt CFtopoPoint::FindDirection( const CFpoint& position )
{
    CFpoint vectDirection( position - *this );
    vectDirection.ToPolar();

    DirsIt goodDirection = std::lower_bound( m_directions.begin(), m_directions.end(), vectDirection[0] );

    if( goodDirection != m_directions.end() )
    {
        return goodDirection;
    }

    return m_directions.begin();
}

const std::vector<CFpoint>& CFtopoPoint::VoronoiVertices( void ) const
{
    if( m_voronoi.empty() )
    {
        ConstDirsIt edge( GetDirsBegin() );

        do
        {
            const CFcircle&	neighborTopoPoint = *edge->GetPoint();

            edge++; if( edge == GetDirsEnd() ) edge = GetDirsBegin();

            CFcircle circum( *this, neighborTopoPoint, *edge->GetPoint() );

            m_voronoi.push_back( circum );
        }
        while( edge != GetDirsBegin() );
    }

    return m_voronoi;
}

CFtopoPoint* CFtopoPoint::FindOpposite( const CFtopoPoint* edgeEnd, const CFtopoPoint* neighbor )
{
    //  Invalid point check
    //
    assert( m_directions.size() > 1 );

    //  Corner test
    //
    if( m_directions.size() == 2 )
    {
        return 0;   // no opposite
    }

    //  Find edge
    //
    DirsIt edgeDir = std::find_if( m_directions.begin(), m_directions.end(), CFtopoDirectionMatchPoint( edgeEnd )   );

    if( edgeDir == m_directions.end() )
    {
        assert( 0 );
        return 0;
    }

    //  Look for the opposite on next direction
    //
    DirsIt nextEdgeDir = edgeDir;
    nextEdgeDir++;

    if( nextEdgeDir == m_directions.end() )
    {
        nextEdgeDir = m_directions.begin();
    }

    CFtopoPoint* opposite = ( *nextEdgeDir ).GetPoint();

    assert( opposite );

    if( opposite != neighbor )
    {
        return opposite;
    }

    //  Else look for the opposite on previous direction
    //
    if( edgeDir == m_directions.begin() )
    {
        edgeDir = m_directions.end();
    }

    edgeDir--;

    assert( edgeDir != m_directions.end() );

    opposite = ( *edgeDir ).GetPoint();

    return opposite;
}



// ----------------------------------------------------------------------- //
//|                            BSPLINE CONTOUR                            |//
// ----------------------------------------------------------------------- //

struct CFsegment  {

    CFsegment( CFpoint _p1, CFpoint _p2, float& accLen ) : p1(_p1), p2(_p2), accLen1(accLen) { accLen += (p2-p1).Length(); accLen2 = accLen; }
    CFpoint interp( float t ) { if( t >= accLen1 && t < accLen2 ) return (p1 * (accLen2-t) + p2 * (t-accLen1))/(accLen2-accLen1); else return 0; }
    CFpoint p1, p2;
    float accLen1, accLen2;
};


const CFbspline& CFtopoPoint::BSplineContour( CFfloat smoothness ) const {

    if( m_bspline.getSmoothness() != smoothness )
    {
        m_bspline.clear();

        // Control points are sampled uniformely along the voronoi polygon
        // The more smoothness we want the less we add control points 
        std::vector<CFpoint> v( VoronoiVertices() );
        unsigned int vCount = v.size();
        float nbSamples = std::max((1.0 - smoothness) * 3 * vCount, 3.0);

        // Compute polygon segments length
        std::vector<CFsegment> seg;
        float accLen = 0;
        for( unsigned int i = 0; i < vCount; i++ )
            seg.push_back( CFsegment( v[i] , v[(i+1)%vCount], accLen ) );
    
        // Sample points at equal distance
        float step = accLen/nbSamples + std::numeric_limits<float>::epsilon();
        for( float len = step/2; len < accLen; len += step ) {
    
            CFpoint p(0,0);
            for( unsigned int i = 0; i < vCount; i++ )
                p += seg[i].interp( len );
    
            m_bspline.addControlPoint( p );
        }
    
        m_bspline.close();
        assert( m_bspline.is_valid() );

        m_bspline.setSmoothness( smoothness );
        m_bspline.setCenter( *this );
    }

    return m_bspline;
}


CFfloat CFtopoPoint::DistanceToCenter( const CFpoint& point, CFfloat smoothness ) const
{
    CFfloat centerDistance;

    if( smoothness == 0 ) 
    {
        const CFpoint& cellCenter = *this;
        std::vector<CFpoint> v( VoronoiVertices() );
        int vCount = v.size();
        int i = vCount; while( i-- ) v[i] -= cellCenter;

        CFpoint p( point - cellCenter );
        CFpoint  v1, v2;
        bool     intersectionFound( false );
        CFfloat  coordOnP;
        CFfloat  coordOnV1V2;
        i = vCount; while( i-- )
        {
            //  Take two consecutive voronoi vertices ( the current one and the next one )
            //
            v1 = v[i];
    
            if( i == 0 ) v2 = v[ vCount - 1 ];
            else         v2 = v[ i - 1 ];
    
            //  Compute intersection of center
            //
            CFfloat denominator = ( v2[1] - v1[1] ) * p[0] - ( v2[0] - v1[0] ) * p[1];
            if( denominator == 0 ) continue;
    
            coordOnV1V2 = ( ( p[0] * -v1[1] ) - ( p[1] * -v1[0] ) ) / denominator;
    
            coordOnP    = ( ( v2[0] - v1[0] ) * ( -v1[1] ) - ( v2[1] - v1[1] ) * ( -v1[0] ) ) / denominator;
    
            if( coordOnV1V2 >= 0.0 && coordOnV1V2 <= 1.0 && coordOnP >= 0.0 )
            {
                intersectionFound = true;
                break;
            }
        }
    
        assert( intersectionFound );
        if( !intersectionFound ) return CFfloat(0);

        CFpoint proj = cellCenter + (p * coordOnP);
        assert( (proj - cellCenter).Length() != 0 );

        centerDistance = (point - cellCenter).Length() / (proj - cellCenter).Length();
    }
    else
    {
        const CFbspline& bspline = BSplineContour(smoothness);
        CFpoint proj = bspline.evalIntersect( point );
        assert( (proj - bspline.getCenter()).Length() != 0);

        centerDistance = (point - bspline.getCenter()).Length()  / (proj - bspline.getCenter()).Length();
    }

    if( centerDistance > 1.0 ) centerDistance = 1.0;
    return centerDistance;
}

