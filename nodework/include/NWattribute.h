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
#ifndef __NWattribute_h__
#define __NWattribute_h__

#include "CFdata.h"

#include <QtCore/QString>

class NWnode;

//! \brief  A named value belonging to a node
//! \author bEngo
//! \date   2008 March 24
//!
class NWattribute
{
public:
                        NWattribute       ( const QString&  name,
                                            CFtype          type    );

    const QString&      name              ( void                    ) const { return m_name; }
    CFtype              type              ( void                    ) const { return m_type; }

protected:

    QString             m_name;
    CFtype              m_type;
};

#endif //__NWattribute_h__
