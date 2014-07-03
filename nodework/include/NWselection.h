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
#ifndef __NWselection_h__
#define __NWselection_h__

#include <QtCore/QObject>
#include <set>

class NWnode;

//! \brief  Hold references to selected nodes
//! \author bEngo
//! \date   2008 March 24
//!
class NWselection : public QObject
{
    Q_OBJECT

public:

    typedef std::set<NWnode*>      Nodes;
    typedef Nodes::const_iterator  NodeConstIt;
    typedef Nodes::iterator        NodeIt;

public:
                            NWselection         ( QObject*            parent = 0            );

    NodeIt                  firstNodeIt         ( void                                      )       { return m_nodes.begin(); }
    NodeConstIt             firstNodeIt         ( void                                      ) const { return m_nodes.begin(); }
    NodeIt                  endNodeIt           ( void                                      )       { return m_nodes.end();   }
    NodeConstIt             endNodeIt           ( void                                      ) const { return m_nodes.end();   }

    void                    clear               ( void                                      );
    void                    set                 ( NWnode*             node                  );
    void                    add                 ( NWnode*             node                  );

    bool                    isEmpty             ( void                                      ) const;
    bool                    has                 ( NWnode*             node                  ) const;

    template< typename T >
    QList<T>                findNodes           ( bool                searchDownToo = true  ) const;

signals:

    void                    changed             ( const NWselection&  selection   );

protected:

    Nodes                   m_nodes;
};

template< typename T >
QList<T> NWselection::findNodes( bool searchDownToo ) const
{
    QList<T> foundNodes;

    NodeIt nodeIt = m_nodes.begin();

    for( ; nodeIt != m_nodes.end(); nodeIt++ )
    {
        T node( qobject_cast<T>( *nodeIt ) );

        if( ! node ) continue;

        foundNodes.append( node );

        if( searchDownToo ) foundNodes += node->findChildren<T>();
    }

    return foundNodes;
}

#endif //__NWselection_h__
