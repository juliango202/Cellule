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
#ifndef __CWcolorGradient_h__
#define __CWcolorGradient_h__

#include "CWnodeTypes.h"

#include "NWnode.h"
#include "NWinputAttribute.h"
#include "NWoutputAttribute.h"

//! \brief  Output a color from a color gradient
//! \author bEngo
//! \date   2008 March 24
//!
class CWcolorGradient : public NWnode
{
public:
                                CWcolorGradient ( void      );

    virtual CFtype              type            ( void      ) const { return CWcolorGradientType; }
    static NWnode*              creator         ( void      )       { return new CWcolorGradient; }

    virtual void                evaluate        ( void      );

protected:

    static NWinputAttribute     m_colorsAttr;
    static NWinputAttribute     m_positionAttr;
    static NWoutputAttribute    m_colorAttr;
};

#endif //__CWcolorGradient_h__
