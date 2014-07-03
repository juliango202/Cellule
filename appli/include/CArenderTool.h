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
#ifndef __CArenderTool_h__
#define __CArenderTool_h__

#include "CAtool.h"

// Tool for selecting a render window
class CArenderTool: public CAtool
{
public:
                            CArenderTool        () : CAtool( "Render" ),
                                                     m_visible(false)   {}
    virtual                 ~CArenderTool       ()                      {}

    virtual void            click               ( const CFpoint&  position );
    virtual void            drag                ( const CFpoint&  position );
    virtual void            unclick             ( const CFpoint&  position );
    virtual void            Draw                ( void                     ) const;

private:
    CFpoint                 m_corner1;      // Coordinates of the first click
    CFpoint                 m_corner2;      // Coordinates of the second click
    bool                    m_visible;
};

#endif //__CArenderTool_h__
