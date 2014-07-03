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
#ifndef __NWgraph_h__
#define __NWgraph_h__

#include "NWnodeLibrary.h"
#include "NWgroup.h"
#include "NWselection.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QXmlStreamReader>

class QString;

//! \brief  Contains all nodes and maintains a selection
//! \author bEngo
//! \date   2008 March 24
//!
class NWgraph : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QObject* nodeLibrary READ nodeLibrary )
    Q_PROPERTY( QObject* selection   READ selection   )

public:

    typedef QMap<QString, NWnode*> Nodes;

public:
                            NWgraph                 ( QObject*              parent          );

    QObject*                nodeLibrary             ( void                                  )       { return &m_nodeLibrary; }
    const NWnodeLibrary&    getNodeLibrary          ( void                                  ) const { return m_nodeLibrary;  }
    NWnodeLibrary&          getNodeLibrary          ( void                                  )       { return m_nodeLibrary;  }

    QObject*                selection               ( void                                  )       { return &m_selection; }
    const NWselection&      getSelection            ( void                                  ) const { return m_selection;  }
    NWselection&            getSelection            ( void                                  )       { return m_selection;  }

    NWnode*                 findNode                ( const QString&        name            ) const;
    NWnode*                 findNode                ( const CFtype&         type            ) const;

    QString                 newUniqueName           ( const QString&        proposition     ) const;

    NWnode*                 createNode              ( CFtype                type,
                                                      const QString&        name        = "",
                                                      NWnode*               parent      = 0,
                                                      bool                  isMandatory = false,
                                                      bool                  isSingleton = false );

    bool                    renameNode              ( NWnode*               node,
                                                      const QString&        newName,
                                                      bool                  autoFixBadName  );

    void                    clear                   ( void                                  );

    void                    saveTo                  ( CFxmlOut&             xml             ) const;
    void                    loadFrom                ( CFxmlIn&              xml             );

signals:

    void                    nodeAdded               ( NWnode*               newNode         );

    void                    selectionChanged        ( const NWselection&    selection       );

    void                    incomingConnectionMade  ( void                                  );
    void                    incomingConnectionBroken( void                                  );

protected:

    NWnodeLibrary           m_nodeLibrary;
    std::set<CFtype>        m_singletonTypes;

    Nodes                   m_nodes;
    NWgroup                 m_rootNode;

    NWselection             m_selection;
};

#endif //__NWgraph_h__
