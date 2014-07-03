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
#ifndef __CFenum_h__
#define __CFenum_h__

#include "assert.h"

#include <iostream>

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CFenum
{
public:
                            CFenum          ();

                            CFenum          ( unsigned int      valueIndex,
                                              unsigned int      valuesCount,
                                              const char*       valuesNames[]   );

    void                    set             ( unsigned int      valueIndex      )       { m_valueIndex = valueIndex; assert( m_valueIndex < m_valuesCount ); }

    unsigned int            asInt           ( void                              ) const { return m_valueIndex;                  }
    unsigned int            valuesCount     ( void                              ) const { return m_valuesCount;                 }
    const char*             valueName       ( void                              ) const { return m_valuesNames[ m_valueIndex ]; }
    const char*             valueName       ( unsigned int      valueIndex      ) const { return m_valuesNames[ valueIndex   ]; }

    void                    saveTo          ( std::ostream&     stream          ) const { stream << m_valueIndex; }
    void                    loadFrom        ( std::istream&     stream          )       { stream >> m_valueIndex; }

protected:

    unsigned int            m_valueIndex;
    unsigned int            m_valuesCount;
    const char**            m_valuesNames;
};

inline std::ostream& operator<<( std::ostream& stream, const CFenum& enumVal ) { enumVal.saveTo  ( stream ); return stream; }
inline std::istream& operator>>( std::istream& stream,       CFenum& enumVal ) { enumVal.loadFrom( stream ); return stream; }

#endif //__CFenum_h__
