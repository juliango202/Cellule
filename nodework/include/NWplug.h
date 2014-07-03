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
#ifndef __NWplug_h__
#define __NWplug_h__

#include "NWattribute.h"

#include "CFxmlIO.h"
#include "CFdata.h"

class NWnode;
class NWinputAttribute;
class NWoutputAttribute;

class QGridLayout;

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class NWplug : public QObject
{
    Q_OBJECT

public:

    enum Direction
    {
        kInput,
        kOutput
    };

public:
                                    NWplug                      ( NWnode*                   node,
                                                                  const NWinputAttribute&   attribute       );

                                    NWplug                      ( NWnode*                   node,
                                                                  const NWoutputAttribute&  attribute       );

    const NWnode*                   node                        ( void                                      ) const { return m_node;              }
    const NWattribute*              attribute                   ( void                                      ) const { return m_attribute;         }

    const QString&                  name                        ( void                                      ) const { return m_attribute->name(); }

    Direction                       direction                   ( void                                      ) const { return m_direction;         }

    bool                            isCompatible                ( const NWplug&             plug            ) const;
    bool                            connectSource               ( NWplug&                   sourcePlug      );

    NWplug*                         incomingConnection          ( void                                      ) const { return m_source;            }
    void                            disconnectSource            ( void                                      );

    //  Value interface

    template <typename Type>
    const Type&                     value                       ( void                                      ) const;

    //  Modify the plug value and notify the graph of the change.
    //
    template <typename Type>
    void                            setValue                    ( const Type&               value           );

    //  Return a reference to the plug value.
    //  You must call notifyChange() when finished modifying the value.
    //
    template <typename Type>
    Type&                           editableValue               ( void                                      );
    void                            notifyChange                ( void                                      );

    //  Create a line of widgets for plug name, connection status, plug value, ...
    //  Used to fill the Node Editor.
    //
    void                            createGUI                   ( QGridLayout*              gridLayout      );

    void                            saveTo                      ( CFxmlOut&                 xml             ) const;
    void                            loadFrom                    ( CFxmlIn&                  xml             );

signals:

    void                            incomingConnectionMade      ( void                                      );
    void                            incomingConnectionBroken    ( void                                      );

protected:

    void                            evaluate                    ( void                                      ) const;

    //  Create the kind of widget corresponding to the type of the plug value
    //  and connect the widget to the plug value
    //
    QWidget*                        createWidget                ( QWidget*                  parent          );

protected:

    NWnode*                         m_node;
    const NWattribute*              m_attribute;
    Direction                       m_direction;

    CFdata                          m_data;
    NWplug*                         m_source;       // Null when direction is kOutput
    std::vector<NWplug*>            m_destinations;
};


template <typename Type>
inline const Type& NWplug::value( void ) const
{
    if( m_source ) return m_source->value<Type>();

    if( m_direction == kOutput ) evaluate();

    return m_data.value<Type>();
}

template <typename Type>
inline Type& NWplug::editableValue( void )
{
    //  In that case ( futur modification of the plug value )
    //  the plug should be an unconnected input plug
    //
    assert( m_direction == kInput );
    assert( ! m_source            );

    return m_data.value<Type>();
}

template <typename Type>
inline void NWplug::setValue( const Type& value )
{
    m_data.setValue<Type>( value );

    notifyChange();
}

#endif //__NWplug_h__
