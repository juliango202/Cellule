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
#include "CWcomposeColors.h"

#include "CFenum.h"

static const unsigned int nbOperations               = 2;
static const char*        operations[ nbOperations ] = { "Add", "Multiply" };

NWinputAttribute  CWcomposeColors::m_operationAttr( "operation", CFenum( 0, nbOperations, operations ) );
NWinputAttribute  CWcomposeColors::m_input1Attr   ( "input1",    CFcolor()                             );
NWinputAttribute  CWcomposeColors::m_input2Attr   ( "input2",    CFcolor()                             );

NWoutputAttribute CWcomposeColors::m_outputAttr   ( "output",    CFdata::type<CFcolor>()               );


CWcomposeColors::CWcomposeColors( void )
{
    addInput ( m_operationAttr );
    addInput ( m_input1Attr    );
    addInput ( m_input2Attr    );
    addOutput( m_outputAttr    );
}

void CWcomposeColors::evaluate( void )
{
    NWnode::evaluate();

    int operation = m_inputs[0]->value<CFenum>().asInt();

    if( operation == 0 )
    {
        m_outputs[0]->setValue<CFcolor>( m_inputs[1]->value<CFcolor>() + m_inputs[2]->value<CFcolor>() );
    }
    else if( operation == 1 )
    {
        m_outputs[0]->setValue<CFcolor>( m_inputs[1]->value<CFcolor>() * m_inputs[2]->value<CFcolor>() );
    }
}
