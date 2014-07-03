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
#ifndef __CFvectorField_h__
#define __CFvectorField_h__

#include "CFpoint.h"

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CFvectorField
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

class CFtopoWeightedSet;

class CFvectorField
{
public:
    virtual                 ~CFvectorField  ()                                  {}

    virtual CFvector        Sample          ( const CFpoint&    position    )   const = 0;

    inline void             SetIntensity    ( CFfloat           intensity   )   { m_intensity = intensity;  }
    inline const CFfloat&   GetIntensity    ( void ) const                      { return m_intensity;       }

private:

    CFfloat                 m_intensity;
};


class CFradialField : public CFvectorField
{
public:
                            CFradialField   ( const CFpoint&    center      ) : m_center( center )  {}
    virtual                 ~CFradialField  ()                                                      {}

    virtual CFvector        Sample          ( const CFpoint&    position    ) const;

private:
    CFpoint                 m_center;
};

/*
class CFrelaxField : public CFvectorField
{
public:
                            CFrelaxField    ( const CFtopoWeightedSet& points );

    virtual CFvector        Sample          ( const CFpoint&    position    ) const;

private:
    CFfloat                 m_avgDistance;
};
*/

#endif //__CFvectorField_h__
