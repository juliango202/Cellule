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
#ifndef __NWgraphEditorNode_h__
#define __NWgraphEditorNode_h__

#include "CFpoint.h"
#include "NWnode.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>

class NWgraphEditor;

//! \brief  Widget of the graph editor to display and control an associated node
//! \author bEngo
//! \date   2008 March 24
//!
class NWgraphEditorNode : public QObject
{
    Q_OBJECT

public:

    enum DrawStyle
    {
        kMinimized,
        kExpanded
    };

public:
                            NWgraphEditorNode   ( NWnode*           node            );

    virtual                ~NWgraphEditorNode   (                                   );

    NWnode*                 node                ( void                              )       { return m_node; }

    void                    move                ( const CFpoint&    delta           );

    bool                    intersect           ( const CFpoint&    position        ) const;

    bool                    intersect           ( const CFpoint&    corner1,
                                                  const CFpoint&    corner2         ) const;

    void                    draw                ( NWgraphEditor*    graphEditor,
                                                  bool              isSelected      ) const;
protected:

    CFpoint                 anchorPosition      ( const NWplug&     plug            ) const;

protected:

    QPointer<NWnode>        m_node;

    DrawStyle               m_drawStyle;
    CFpoint                 m_size;
    CFcolor                 m_color;
};

#endif //__NWgraphEditorNode_h__
