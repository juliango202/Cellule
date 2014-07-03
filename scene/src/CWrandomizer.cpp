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
#include "CWrandomizer.h"

NWinputAttribute  CWrandomizer::m_seedAttr       ( "seed",        int( 0 ), NWinputAttribute::kConfiguration );
NWoutputAttribute CWrandomizer::m_randomFloatAttr( "randomFloat", CFdata::type<CFfloat>()                    );

CWrandomizer::CWrandomizer()
{
    addInput ( m_seedAttr        );
    addOutput( m_randomFloatAttr );

    m_generator = gsl_rng_alloc( gsl_rng_taus );
}

CWrandomizer::~CWrandomizer()
{
    gsl_rng_free( m_generator );
}

void CWrandomizer::configure( void )
{
    NWnode::configure();

    gsl_rng_set( m_generator, m_inputs[0]->value<int>() );
}

void CWrandomizer::evaluate( void )
{
    NWnode::evaluate();

    m_outputs[0]->setValue<CFfloat>( gsl_rng_uniform( m_generator ) );

    //  Force another evaluation
    //  each time randomFloat output is queried
    //
    m_state = kConfigured;
}
