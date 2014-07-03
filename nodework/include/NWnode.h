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
#ifndef __NWnode_h__
#define __NWnode_h__

#include "NWplug.h"

#include "CFpoint.h"

#include <QtCore/QObject>

#include <vector>

class NWgraph;
class NWinputAttribute;
class NWoutputAttribute;

//! \brief  Base class for nodes. Has input and ouput attributes.
//! \author bEngo
//! \date   2008 March 19
//!
class NWnode : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString name READ name )

public:

    friend class NWgraph;

    enum State
    {
        kAltered,
        kConfigured,
        kEvaluated
    };

    typedef QList<NWplug*>         Plugs;
    typedef QListIterator<NWplug*> PlugIt;

public:
                            NWnode                  (                                               );
    virtual                ~NWnode                  (                                               );

    virtual CFtype          type                    ( void                                          ) const = 0;
    QString                 name                    ( void                                          ) const { return objectName();  }

    bool                    isMandatory             ( void                                          ) const { return m_isMandatory; }
    bool                    isSingleton             ( void                                          ) const { return m_isSingleton; }

    void                    addInput                ( const NWinputAttribute&   attribute           );
    void                    addOutput               ( const NWoutputAttribute&  attribute           );

    const Plugs&            inputs                  ( void                                          ) const { return m_inputs;  }
    Plugs&                  inputs                  ( void                                          )       { return m_inputs;  }
    const Plugs&            outputs                 ( void                                          ) const { return m_outputs; }
    Plugs&                  outputs                 ( void                                          )       { return m_outputs; }

    const NWplug&           input                   ( unsigned int              index               ) const { assert( index < static_cast<unsigned int>( m_inputs.size() ) );  return *m_inputs[ index ];  }
    NWplug&                 input                   ( unsigned int              index               )       { assert( index < static_cast<unsigned int>( m_inputs.size() ) );  return *m_inputs[ index ];  }

    const NWplug&           output                  ( unsigned int              index               ) const { assert( index < static_cast<unsigned int>( m_outputs.size() ) ); return *m_outputs[ index ]; }
    NWplug&                 output                  ( unsigned int              index               )       { assert( index < static_cast<unsigned int>( m_outputs.size() ) ); return *m_outputs[ index ]; }

    NWplug*                 findPlug                ( const QString&            attributeName       );

    unsigned int            inputCount              ( void                                          ) const { return m_inputs.size();    }

    void                    downgradeState          ( State                     state               );

    virtual void            configure               ( void                                          );
    virtual void            evaluate                ( void                                          );

    void                    saveTo                  ( CFxmlOut&                 xml                 ) const;
    void                    loadFrom                ( CFxmlIn&                  xml                 );

    const CFpoint&          visualPosition          ( void                                          ) const { return m_visualPosition;   }
    void                    visualMove              ( const CFpoint&            delta               )       { m_visualPosition += delta; }

signals:

    void                    renamed                 ( const QString&            newName             );
    void                    incomingConnectionMade  ( void                                          );
    void                    incomingConnectionBroken( void                                          );

public slots:

    bool                    rename                  ( const QString&            newName,
                                                      bool                      autoFixBadName      );
protected:

    Plugs                   m_inputs;
    Plugs                   m_outputs;

    State                   m_state;

private:

    NWgraph*                m_graph;
    bool                    m_isMandatory;
    bool                    m_isSingleton;

    QMap<QString,int>       m_plugIndex;

    CFpoint                 m_visualPosition;
};

#endif //__NWnode_h__
