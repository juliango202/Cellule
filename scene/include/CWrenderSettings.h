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
#ifndef __CWrenderSettings_h__
#define __CWrenderSettings_h__

#include "NWnode.h"
#include "NWinputAttribute.h"

#include "CWnodeTypes.h"

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CWrenderSettings : public NWnode
{
    Q_OBJECT

public:

    enum Input
    {
        kAntialiasing,
        kNbSamples,
        kUseCustomResolution,
        kWidth,
        kHeight
    };

public:
                                CWrenderSettings ( void      );

    virtual CFtype              type             ( void      ) const { return CWrenderSettingsType; }
    static NWnode*              creator          ( void      )       { return new CWrenderSettings; }

protected:

    static NWinputAttribute     m_antialiasingAttr;
    static NWinputAttribute     m_nbSamplesAttr;
    static NWinputAttribute     m_useCustomResolutionAttr;
    static NWinputAttribute     m_widthAttr;
    static NWinputAttribute     m_heightAttr;
};

#endif //__CWrenderSettings_h__
