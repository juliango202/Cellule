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
#ifndef __CWscene_h__
#define __CWscene_h__

#include "CWcell.h"
#include "CWgenerator.h"
#include "CWcamera2D.h"

#include "NWgraph.h"

#include "CFpoint.h"
#include "CFsurfaceIntegral.h"
#include "CFtopoMesh.h"

#include <vector>

class CWcellSampler;
class CWrenderSettings;

//! \brief  Contains all objects of a Cellule scene
//! \author bEngo
//! \date   2008 March 24
//!
class CWscene : public NWgraph
{
    Q_OBJECT
    Q_PROPERTY( QObject* defaultCamera2D READ defaultCamera2D )

public:

    typedef std::set<CWcell*>           Cells;

    enum DrawStyle
    {
        kPoint,
        kWireframe,
        kShaded
    };

public:

                            CWscene                 ( QObject*                    parent = 0            );
    virtual                ~CWscene                 ();

//  SCRIPTING INTERFACE
//  ( PROPERTIES )
//
public:

    QObject*                defaultCamera2D         ( void                                              )         { return m_defaultCamera2D; }

//  SCRIPTING INTERFACE
//  ( METHODS )
//
public slots:

    bool                    clear                   ( bool                       force = false          );

    QObject*                createNode              ( const QString*             type,
                                                      const QString*             name      = 0,
                                                      QObject*                   parent    = 0          );
    //  Generators
    //
    void                    fillRandomly            ( void                                              );

    //  Input / Output
    //
    void                    rename                  ( QString*                   fileName               );
    void                    load                    ( QString*                   fileName               );
    void                    save                    ( void                                              ) const;
    void                    saveAs                  ( QString*                   fileName               );

signals:

    void                    renamed                 ( QString*                   fileName               );

//  C++ INTERFACE
//
public:

    CWcamera2D*             getDefaultCamera2D      ( void                                              )         { return m_defaultCamera2D; }
    const CWcamera2D*       getDefaultCamera2D      ( void                                              ) const   { return m_defaultCamera2D; }

    CWsurfaceShader*        getDefaultSurfaceShader ( void                                              )         { return m_defaultSurfaceShader; }
    const CWsurfaceShader*  getDefaultSurfaceShader ( void                                              ) const   { return m_defaultSurfaceShader; }

    const CWrenderSettings* getRenderSettings       ( void                                              ) const   { return m_renderSettings; }

    const CFtopoMesh&       getCells                ( void                                              ) const   { return m_cells; }
    CFtopoMesh&             getCells                ( void                                              )         { return m_cells; }

    CWcell*                 createCell              ( const CFcircle&            shape,
                                                      const CFcolor&             color                  );
    
    void                    deleteContent           ( CWcell*                    cell                   );
    void                    deleteContent           ( CFtopoWeightedSet&         cells                  );

    //  Rendering
    //
    void                    sample                  ( const CFpoint&             point,
                                                      CFsample&                  result                 );

    void                    render                  ( const CFcircle&            point,
                                                      CFcolor&                   result                 );

    //  openGL display
    //
    void                    Draw                    ( const CWcell::DrawStyle&   cellDrawStyle,
                                                      bool                       drawDelauney           ) const;

protected:

    void                    registerNodes           ( void                                              );

    void                    createDefaultNodes      ( void                                              );

protected:

    QString                 m_fileName;
    
    CWcamera2D*             m_defaultCamera2D;
    CWcellSampler*          m_cellSampler;
    CWrenderSettings*       m_renderSettings;
    CWsurfaceShader*        m_defaultSurfaceShader;
    
    CFtopoMesh              m_cells;
    int                     m_newCellId;
    CWcell::DrawStyle       m_cellsDrawStyle;
    
    std::vector<void*>      m_generators;
};

#endif //__CWscene_h__
