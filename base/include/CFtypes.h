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
#ifndef __CFtypes_h__
#define __CFtypes_h__

#include <assert.h>

#ifdef _WIN32
#include "windows.h"
#endif //_WIN32

#include "GL/gl.h"
#include <cmath>

#include <iostream>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

#define CFassert( condition, message )      if( ! ( condition ) ) { _asm int 3 }

#else

#define CFassert( condition, message )

#endif
/////////////////////////////////////////////////////////////////////////////////////////

typedef float           CFfloat;
typedef unsigned int    uint;

const CFfloat PI        = 3.14159265358979323846264338328;
const CFfloat PIx2      = 6.28318530717958647692528676656;
const CFfloat RAD2DEG   = 57.2957795130823208767981548141;

const CFfloat SQRT2     = 1.4142135623730950488016887242097;

class CFfloat3
{
public:

    inline CFfloat3     ( CFfloat   v1 = 0.0,
                          CFfloat   v2 = 0.0,
                          CFfloat   v3 = 0.0    )   { values[0] = v1; values[1] = v2; values[2] = v3; }

    inline CFfloat norm( void )
    {
        return sqrt( norm2() );
    }

    inline CFfloat norm2( void )
    {
        return values[0] * values[0] + values[1] * values[1] + values[2] * values[2];
    }

    inline CFfloat &operator[]  ( const unsigned int &index )           { return values[index]; }
    inline const CFfloat &operator[]( const unsigned int &index ) const { return values[index]; }

    inline CFfloat3 operator*   ( const CFfloat &factor ) const
    { return CFfloat3( values[0]*factor, values[1]*factor, values[2]*factor ); }

    inline CFfloat3 operator/   ( const CFfloat &factor ) const
    {
        assert( factor != 0.0 );
        return CFfloat3( values[0]/factor, values[1]/factor, values[2]/factor );
    }

    inline CFfloat3 operator+   ( const CFfloat3 &other ) const
    { return CFfloat3( values[0] + other[0], values[1] + other[1], values[2] + other[2] ); }

    inline CFfloat3 operator-   ( const CFfloat3 &other ) const
    { return CFfloat3( values[0] - other[0], values[1] - other[1], values[2] - other[2] ); }

    inline void operator+=      ( const CFfloat3 &other )
    { values[0]+=other[0]; values[1]+=other[1]; values[2]+=other[2]; }

    inline void operator=       ( const CFfloat3 &other )
    { values[0]=other[0]; values[1]=other[1]; values[2]=other[2]; }

    inline void operator*=      ( const CFfloat &factor )
    { values[0]*=factor; values[1]*=factor; values[2]*=factor; }

    inline void operator/=      ( const CFfloat &factor )
    { values[0]/=factor; values[1]/=factor; values[2]/=factor; }

protected:

    CFfloat values[3];
};

inline std::ostream& operator<<( std::ostream& os, const CFfloat3& point )
{
    os << point[0] << " " << point[1] << " " << point[2];

    return os;
}

inline std::istream& operator>>( std::istream& is, CFfloat3& point )
{
    is >> point[0] >> point[1] >> point[2];

    return is;
}

typedef std::string CFstring;

#endif //__CFtypes_h__
