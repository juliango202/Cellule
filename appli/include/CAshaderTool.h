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
#ifndef __CAshaderTool_h__
#define __CAshaderTool_h__

#include "CAtool.h"
#include "CWsurfaceShader.h"

#include <QtCore/QPointer>

class NWselection;

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class CAshaderTool: public CAtool
{
    Q_OBJECT

public:
                                CAshaderTool        () : CAtool( "Shader" ) {}
    virtual                    ~CAshaderTool        ()                      {}

    virtual void                activate            ( void                              );
    virtual void                deactivate          ( void                              );

    virtual void                click               ( const CFpoint&        position    );
    void                        drag                ( const CFpoint&        position    );

    virtual void                Draw                ( void                              ) const;

public slots:

    void                        setShader           ( const NWselection&    nodes       );

protected:

    QPointer<CWsurfaceShader>   m_shader;
};

#endif //__CAshaderTool_h__
