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
#ifndef __CFkey1DList_h__
#define __CFkey1DList_h__

#include "CFkey1D.h"

#include <vector>
#include <limits>

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
template <typename ValueType>

class CFkey1DList
{
public:
    
    typedef CFkey1D<ValueType>  Key;
    typedef std::vector<Key>    KeyContainer;

public:
                                CFkey1DList (                               )       {}

    //  If another key is already at the given position
    //  The new key will be inserted before that key.
    //
    unsigned int                insertKey   ( const Key&        key         );
    void                        deleteKey   ( unsigned int      keyIndex    )       { m_keys.erase( m_keys.begin() + keyIndex ); }
    unsigned int                keyCount    ( void                          ) const { return m_keys.size();                      }

    //  Change key position and return the new index.
    //  Keys are kept sorted by position
    //  that's why the key index may change.
    //  If another key is already at the given position
    //  it is undefined whether the moving key
    //  will be inserted before or after that key.
    //
    unsigned int                moveKey     ( unsigned int      keyIndex,
                                              CFfloat           position    );

    void                        setKeyValue ( unsigned int      keyIndex,
                                              const ValueType&  value       )       { m_keys[ keyIndex ].setValue( value ); }

    //  Return the index of the closest key sitting next to the given position.
    //  Returns an invalid index ( equal to keyCount ) when no key match.
    //
    unsigned int                findNextKey ( CFfloat           position    ) const;
 
    const Key&                  operator[]  ( unsigned int      keyIndex    ) const { return m_keys[ keyIndex ];  }

    void                        saveTo      ( std::ostream&     stream      ) const;
    void                        loadFrom    ( std::istream&     stream      );

protected:

    KeyContainer                m_keys;
};

////////////////////////////////////////////////////////////////////

template <typename ValueType>
unsigned int CFkey1DList<ValueType>::insertKey( const Key& key )
{
    //  Simple and fast case: append at the end
    //
    if( ( keyCount() == 0 ) || ( m_keys[ keyCount() - 1 ].position() <= key.position() ) )
    {
        m_keys.push_back( key );

        return keyCount() - 1;
    }

    //  General case
    //
    typename KeyContainer::iterator newIndex( std::lower_bound( m_keys.begin(), m_keys.end(), key ) );

    return std::distance( m_keys.begin(), m_keys.insert( newIndex, key ) );
}

template <typename ValueType>
unsigned int CFkey1DList<ValueType>::moveKey( unsigned int keyIndex, CFfloat position )
{
    m_keys[ keyIndex ].setPosition( position );

    //  Maintain container order
    //  ( key position in the vector )

    //  Fast method for usual cases
    //
    CFfloat prevPrev = keyIndex     > 1          ? m_keys[ keyIndex - 2 ].position() : -std::numeric_limits<CFfloat>::max();
    CFfloat prev     = keyIndex     > 0          ? m_keys[ keyIndex - 1 ].position() : -std::numeric_limits<CFfloat>::max();
    CFfloat next     = keyIndex + 1 < keyCount() ? m_keys[ keyIndex + 1 ].position() :  std::numeric_limits<CFfloat>::max();
    CFfloat nextNext = keyIndex + 2 < keyCount() ? m_keys[ keyIndex + 2 ].position() :  std::numeric_limits<CFfloat>::max();

    //  Same index than before
    //
    if( position >= prev && position <= next )
    {
        return keyIndex;
    }

    //  Move one key backward
    //
    if( position < prev && position >= prevPrev )
    {
        std::swap( m_keys[ keyIndex - 1 ], m_keys[ keyIndex ] );
        return keyIndex - 1;
    }
    
    //  Move one key forward
    //
    if( position > next && position <= nextNext )
    {
        std::swap( m_keys[ keyIndex + 1 ], m_keys[ keyIndex ] );
        return keyIndex + 1;
    }

    //  General case
    //
    Key key( m_keys[ keyIndex ] );

    deleteKey( keyIndex );

    return insertKey( key );
}

template <typename ValueType>
unsigned int CFkey1DList<ValueType>::findNextKey( CFfloat position ) const
{
    return std::distance
    (
        m_keys.begin(),

        std::lower_bound
        (
            m_keys.begin(), m_keys.end(), position, CFkey1DorderByPosition<ValueType>()
        )
    );
}

template <typename ValueType>
void CFkey1DList<ValueType>::saveTo( std::ostream& stream ) const
{
    unsigned int i;

    stream << keyCount();

    for( i = 0; i < keyCount(); i++ )
    {
        stream << " ";
        m_keys[i].saveTo( stream );
    }
}

template <typename ValueType>
void CFkey1DList<ValueType>::loadFrom( std::istream& stream )
{
    unsigned int i;

    stream >> i;

    m_keys.resize( i );

    for( i = 0; i < keyCount(); i++ )
    {
        m_keys[i].loadFrom( stream );
    }
}

#endif //__CFkey1DList_h__
