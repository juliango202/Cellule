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
#ifndef __CFkey1D_h__
#define __CFkey1D_h__

#include "CFtypes.h"

//! \brief  Value associated to a one-dimensional parameter
//! \author bEngo
//! \date   2008 March 26
//!
template< typename ValueType >
class CFkey1D
{
public:
                        CFkey1D     (                               ) : m_position( 0.0 ) {}

                        CFkey1D     ( CFfloat           position,
                                      const ValueType&  value       ) : m_position( position ), m_value( value ) {}

    CFfloat             position    ( void                          ) const { return m_position;     }
    void                setPosition ( CFfloat           position    )       { m_position = position; }

    const ValueType&    value       ( void                          ) const { return m_value;  }
    void                setValue    ( const ValueType&  value       )       { m_value = value; }

    void                saveTo      ( std::ostream&     stream      ) const { stream << m_position << " " << m_value; }
    void                loadFrom    ( std::istream&     stream      )       { stream >> m_position        >> m_value; }

protected:

    CFfloat             m_position;
    ValueType           m_value;
};


//  Define a strict weak ordering for keys ( by position )
//
template< typename ValueType >
inline bool operator<( const CFkey1D< ValueType >& key1, const CFkey1D< ValueType >& key2 )
{
    return key1.position() < key2.position();
}

//  Strict weak ordering functor
//
template< typename ValueType >
struct CFkey1DorderByPosition : public std::binary_function<const CFkey1D<ValueType>&, CFfloat, bool>
{
    bool operator() ( const CFkey1D<ValueType>& key, CFfloat position )
    {
        return key.position() < position;
    }
};


#endif //__CFkey1D_h__
