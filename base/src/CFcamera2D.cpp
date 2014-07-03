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
#include "CFcamera2D.h"

#include "GL/gl.h"
#include "assert.h"


CFcamera2D::CFcamera2D( QObject* parent )
:
    QObject      ( parent   ),
    m_position   ( 0.0, 0.0 ),
    m_orientation( 0.0      ),
    m_scale      ( 0.4      )
{
}

void CFcamera2D::Resize( unsigned int xmax, unsigned int ymax )
{
    assert( xmax < 32768 );
    assert( ymax < 32768 );

    m_xmax = xmax; m_ymax = ymax;

    m_halfScreenSize[0] = ( m_xmax ) / 2.0;
    m_halfScreenSize[1] = ( m_ymax ) / 2.0;

    m_aspectRatio = (double)( m_xmax ) / m_ymax;
    m_worldSize[0] = 1000.0 / m_xmax;
    m_worldSize[1] = m_worldSize[0] * m_aspectRatio;

    glViewport( 0, 0, m_xmax, m_ymax );
}

CFpoint CFcamera2D::ScreenToWorld( const CFpoint& screenPoint ) const
{
    CFpoint worldPoint( screenPoint );

    // revert vertical axis ( OS vs OpenGL )
    worldPoint[1] *= -1.0;
    worldPoint[1] += m_ymax;

    // from center to screen corner
    worldPoint -= m_halfScreenSize;

    // from pixel to world space
    worldPoint /= m_halfScreenSize;
    worldPoint /= m_worldSize;

    // invert camera roll
    worldPoint.Rotate( -m_orientation );

    // invert camera zoom
    worldPoint /= m_scale;

    // invert camera pan
    worldPoint -= m_position;

    return worldPoint;
}

CFpoint CFcamera2D::WorldToScreen( const CFpoint& worldPoint ) const
{
    CFpoint screenPos( worldPoint );

    screenPos += m_position;
    screenPos *= m_scale;
    screenPos.Rotate( m_orientation );
    screenPos *= m_worldSize;
    screenPos *= m_halfScreenSize;
    screenPos += m_halfScreenSize;

    screenPos[1] -= m_ymax;
    screenPos[1] *= -1.0;

    return screenPos;
}

CFcircle CFcamera2D::ScreenToWorld( const CFcircle& screenCircle ) const
{
    CFfloat worldRadius( screenCircle.GetRadius() );

    // from pixel to world space
    worldRadius /= ( m_halfScreenSize[0] + m_halfScreenSize[1] ) / 2.0;
    worldRadius /= ( m_worldSize[0] + m_worldSize[1] ) / 2.0;

    // invert camera zoom
    worldRadius /= m_scale;

    return CFcircle( ScreenToWorld( static_cast<CFpoint>( screenCircle ) ), worldRadius );
}

CFcircle CFcamera2D::WorldToScreen( const CFcircle& worldCircle ) const
{
    CFfloat screenRadius( worldCircle.GetRadius() );

    screenRadius /= m_scale;
    screenRadius /= ( m_worldSize[0] + m_worldSize[1] ) / 2.0;;
    screenRadius /= ( m_halfScreenSize[0] + m_halfScreenSize[1] ) / 2.0;

    return CFcircle( WorldToScreen( static_cast<CFpoint>( worldCircle ) ), screenRadius );
}

void CFcamera2D::WorldView( void ) const
{
    glMatrixMode    ( GL_PROJECTION );
    glLoadIdentity  ();
    glScaled        ( m_worldSize[0], m_worldSize[1], 1.0 );

    glMatrixMode    ( GL_MODELVIEW ); 
    glLoadIdentity  ();
    glRotated       ( m_orientation * RAD2DEG, 0.0, 0.0, 1.0 );
    glScaled        ( m_scale, m_scale, 1.0 );
    glTranslated    ( m_position[0], m_position[1], 0.0 );
}

void CFcamera2D::ScreenView( void ) const
{
    glMatrixMode( GL_PROJECTION );  glLoadIdentity();
    glOrtho( 0.0, m_xmax, m_ymax, 0.0, -1.0, 1.0 );
    glMatrixMode( GL_MODELVIEW  );  glLoadIdentity();
}
