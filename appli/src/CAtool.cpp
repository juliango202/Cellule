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
#include "CAtool.h"

#include "CAapplication.h"
#include "CWcamera2D.h"
#include "CWscene.h"
#include "CFcircle.h"

CAtool::CAtool( const QString& name ) :
    m_name      ( name          ),
    m_shape     ( 0.0, 0.0, 0.1 ),
    m_intensity ( 1.0           ),
    m_screenPos ( 0.0, 0.0      )
{
}

void CAtool::Scale( float factor )
{
    m_shape.GetRadius2() *= factor;
}

void CAtool::Draw( void ) const
{
    float intensNorm = 0.5f + m_intensity * 0.5f;;

    glColor3f( 1.0f - intensNorm, intensNorm, 0.0f );
    
    m_shape.Draw();
}
