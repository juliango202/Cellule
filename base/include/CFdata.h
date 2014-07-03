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
#ifndef __CFdata_h__
#define __CFdata_h__

#include "CFtypes.h"
#include "CFcolorGradient.h"

#include <QtCore/QMap>
#include <QtGui/QWidget>

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CFtype
{
public:

    static const CFtype kInvalid;

public:
                    CFtype      ( int               id,
                                  const QString&    name    )       : m_id( id ), m_name( name )     {}

    bool            isValid     ( void                      ) const { return m_id > 0; }
    int             id          ( void                      ) const { return m_id;     }
    const QString&  name        ( void                      ) const { return m_name;   }

private:
    int             m_id;
    QString         m_name;
};

inline bool operator<( const CFtype& type1, const CFtype& type2 )
{
    return type1.id() < type2.id();
}

inline bool operator==( const CFtype& type1, const CFtype& type2 )
{
    return type1.id() == type2.id();
}

inline bool operator!=( const CFtype& type1, const CFtype& type2 )
{
    return ! ( type1 == type2 );
}

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CFdata;

class CFdataValue
{
    friend class CFdata;

public:
                                CFdataValue (                               )       : m_referenceCount( 1 )   {}
    virtual                    ~CFdataValue (                               )                                 {}

    virtual CFdataValue*        newCopy     ( void                          ) const { return new CFdataValue;  }

    virtual CFtype              type        ( void                          ) const { return CFtype::kInvalid; }

    virtual void                saveTo      ( std::ostream&     stream      ) const {                          }
    virtual void                loadFrom    ( std::istream&     stream      )       {                          }

private:
    unsigned int                m_referenceCount;
};

//  Warning: due to the template constructor
//  the compiler will accepted argument of any type
//  watch out of argument when you construct a CFdata !
//
class CFdata
{
public:
                                CFdata              (                                   )       : m_value( new CFdataValue ) {}
                                CFdata              ( const CFdata&     data            );
                                CFdata              ( const CFtype&     type            );
    template <typename Type>    CFdata              ( const Type&       value           );
                               ~CFdata              (                                   );

    static void                 registerType        ( const CFtype&     type,
                                                      CFdata            defaultValue    );

    static void                 registerBasicTypes  ( void                              );

    CFdata&                     operator=           ( const CFdata&     data            );

    CFdata                      copy                ( void                              ) const;

    template <typename Type>
    static CFtype               type                ( void                              );

    static CFtype               type                ( const QString&    typeName        );

    CFtype                      type                ( void                              ) const { return m_value->type();                     }
    bool                        isValid             ( void                              ) const { return m_value->type() != CFtype::kInvalid; }

    void                        saveTo              ( std::ostream&     stream          ) const { m_value->saveTo  ( stream ); }
    void                        loadFrom            ( std::istream&     stream          )       { m_value->loadFrom( stream ); }

    template <typename Type>
    const Type&                 value               ( void                              ) const;

    template <typename Type>
    Type&                       value               ( void                              );

    template <typename Type>
    void                        setValue            ( const Type&       value           );

private:
                                CFdata              ( CFdataValue*      valuePtr        )       : m_value( valuePtr ) {}

protected:

    //  Registered data types
    //
    static QMap<CFtype,CFdata>  m_defaultValues;
    static QMap<QString,int>    m_typeNameToTypeId;

    //  Data value
    //
    CFdataValue*                m_value;
};


//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
template <typename Type> class CFdataAnyValue : public CFdataValue
{
    friend class CFdata;

public:
                            CFdataAnyValue  ( const Type&   value           )       : m_value( value ) {}
    virtual                ~CFdataAnyValue  (                               )                          {}

    virtual CFdataValue*    newCopy         ( void                          ) const { return new CFdataAnyValue( m_value ); }

    virtual CFtype          type            ( void                          ) const { return m_type;                        }

    virtual void            saveTo          ( std::ostream&     stream      ) const { stream << m_value;                    }
    virtual void            loadFrom        ( std::istream&     stream      )       { stream >> m_value;                    }

    const Type&             value           ( void                          ) const { return m_value;                       }
    Type&                   value           ( void                          )       { return m_value;                       }
    void                    setValue        ( const Type&       value       )       { m_value = value;                      }

protected:
    static CFtype           m_type;
    Type                    m_value;
};


//////////////////////////////////////////////////////////////////////////
//
//  CFdata implementation
//
inline CFdata::CFdata( const CFdata& data ) : m_value( data.m_value )
{
    m_value->m_referenceCount++;
}

inline CFdata::CFdata( const CFtype& type )
{
    QMap<CFtype,CFdata>::iterator dataIt( m_defaultValues.find( type ) );

    //  If data of that type has not been registered
    //
    if( dataIt == m_defaultValues.end() )
    {
        //  Initialize with an invalid data
        //
        m_value = new CFdataValue;
    }
    else
    {
        //  Initialize with a copy of the default value
        //
        m_value = dataIt.value().m_value->newCopy();
    }
}

template <typename Type> inline CFdata::CFdata( const Type& value ) : m_value( new CFdataAnyValue<Type>( value ) )
{
}

inline CFdata::~CFdata()
{
    m_value->m_referenceCount--;

    if( ! m_value->m_referenceCount ) delete m_value;
}

inline CFdata& CFdata::operator=( const CFdata& data )
{
    m_value = data.m_value;

    m_value->m_referenceCount++;

    return *this;
}

inline CFdata CFdata::copy( void ) const
{
    return CFdata( m_value->newCopy() );
}

template <typename Type> inline CFtype CFdata::type( void )
{
    return CFdataAnyValue<Type>::m_type;
}

template <typename Type> inline const Type& CFdata::value( void ) const
{
    assert( type() == CFdata::type<Type>() );

    return static_cast<const CFdataAnyValue<Type>*>( m_value )->value();
}

template <typename Type> inline Type& CFdata::value( void )
{
    assert( type() == CFdata::type<Type>() );

    return static_cast<CFdataAnyValue<Type>*>( m_value )->value();
}

template <typename Type> inline void CFdata::setValue( const Type& value )
{
    assert( type() == CFdata::type<Type>() );

    static_cast<const CFdataAnyValue<Type>*>( m_value )->setValue( value );
}



#endif //__CFdata_h__
