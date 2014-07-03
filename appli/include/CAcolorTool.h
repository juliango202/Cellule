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
#ifndef __CAcolorTool_h__
#define __CAcolorTool_h__

#include "CAtool.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CAcolorTool
//
//  Author: bEnGo
//  Date:   17 Fevrier 2008
//
//  Desc:
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CAcolorTool: public CAtool
{
public:
                            CAcolorTool         () : CAtool( "Color" ) {}
    virtual                ~CAcolorTool         ()                     {}

    virtual void            click               ( const CFpoint&        position );
    void                    drag                ( const CFpoint&        position );
};


#endif //__CAcolorTool_h__
