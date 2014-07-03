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
#include "CWrenderSettings.h"
#include "CFsurfaceIntegral.h"

#include <assert.h>

static const unsigned int nbTypes               = 2;
static const char*        typesStr[ nbTypes ]   = { "kUniform", "kAdaptive" };

NWinputAttribute  CWrenderSettings::m_antialiasingAttr       ( "antialiasing",        CFenum( 1, nbTypes, typesStr ) );
NWinputAttribute  CWrenderSettings::m_nbSamplesAttr          ( "nbSamples",           int   ( 50                   ), NWinputAttribute::kConfiguration, 4, 80 );
NWinputAttribute  CWrenderSettings::m_useCustomResolutionAttr( "useCustomResolution", bool  ( false                ) );
NWinputAttribute  CWrenderSettings::m_widthAttr              ( "width",               int   ( 200                  ), NWinputAttribute::kConfiguration, 1, 5000 );
NWinputAttribute  CWrenderSettings::m_heightAttr             ( "height",              int   ( 200                  ), NWinputAttribute::kConfiguration, 0, 5000 );


CWrenderSettings::CWrenderSettings( void )
{
    addInput( m_antialiasingAttr        );
    addInput( m_nbSamplesAttr           );
    addInput( m_useCustomResolutionAttr );
    addInput( m_widthAttr               );
    addInput( m_heightAttr              );
}
