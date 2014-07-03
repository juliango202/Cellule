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
#include "CWcomposeReals.h"

#include "CFenum.h"

static const unsigned int nbOperations               = 4;
static const char*        operations[ nbOperations ] = { "Multiply", "Divide", "Add", "Substract" };

NWinputAttribute  CWcomposeReals::m_operationAttr( "operation", CFenum( 0, nbOperations, operations ) );
NWinputAttribute  CWcomposeReals::m_input1Attr   ( "input1",    CFfloat()                             );
NWinputAttribute  CWcomposeReals::m_input2Attr   ( "input2",    CFfloat()                             );

NWoutputAttribute CWcomposeReals::m_outputAttr   ( "output",    CFdata::type<CFfloat>()               );


CWcomposeReals::CWcomposeReals( void )
{
    addInput ( m_operationAttr );
    addInput ( m_input1Attr    );
    addInput ( m_input2Attr    );
    addOutput( m_outputAttr    );
}

void CWcomposeReals::evaluate( void )
{
    NWnode::evaluate();

    int operation = m_inputs[0]->value<CFenum>().asInt();

    if( operation == 0 )
    {
        m_outputs[0]->setValue<CFfloat>( m_inputs[1]->value<CFfloat>() * m_inputs[2]->value<CFfloat>() );
    }
    else if( operation == 1 )
    {
        m_outputs[0]->setValue<CFfloat>( m_inputs[1]->value<CFfloat>() / m_inputs[2]->value<CFfloat>() );
    }
    else if( operation == 2 )
    {
        m_outputs[0]->setValue<CFfloat>( m_inputs[1]->value<CFfloat>() + m_inputs[2]->value<CFfloat>() );
    }
    else if( operation == 3 )
    {
        m_outputs[0]->setValue<CFfloat>( m_inputs[1]->value<CFfloat>() - m_inputs[2]->value<CFfloat>() );
    }
}
