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
#ifndef __NWinputAttribute_h__
#define __NWinputAttribute_h__

#include "NWattribute.h"

#include "CFdata.h"

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class NWinputAttribute : public NWattribute
{
public:

    enum Level
    {
        kConfiguration,
        kEvaluation
    };

public:
                    NWinputAttribute  ( const QString&  name,
                                        const CFdata&   initialValue,
                                        Level           level         = kEvaluation,
                                        const CFdata&   minimum       = CFdata(),
                                        const CFdata&   maximum       = CFdata()        );

    const CFdata&   initialValue      ( void                                            ) const { return m_initialValue; }
    const CFdata&   minimum           ( void                                            ) const { return m_minimum;      }
    const CFdata&   maximum           ( void                                            ) const { return m_maximum;      }

    Level           level             ( void                                            ) const { return m_level;        }

protected:
    
    CFdata          m_initialValue;
    CFdata          m_minimum;
    CFdata          m_maximum;

    Level           m_level;
};

#endif //__NWinputAttribute_h__
