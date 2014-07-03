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
#ifndef __CAapplication_h__
#define __CAapplication_h__

#include "CAtoolManager.h"
#include "CAscriptEngine.h"
#include "CWscene.h"
#include "CFlog.h"

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

class CAview2D;

class CAapplication : public QApplication
{
    Q_OBJECT
    Q_PROPERTY( QString   version       READ version      )
    Q_PROPERTY( QObject*  log           READ log          )
    Q_PROPERTY( QWidget*  mainWin       READ mainWin      )
    Q_PROPERTY( QObject*  scriptEngine  READ scriptEngine )
    Q_PROPERTY( QObject*  toolManager   READ toolManager  )
    Q_PROPERTY( QObject*  scene         READ scene        )

public:

                                CAapplication     ( int&        argc,
                                                    char**      argv      );
    virtual                    ~CAapplication     (                       );

    static CAapplication*       instance          ( void                  );

//  SCRIPTING INTERFACE
//  ( PROPERTIES )
//
public:

    QString&                    version           ( void                  ) { return m_version; }
    QObject*                    log               ( void                  ) { return CFlog::instance(); }
    QWidget*                    mainWin           ( void                  ) { return m_mainWin;         }
    QObject*                    scriptEngine      ( void                  ) { return m_scriptEngine;    }
    QObject*                    toolManager       ( void                  ) { return &m_toolManager;    }
    QObject*                    scene             ( void                  ) { return &m_scene;          }

//  SCRIPTING INTERFACE
//  ( METHODS )
//
public slots:

    void                        setTitle          ( QString*    fileName  );
    QWidget*                    createView2D      ( QWidget*    parent    );
    QWidget*                    createGraphEditor ( QWidget*    parent    );
    QWidget*                    createNodeEditor  ( QWidget*    parent    );
    void                        about             ( void                  );

//  C++ INTERFACE
//
public:
    
    const CAtoolManager&        getToolManager    ( void                  ) const { return m_toolManager; }
    CAtoolManager&              getToolManager    ( void                  )       { return m_toolManager; }

    const CWscene&              getScene          ( void                  ) const { return m_scene;       }
    CWscene&                    getScene          ( void                  )       { return m_scene;       }

    int                         exec              ( void                  );

protected:

    void                        initGUI           ( void                  );

protected:

    static QString              m_version;

    QMainWindow*                m_mainWin;
    CAscriptEngine*             m_scriptEngine;
    CAtoolManager               m_toolManager;

    CWscene                     m_scene;
};


#endif //__CAapplication_h__
