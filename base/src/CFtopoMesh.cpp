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
#include "CFtopoMesh.h"
#include "CFsegment.h"
#include "CFcircle.h"

#include <algorithm>
#include <fstream>
#include <deque>
#include <utility>

#include "assert.h"

CFtopoMesh::CFtopoMesh()
{
    m_master1 = new CFtopoPoint( -100.0, -50.0, 0.2 );
    m_master2 = new CFtopoPoint(  100.0, -50.0, 0.2 );
    m_master3 = new CFtopoPoint(    0.0, 100.0, 0.2 );

    Clear();
}

CFtopoMesh::~CFtopoMesh()
{
    Clear();

    delete m_master1;
    delete m_master2;
    delete m_master3;
}

void CFtopoMesh::Clear( void )
{
    SetCheck( true );

    m_points.clear();

    m_master1->Clear();
    m_master2->Clear();
    m_master3->Clear();

    m_master1->Link( m_master2 );
    m_master2->Link( m_master3 );
    m_master3->Link( m_master1 );

    m_curPoint = m_master1;
}

bool CFtopoMesh::Insert( CFtopoPoint& point )
{
    if( ! Link( point ) ) return false;

    m_points.insert( &point );

    return true;
}

void CFtopoMesh::Remove( CFtopoPoint& point )
{
    UnLink( point );

    m_points.erase( &point );
}

bool CFtopoMesh::Link( CFtopoPoint& newPoint )
{
    //  Find insertion area ( a triangle )
    //
    CFtopoTriangle triangle = FindTriangle( newPoint );

    if( m_check )
    {
        //  Pull the new point toward the triangle center
        //  to be sure it isn't on an edge
        //
        //CFpoint triCenter( triangle.GetCenter() );
        CFpoint newPos( triangle.GetCenter() + ( newPoint - triangle.GetCenter() ) * 0.9999f );
        //newPoint.SetPosition(  );

        //  Test if there is a place for the new cell !
        //
        if( FindCell( newPoint )->Collide( newPoint ) )
            return false;

        newPoint.SetPosition( newPos );
    }

    //  Insert the point in the triangle
    //
    newPoint.Link( triangle.p1 );
    newPoint.Link( triangle.p2 );
    newPoint.Link( triangle.p3 );

    //  Apply Delauney mesh constraint around
    //
    RecurseDelauney( &newPoint, triangle.p1, triangle.p2 );
    RecurseDelauney( &newPoint, triangle.p2, triangle.p3 );
    RecurseDelauney( &newPoint, triangle.p3, triangle.p1 );

    return true;
}

void CFtopoMesh::RecurseDelauney( CFtopoPoint* newPoint, CFtopoPoint* edgeStart, CFtopoPoint* edgeEnd )
{
    //  Search the point opposite to newPoint
    //  on the other side of the edge
    //
    CFtopoPoint*    oppositePoint = edgeStart->FindOpposite( edgeEnd, newPoint );

    if( ! oppositePoint ) return;

    //  Delauney in-circumcircle test
    //
    CFcircle    circumCircle( *newPoint, *edgeStart, *edgeEnd );

    if( circumCircle.PowDist( *oppositePoint ) < 0.0 )
    {
        //  Swap edge
        //
        edgeStart->UnLink( edgeEnd );
        newPoint->Link( oppositePoint );

        //  Propagate Delauney mesh constraint
        //
        RecurseDelauney( newPoint, edgeStart, oppositePoint );
        RecurseDelauney( newPoint, edgeEnd, oppositePoint );
    }
}

void CFtopoMesh::UnLink( CFtopoPoint& point )
{
    //  Unlink all neighbours
    //
    CFtopoPoint::DirsIt otherDir = point.GetDirsBegin();
    std::vector<CFtopoPoint*> neighbours;

    while( otherDir != point.GetDirsEnd() )
    {
        otherDir->GetPoint()->UnLink( &point, false );
        //point.UnLink( otherDir->GetPoint() );
        neighbours.push_back( otherDir->GetPoint() );
        otherDir++;
    }

    point.Clear();

    //  Find another current point
    //
    if( m_curPoint == &point )
    {
        m_curPoint = neighbours[0];
    }

    //  Triangulate the resulting polygon
    //
    int i = 0;
    int nbp = neighbours.size();

    while( nbp > 3 )
    {
        int ipoint  = i % nbp;
        int iedge   = ( i + 1 ) % nbp;
        int icord   = ( i + 2 ) % nbp;

        CFvector edge( *( neighbours[iedge] ) - *( neighbours[ipoint] ) );
        CFvector cord( *( neighbours[icord] ) - *( neighbours[ipoint] ) );

        if( ( edge^cord ) > 0.0 )
        {
            CFcircle circumCircle( *( neighbours[ipoint] ), *( neighbours[iedge] ), *( neighbours[icord] ) );

            bool valid = true;

            int j = nbp;
            while( j-- )
            {
                if( j != ipoint && j != iedge && j != icord )
                {
                    if( circumCircle.PowDist( *neighbours[j] ) < 0.0 )
                        valid = false;
                }
            }

            if( valid )
            {
                neighbours[ipoint]->Link( neighbours[icord] );

                neighbours.erase( neighbours.begin() + iedge );

                nbp--;
            }
        }

        i++;
    }
}

void CFtopoMesh::move( CFtopoPoint& point, const CFvector& movement )
{
    CFpoint prev = point;

    UnLink( point );

    point += movement;

    if( ! Link( point ) )
    {
        // Now we know that the move is invalid
        // so insert back the point where it was
        //
        point.SetPosition(prev);

        SetCheck( false );
        assert( Link(point) );
        SetCheck( true );
    }
}

void CFtopoMesh::move( CFtopoWeightedSet& points, const CFvectorField& vectorField )
{
    CFtopoWeightedSet::ElemsIt elemsIt = points.GetElemsBegin();

    while( elemsIt != points.GetElemsEnd() )
    {
        const CFtopoWeightedElem&   elem    = *elemsIt;
        CFtopoPoint&                point   = *elem.GetPoint();

        //  Compute vector for this point
        //
        CFvector movement = vectorField.Sample( point );
        
        //  move this point
        //
        move( point, movement * elem.GetWeight() );

        elemsIt++;
    }
}

void CFtopoMesh::Scale( CFtopoPoint& point, float factor )
{
    CFcircle prev = point;

    factor *= factor;   // Since working on a squared radius

    point.SetRadius2( point.GetRadius2() * factor );

    UnLink( point );

    if( ! Link( point ) )
    {
        // Now we know that the scale is invalid
        // so insert back the point with the original scale
        //
        point.SetRadius2( prev.GetRadius2() );

        SetCheck( false );
        assert( Link(point) );
        SetCheck( true );
    }
}

void CFtopoMesh::Scale( CFtopoWeightedSet& points )
{
    CFtopoWeightedSet::ElemsIt pointIt = points.GetElemsBegin();

    while( pointIt != points.GetElemsEnd() )
    {
        CFtopoPoint& point = *( pointIt->GetPoint() );

        Scale( point, pointIt->GetWeight() );

        pointIt++;
    }
}

CFtopoTriangle CFtopoMesh::FindTriangle( const CFpoint& point ) const
{
    CFtopoTriangle triangle;
    triangle.p1 = 0;
    triangle.p2 = 0;
    triangle.p3 = 0;

    CFtopoPoint* point4 = 0;

    while( m_curPoint != triangle.p3 )
    {
        point4 = triangle.p3;
        triangle.p3 = triangle.p2;
        triangle.p2 = triangle.p1;
        triangle.p1 = m_curPoint;
        m_curPoint = m_curPoint->FindDirection( point )->GetPoint();

        if( m_curPoint == triangle.p2 )
        {
            //  Found an edge: return one of the 2 triangles
            //  that share this edge
            //
            triangle.p3 = triangle.p1->FindOpposite( triangle.p2 );
            break;
        }
        if( m_curPoint == point4 )
        {
            //  Walk on a quad: return one of the 2 triangles
            //  of this quad
            //
            break;
        }
    }
    return triangle;
}

CFtopoPoint* CFtopoMesh::FindCell( const CFpoint& point, CFfloat* resultDistance ) const
{
    CFtopoWeightedVect  neighbors;
    CFfloat             d0;
    CFfloat             d1 = m_curPoint->PowDist( point );

    neighbors.Add( m_curPoint, 0.0 );

    //  Find the cell we are in
    //
    do
    {
        m_curPoint = neighbors[0].GetPoint();

        neighbors.Clear();

        d0 = d1;

        //  Sort neighbors by power distance
        //
        CFtopoPoint::DirsIt neighbor = m_curPoint->GetDirsBegin();

        while( neighbor != m_curPoint->GetDirsEnd() )
        {
            CFtopoPoint* newPoint = neighbor->GetPoint();

            neighbors.Add( newPoint, newPoint->PowDist( point ) );

            neighbor++;
        }
        neighbors.SortByWeight();

        d1 = neighbors[0].GetWeight();
    }
    while( d1 < d0 );

    CFassert( ! IsMaster( *m_curPoint ), "on est dans une cellule master !" )

    if( resultDistance ) *resultDistance = d0;

    return m_curPoint;
}

CFtopoWeightedSet CFtopoMesh::FindCells( const CFcircle& area ) const
{
    CFtopoWeightedSet cells;
    
    RecurseSearch( area, FindCell( area ), cells );
    
    return cells;
}

void CFtopoMesh::RecurseSearch( const CFcircle& area, CFtopoPoint* start, CFtopoWeightedSet& result ) const
{
    CFfloat distance = area.Distance2( *start );

    if( distance < area.GetRadius2() )
    {
        result.Add( start, 1.0 - distance / area.GetRadius2() );

        CFtopoPoint::DirsIt dirIt = start->GetDirsBegin();
        while( dirIt != start->GetDirsEnd() )
        {
            if( ! result.IsIn( (*dirIt).GetPoint() ) )
            {
                RecurseSearch( area, (*dirIt).GetPoint(), result );
            }
            dirIt++;
        }
    }
}

void CFtopoMesh::Relax( CFtopoWeightedSet& points )
{
    CFpoint                     randVect;
    CFtopoPoint::DirsIt         dirIt;

    //  Force relax movements
    //
    SetCheck( false );

    // For each point
    //
    CFtopoWeightedSet::ElemsIt  elemIt = points.GetElemsBegin();
    while( elemIt != points.GetElemsEnd() )
    {
        CFtopoPoint& point = *elemIt->GetPoint();

        //  Compute the relax movement
        //
        CFpoint movement;

        //  Add each neighbor attraction / repulsion vector
        //
        dirIt = point.GetDirsBegin();
        while( dirIt != point.GetDirsEnd() )
        {
            CFtopoPoint& neighbor = *dirIt->GetPoint(); dirIt++;

            //  Skip points of the master triangle
            //
            if( IsMaster( neighbor) ) continue;

            CFpoint vect    = neighbor - point;
            vect.Normalize();

            CFfloat dist    = point.Distance( neighbor );

            if( dist < 0.0 )
            {
                movement += vect * dist / 2.0;
            }
            else
            {
                movement += vect * 0.0001 / ( 0.1 + dist );
            }
        }

        //  Add a 1% perturbation ( allowing to leave blocked situations )
        //
        //randVect[0] = float( rand() ) / RAND_MAX;
        //randVect[1] = float( rand() ) / RAND_MAX;
        //move += randVect * ( move.Length() * 0.01f );

        //  Apply the relaxation move
        //
        move( point, movement * elemIt->GetWeight() );

        elemIt++;
    }

    SetCheck( true );
}

bool CFtopoMesh::IsMaster( const CFtopoPoint& point ) const
{
    if( &point == m_master1 ) return true;
    if( &point == m_master2 ) return true;
    if( &point == m_master3 ) return true;

    return false;
}

//////////////////////////////////////////////////////////////
//
//  openGL drawing 
//
void CFtopoMesh::DrawDelauney( void ) const
{
    //   Draw edges between voronoi cell centers
    //
    CFtopoPoint::DirsIt     edge;
    ConstPointsIt           pointIt;

    glBegin( GL_LINES );    

    pointIt = GetPointsBegin();
    while( pointIt != GetPointsEnd() )
    {
        CFtopoPoint& feature = **pointIt;

        edge = feature.GetDirsBegin();

        while( edge != feature.GetDirsEnd() )
        {
            glVertex2f( feature[0], feature[1] );

            const CFpoint&  edgeEnd = ( *edge->GetPoint() );

            edge++;

            glVertex2f( edgeEnd[0], edgeEnd[1] );
        }

        pointIt++;
    }

    glEnd();
}

void CFtopoMesh::DrawPoints( void ) const
{
    //  Draw voronoi cell centers
    //
    CFtopoPoint::DirsIt     edge;
    ConstPointsIt           pointIt;

    pointIt = GetPointsBegin();
    while( pointIt != GetPointsEnd() )
    {
        (*pointIt)->Draw();     pointIt++;
    }
}
