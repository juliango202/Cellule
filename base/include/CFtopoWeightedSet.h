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
#ifndef __CFtopoWeightedSet_h__
#define __CFtopoWeightedSet_h__

#include <set>
#include <vector>
#include <algorithm>

#include "CFtypes.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFtopoWeightedSet
//
//  Author: Bengo & JuL
//  Date:   29/05/2004
//
//  Desc:       
//
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CFtopoPoint;

class CFtopoWeightedElem
{
public:
                            CFtopoWeightedElem  ( CFtopoPoint*      point,
                                                  CFfloat           weight  )   : m_point( point ), m_weight( weight ) {}

    inline CFtopoPoint*     GetPoint            ( void )    const               { return m_point;   }
    inline CFfloat          GetWeight           ( void )    const               { return m_weight;  }

    inline void             SetPoint            ( CFtopoPoint*      point   )   { m_point   = point;    }
    inline void             SetWeight           ( CFfloat           weight  )   { m_weight  = weight;   }

private:
    CFtopoPoint*        m_point;
    CFfloat             m_weight;
};
 
struct CFtopoWeightedElemPointCompare : std::binary_function<CFtopoWeightedElem, CFtopoWeightedElem, bool>
{
    inline bool operator() ( const CFtopoWeightedElem& elem1, const CFtopoWeightedElem& elem2 ) const
    {
        return elem1.GetPoint() < elem2.GetPoint();
    }
};

struct CFtopoWeightedElemWeightCompare : std::binary_function<CFtopoWeightedElem, CFtopoWeightedElem, bool>
{
    inline bool operator() ( const CFtopoWeightedElem& elem1, const CFtopoWeightedElem& elem2 ) const
    {
        return elem1.GetWeight() < elem2.GetWeight();
    }
};

class CFtopoWeightedSet
{
public:
    typedef std::set<CFtopoWeightedElem, CFtopoWeightedElemPointCompare>    Elems;
    typedef Elems::iterator                                                 ElemsIt;
    typedef Elems::const_iterator                                           ConstElemsIt;

public:
    inline ElemsIt      GetElemsBegin       ( void                  )           { return m_elems.begin();   }
    inline ConstElemsIt GetElemsBegin       ( void                  )   const   { return m_elems.begin();   }
    inline ElemsIt      GetElemsEnd         ( void                  )           { return m_elems.end();     }
    inline ConstElemsIt GetElemsEnd         ( void                  )   const   { return m_elems.end();     }
    
    inline unsigned int GetSize             ( void                  )           { return m_elems.size();    }

    inline void         Clear               ( void                  )           { m_elems.clear(); }

    inline void         Add                 ( CFtopoPoint*  point,
                                              CFfloat       weight  )           { if( weight > 0.0 ) m_elems.insert( CFtopoWeightedElem( point, weight) ); }

    ElemsIt             Find                ( CFtopoPoint*  point   );
    inline void         Erase               ( CFtopoPoint*  point   )           { m_elems.erase( Find( point ) ); }

    bool                IsIn                ( CFtopoPoint*  point   );

private:
    Elems               m_elems;

};

class CFtopoWeightedVect
{
public:
    typedef std::vector<CFtopoWeightedElem >        Elems;
    typedef Elems::iterator                         ElemsIt;
    typedef Elems::const_iterator                   ConstElemsIt;

public:
    inline ElemsIt      GetElemsBegin       ( void                  )           { return m_elems.begin();   }
    inline ConstElemsIt GetElemsBegin       ( void                  )   const   { return m_elems.begin();   }
    inline ElemsIt      GetElemsEnd         ( void                  )           { return m_elems.end();     }
    inline ConstElemsIt GetElemsEnd         ( void                  )   const   { return m_elems.end();     }
    
    inline unsigned int GetSize             ( void                  )           { return m_elems.size();    }

    inline void         Clear               ( void                  )           { m_elems.clear(); }

    inline void         Add                 ( CFtopoPoint*  point,
                                              CFfloat           weight  )       { m_elems.push_back( CFtopoWeightedElem( point, weight) ); }

    inline void         SortByWeight        ( void                      )       { std::sort( m_elems.begin(), m_elems.end(), CFtopoWeightedElemWeightCompare() ); }

    
    //  Operators
    inline CFtopoWeightedElem& operator[]( unsigned int index   )           { return m_elems[index]; }

private:
    Elems               m_elems;
};

#endif //__CFtopoWeightedSet_h__
