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
#include "CWcolorGradient.h"

#include "CFcolorGradient.h"

#include <assert.h>


NWinputAttribute  CWcolorGradient::m_colorsAttr  ( "colors",   CFcolorGradient() );
NWinputAttribute  CWcolorGradient::m_positionAttr( "position", CFfloat()         );

NWoutputAttribute CWcolorGradient::m_colorAttr   ( "color",    CFdata::type<CFcolor>() );


CWcolorGradient::CWcolorGradient( void )
{
    addInput ( m_colorsAttr   );
    addInput ( m_positionAttr );
    addOutput( m_colorAttr    );
}

void CWcolorGradient::evaluate( void )
{
    NWnode::evaluate();

    const CFfloat& position = m_inputs[1]->value<CFfloat>();

    m_outputs[0]->setValue<CFcolor>( m_inputs[0]->value<CFcolorGradient>().evaluate( position ) );
}
