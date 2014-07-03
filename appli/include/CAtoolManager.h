//  (C) Copyright 2005-2007 Benjamin Godon ( aka bEngo )
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
#ifndef __CAtoolManager_h__
#define __CAtoolManager_h__

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

class CAtool;
class CFpoint;

//! \brief  Manage the user interactive tools
//! \author bEngo
//! \date   2008 March 05
//!
class CAtoolManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QStringList toolNames READ toolNames )

public:
                            CAtoolManager       ( QObject*           parent = 0 );
    virtual                ~CAtoolManager       (                               );

    const QStringList&      toolNames           ( void                          ) const { return m_toolNames;   }
    CAtool*                 getCurrentTool      ( void                          )       { return m_currentTool; }
    void                    setCurrentTool      ( const QString&     toolName   );

public slots:

    void                    setCurrentTool      ( QString*           toolName   );

    void                    clickCurrent        ( const CFpoint&     position   );
    void                    moveCurrent         ( const CFpoint&     position   );
    void                    dragCurrent         ( const CFpoint&     position   );
    void                    unclickCurrent      ( const CFpoint&     position   );

protected:

    QStringList             m_toolNames;
    QMap<QString, CAtool*>  m_tools;
    CAtool*                 m_currentTool;
};

#endif //__CAtoolManager_h__
