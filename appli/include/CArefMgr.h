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
#ifndef __CArefMgr_h__
#define __CArefMgr_h__

#include <assert.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CArefMgr
//
//  Author: bEngo
//  Date:   21/04/2006
//
//  Desc:       
//
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

template<typename T> class CArefMgr;

template<typename T> class CAref
{
    friend class CArefMgr<T>;
public:

                        CAref       ()                      : m_refId( 0 )      {}

    bool                IsNull      ( void              ) const                 { return m_refId == 0;              }
    bool                operator==  ( CAref<T> other    ) const                 { return m_refId == other.m_refId;  }
    bool                operator!=  ( CAref<T> other    ) const                 { return m_refId != other.m_refId;  }
private:
                        CAref       ( unsigned refId )  : m_refId( refId )  {}
private:
    unsigned            m_refId;
};

template<typename T> class CArefMgr
{
private:
    typedef typename std::vector<T*>    PtrVect;
    typedef typename PtrVect::iterator  PtrIt;

public:
                        CArefMgr    ()
    {
        m_refs.push_back( 0 );
    }
                        ~CArefMgr   ()
    {
        PtrIt it = m_refs.begin();
        while( it != m_refs.end() )
        {
            delete *it;
            it++;
        }
    }

    inline CAref<T>     New         ( void )
    {
        T* newRef = new T; assert( newRef > 0 );

        m_refs.push_back( newRef );

        return CAref<T>( m_refs.size() - 1 );
    }
    
    inline CAref<T>     New         ( T* newedObj )
    {
        assert( newedObj > 0 );
        
        m_refs.push_back( newedObj );

        return CAref<T>( m_refs.size() - 1 );
    }
    
    inline void         CheckParam  ( unsigned refId )
    {
        assert( refId           > 0             );
        assert( refId           < m_refs.size() );
        assert( m_refs[ refId ] > 0             );
    }

    inline void         Delete      ( CAref<T> ref )
    {
        CheckParam( ref.m_refId );

        delete m_refs[ ref.m_refId ];
        m_refs[ ref.m_refId ] = 0;
    }
/*
    inline const T&     operator[]  ( CAref<T> ref ) const
    {
        CheckParam( ref.m_refId );

        return *m_refs[ ref.m_refId ];
    }
*/
    inline T&           operator[]  ( CAref<T> ref )
    {
        CheckParam( ref.m_refId );

        return *m_refs[ ref.m_refId ];
    }

private:

    PtrVect m_refs;
};


#endif //__CArefMgr_h__
