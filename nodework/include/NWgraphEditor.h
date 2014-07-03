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
#ifndef __NWgraphEditor_h__
#define __NWgraphEditor_h__

#include "NWgraph.h"
#include "NWgraphEditorNode.h"

#include "CFview2D.h"

#include <QtCore/QList>
#include <QtCore/QPointer>

class NWnode;

//! \brief  Draws and give control on nodes and connections
//! \author bEngo
//! \date   2008 March 24
//!
class NWgraphEditor : public CFview2D
{
    Q_OBJECT

public:

    typedef QList<NWgraphEditorNode*> Nodes;

public:
                                NWgraphEditor           ( QWidget*          parentWidget,
                                                          NWgraph*          graph           );

    virtual                    ~NWgraphEditor           (                                   );

    NWgraph*                    graph                   ( void                              )           { return m_graph; }
    QSize                       sizeHint                ( void                              ) const;

    void                        paintGL                 ( void                              );

    virtual void                mouseClick              ( void                              );
    virtual void                mouseMove               ( void                              );
    virtual void                mouseDrag               ( void                              );
    virtual void                mouseUnclick            ( void                              );

    NWgraphEditorNode*          findNode                ( const NWnode*     node            );
    NWgraphEditorNode*          findNode                ( const CFpoint&    position        );

    Nodes                       findNodes               ( const CFpoint&    corner1,
                                                          const CFpoint&    corner2         );

public slots:

    void                        clearContent            ( void                              );
    void                        setContent              ( NWnode*           node            );
    void                        addContent              ( NWnode*           node            );

private slots:

    void                        beginConnection         ( unsigned int      index           );
    void                        endConnection           ( unsigned int      index           );

protected:

    enum                        ControlMode
    {
        kNone,
        kSelect,
        kMoveSelection,
        kConnect
    };

protected:

    QPointer<NWgraph>           m_graph;
    Nodes                       m_content;

    ControlMode                 m_controlMode;
    NWgraphEditorNode*          m_controlNode;

    NWplug*                     m_connectionSource;
};

#endif //__NWgraphEditor_h__
