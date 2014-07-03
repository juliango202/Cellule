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
#include "CWsurfaceShader.h"

#include "CFenum.h"

#include <assert.h>


NWinputAttribute  CWsurfaceShader::m_colorAttr        ( "color",         CFcolor::kWhite );
NWinputAttribute  CWsurfaceShader::m_heightAttr       ( "height",        CFfloat( 0.0 )  );
NWinputAttribute  CWsurfaceShader::m_hwRenderColorAttr( "hwRenderColor", CFcolor::kWhite );
NWinputAttribute  CWsurfaceShader::m_smoothnessAttr   ( "smoothness",    CFfloat( 0.0 )  );

CWsurfaceShader::CWsurfaceShader( void )
{
    addInput( m_colorAttr         );
    addInput( m_heightAttr        );
    addInput( m_hwRenderColorAttr );
    addInput( m_smoothnessAttr    );
}
