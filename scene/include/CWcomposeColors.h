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
#ifndef __CWcomposeColors_h__
#define __CWcomposeColors_h__

#include "CWnodeTypes.h"

#include "NWnode.h"
#include "NWinputAttribute.h"
#include "NWoutputAttribute.h"

//! \brief  Node mixing two colors ( different kind of mixing available )
//! \author bEngo
//! \date   2008 April 18
//!
class CWcomposeColors : public NWnode
{
public:
                                CWcomposeColors ( void      );

    virtual CFtype              type            ( void      ) const { return CWcomposeColorsType; }
    static NWnode*              creator         ( void      )       { return new CWcomposeColors; }

    virtual void                evaluate        ( void      );

protected:

    static NWinputAttribute     m_operationAttr;
    static NWinputAttribute     m_input1Attr;
    static NWinputAttribute     m_input2Attr;

    static NWoutputAttribute    m_outputAttr;
};

#endif //__CWcomposeColors_h__
