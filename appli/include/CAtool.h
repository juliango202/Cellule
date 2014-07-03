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
#ifndef __CAtool_h__
#define __CAtool_h__

#include "CFcircle.h"

#include <QtCore/QObject>
#include <QtCore/QString>

////////////////////////////////////////////////////////////////////////////
//
//  Class:  CAtool
//
//  Author: bEnGo
//  Date:   2003
//
//  Desc:   Base class of user interactive tools
//
//  To do:
//
////////////////////////////////////////////////////////////////////////////

class CAtool : public QObject
{
    Q_OBJECT

public:
                            CAtool              ( const QString&        name            );

    virtual                 ~CAtool             () {}

    const QString&          getName             ( void                                  ) const { return m_name;        }

    virtual void            activate            ( void                                  )       {}
    virtual void            deactivate          ( void                                  )       {}

    virtual void            click               ( const CFpoint&        position        )       { move( position ); }
    virtual void            drag                ( const CFpoint&        position        )       { move( position ); }
    virtual void            unclick             ( const CFpoint&        position        )       { move( position ); }

    virtual void            Draw                ( void                                  ) const;

    CFcircle&               shape               ( void                                  )       { return m_shape;       }
    const CFcircle&         shape               ( void                                  ) const { return m_shape;       }

    CFfloat&                intensity           ( void                                  )       { return m_intensity;   }
    const CFfloat&          intensity           ( void                                  ) const { return m_intensity;   }

    void                    move                ( const CFpoint&        position        )       { m_shape = position; }

    void                    Scale               ( float                 factor          );

protected:
    
    const CFpoint&          GetPosition         ( void                                  ) const { return m_shape; }

protected:

    QString                 m_name;

    CFcircle                m_shape;
    CFfloat                 m_intensity;

private:

    CFpoint                 m_screenPos;
};

#endif //__CAtool_h__
