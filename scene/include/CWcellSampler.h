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
#ifndef __CWcellSampler_h__
#define __CWcellSampler_h__

#include "CWnodeTypes.h"

#include "NWnode.h"
#include "NWoutputAttribute.h"

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CWcellSampler : public NWnode
{
    Q_OBJECT

public:

    enum Output
    {
        kId,
        kRadius,
        kCenterDistance,
        kContourDistance
    };

public:
                                CWcellSampler       ( void      );

    virtual CFtype              type                ( void      ) const { return CWcellSamplerType; }
    static NWnode*              creator             ( void      )       { return new CWcellSampler; }

protected:

    static NWoutputAttribute    m_idAttr;
    static NWoutputAttribute    m_radius;
    static NWoutputAttribute    m_centerDistanceAttr;
    static NWoutputAttribute    m_contourDistanceAttr;
};

#endif //__CWcellSampler_h__
