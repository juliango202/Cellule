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
#ifndef __CWcell_h__
#define __CWcell_h__

#include "CWsurfaceShader.h"

#include "CFtopoPoint.h"
#include "CFcircle.h"
#include "CFtypes.h"
#include "CFcolor.h"

#include <QtCore/QPointer>

class CWcell : public CFtopoPoint
{
public:

    class DrawStyle
    {
    public:
        
        DrawStyle( void ) : drawCenter( false ), drawSeed( false ), drawBSpline(false), drawContour( true ), drawShading( true ) {}
        
        bool drawCenter;
        bool drawSeed;
        bool drawBSpline;
        bool drawContour;
        bool drawShading;
    };

public:
                            CWcell      ( const CFcircle&           shape,
                                          int                       id ,
                                          const CFcolor&            color           ) : CFtopoPoint( shape ), m_id( id ), m_color( color ) {};

    int                     id          ( void                                      ) const { return m_id;       }
    const CFcolor&          color       ( void                                      ) const { return m_color;    }
    void                    setColor    ( const CFcolor&            color           )       { m_color = color;   }

    const CWsurfaceShader*  shader      ( void                                      ) const { return m_shader;   }
    void                    setShader   ( CWsurfaceShader*          shader          )       { m_shader = shader; }
    
    void                    Draw        ( const CWsurfaceShader*    defaultShader,
                                          const DrawStyle&          style           ) const;

private:
    
    int                         m_id;
    CFcolor                     m_color;
    QPointer<CWsurfaceShader>   m_shader;

    friend std::ostream& operator<<( std::ostream&, const CWcell& );
    friend std::istream& operator>>( std::istream&,       CWcell& );
};


//  Serializer
//
inline std::ostream& operator<<( std::ostream& os, const CWcell& cell )
{
    os << static_cast<const CFtopoPoint&>( cell ) << " ";
    os << cell.m_color;

    return os;
}

inline std::istream& operator>>( std::istream& is, CWcell& cell )
{
    is >> static_cast<CFtopoPoint&>( cell );
    is >> cell.m_color;

    return is;
}

#endif //__CWcell_h__
