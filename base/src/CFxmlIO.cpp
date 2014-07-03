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
#include "CFxmlIO.h"

const unsigned int CFxmlOut::m_indentSize = 2;

CFxmlIn::TokenType CFxmlIn::readAfterWhitespaces( void )
{
    TokenType tokenType;

    while( ! atEnd() )
    {
        tokenType = readNext();

        if( tokenType != Characters ) break;

        if( ! isWhitespace() ) break;
    }

    return tokenType;
}

CFxmlOut::CFxmlOut( std::ostream& stream )
:
    m_stream     ( stream ),
    m_indentLevel( 0      )
{
}

void CFxmlOut::newLine( void )
{
    m_stream << std::endl;

    unsigned int i = m_indentLevel * m_indentSize;

    while( i-- ) m_stream << " ";
}
