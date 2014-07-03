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
#ifndef __NWnodeEditor_h__
#define __NWnodeEditor_h__

#include "NWnode.h"

#include <QtGui/QWidget>

class NWselection;

class QTabWidget;

//! \brief  Graphical user interface to edit node attributes
//! \author bEngo
//! \date   2008 March 24
//!
class NWnodeEditor : public QWidget
{
    Q_OBJECT

public:
                            NWnodeEditor        ( QWidget*              parent    );

    QSize                   sizeHint            ( void                            ) const;

public slots:

    void                    clear               ( void                            );
    void                    addContent          ( NWnode*               node      );
    void                    setContent          ( const NWselection&    selection );

    void                    updateTabTitles     ( void                            );

protected:

    QTabWidget*             m_pages;
    std::vector<NWnode*>    m_nodes;
};

#endif //__NWnodeEditor_h__
