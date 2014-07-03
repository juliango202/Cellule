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
#include "CWdecomposeColor.h"

NWinputAttribute  CWdecomposeColor::m_colorAttr     ( "color",      CFcolor() );

NWoutputAttribute CWdecomposeColor::m_hueAttr       ( "hue",        CFdata::type<CFfloat>() );
NWoutputAttribute CWdecomposeColor::m_saturationAttr( "saturation", CFdata::type<CFfloat>() );
NWoutputAttribute CWdecomposeColor::m_valueAttr     ( "value",      CFdata::type<CFfloat>() );

CWdecomposeColor::CWdecomposeColor( void )
{
    addInput ( m_colorAttr      );
    addOutput( m_hueAttr        );
    addOutput( m_saturationAttr );
    addOutput( m_valueAttr      );
}

void CWdecomposeColor::evaluate( void )
{
    NWnode::evaluate();

    const CFfloat3& hsv = m_inputs[0]->value<CFcolor>().asHSV();

    m_outputs[0]->setValue<CFfloat>( hsv[0] );
    m_outputs[1]->setValue<CFfloat>( hsv[1] );
    m_outputs[2]->setValue<CFfloat>( hsv[2] );
}
