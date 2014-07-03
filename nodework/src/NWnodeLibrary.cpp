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
#include "NWnodeLibrary.h"

#include "NWnode.h"

#include <QtCore/QString>

void NWnodeLibrary::registerNode( CFtype type, Creator creator )
{
    m_creators[ type.id()   ] = creator;
    m_nameIds [ type.name() ] = type.id();
}

CFtype NWnodeLibrary::findNodeType( const QString& typeName ) const
{
    NameIds::const_iterator nameIdIt = m_nameIds.find( typeName );

    if( nameIdIt == m_nameIds.end() ) return CFtype::kInvalid;

    return CFtype( nameIdIt.value(), typeName );
}

QStringList NWnodeLibrary::typeNames( void ) const
{
    QStringList typeNames;
    
    NameIds::const_iterator nameIdIt = m_nameIds.begin();
    
    while( nameIdIt != m_nameIds.end() )
    {
        typeNames.append( nameIdIt.key() );
        nameIdIt++;
    }
    
    return typeNames;
}

NWnode* NWnodeLibrary::createNode( CFtype type ) const
{
    Creators::const_iterator CreatorIt = m_creators.find( type.id() );

    if( CreatorIt == m_creators.end() ) return 0;

    return CreatorIt.value()();
}
