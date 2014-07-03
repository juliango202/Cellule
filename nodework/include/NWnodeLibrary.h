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
#ifndef __NWnodeLibrary_h__
#define __NWnodeLibrary_h__

#include "CFdata.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QMap>

class NWnode;

//! \brief  Repertoriate available nodes and handle their creation
//! \author bEngo
//! \date   2008 April 07
//!
class NWnodeLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QStringList typeNames READ typeNames )

public:

    typedef NWnode* ( *Creator )( void );

    typedef QMap<int,Creator> Creators;
    typedef QMap<QString,int> NameIds;

public:
                            NWnodeLibrary       ()               {}

    void                    registerNode        ( CFtype            type,
                                                  Creator           creator     );

    CFtype                  findNodeType        ( const QString&    typeName    ) const;

    QStringList             typeNames           ( void                          ) const;

    //  Create a new node of the given type
    //
    NWnode*                 createNode          ( CFtype            type        ) const;

protected:

    Creators                m_creators;
    NameIds                 m_nameIds;
};

#endif //__NWnodeLibrary_h__
