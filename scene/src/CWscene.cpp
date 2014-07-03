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
#include "CWscene.h"

#include "CWcellSampler.h"
#include "CWsurfaceShader.h"
#include "CWcolorGradient.h"
#include "CWdecomposeColor.h"
#include "CWcomposeReals.h"
#include "CWcomposeColors.h"
#include "CWrandomizer.h"
#include "CWrenderSettings.h"

#include "CFcircle.h"
#include "CFsegment.h"
#include "CFlog.h"

#include <QtCore/QFile>
#include <QtGui/QMessageBox>

#include <gsl/gsl_rng.h>

#include "GL/gl.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <fstream>
#include <sstream>

CWscene::CWscene( QObject* parent )
:
    NWgraph               ( parent ),
    m_cellSampler         ( 0      ),
    m_renderSettings      ( 0      ),
    m_defaultSurfaceShader( 0      ),
    m_newCellId           ( 1      )
{
    registerNodes();

    m_defaultCamera2D = new CWcamera2D( this );
}

CWscene::~CWscene()
{
    //  Delete all cells
    //
    CFtopoMesh::ConstPointsIt pointIt = m_cells.GetPointsBegin();
    
    while( pointIt != m_cells.GetPointsEnd() ) delete static_cast<CWcell*>( *pointIt++ );
}

bool CWscene::clear( bool force )
{
    //  Confirmation dialog
    //
    if( ! force )
    {
        QMessageBox::StandardButton result( QMessageBox::question
        (
            0, "Save current",
            "Do you want to save the current scene before ?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Cancel )
        );
             if( result == QMessageBox::Save   ) save();
        else if( result == QMessageBox::Cancel ) return false;
    }

    //  Delete all cells
    //
    CFtopoMesh::ConstPointsIt pointIt = m_cells.GetPointsBegin();
    
    while( pointIt != m_cells.GetPointsEnd() ) delete static_cast<CWcell*>( *pointIt++ );

    m_cells.Clear(); // empty cell pointer container

    m_newCellId = 0;

    //  Delete nodes
    //
    NWgraph::clear();

    //  Recreate default nodes
    //
    m_cellSampler          = 0;
    m_renderSettings       = 0;
    m_defaultSurfaceShader = 0;

    createDefaultNodes();
    m_selection.clear();    

    //  Rename scene
    //
    QString untitled( "scenes/untitled.cel" );

    rename( &untitled );

    CFlog::instance()->addMessage( QString( "Scene cleared." ), CFlog::kInfo );

    return true;
}

void CWscene::registerNodes( void )
{
    m_nodeLibrary.registerNode( CWcellSamplerType,    CWcellSampler::creator    );
    m_nodeLibrary.registerNode( CWsurfaceShaderType,  CWsurfaceShader::creator  );
    m_nodeLibrary.registerNode( CWcolorGradientType,  CWcolorGradient::creator  );
    m_nodeLibrary.registerNode( CWdecomposeColorType, CWdecomposeColor::creator );
    m_nodeLibrary.registerNode( CWcomposeRealsType,   CWcomposeReals::creator   );
    m_nodeLibrary.registerNode( CWcomposeColorsType,  CWcomposeColors::creator  );
    m_nodeLibrary.registerNode( CWrandomizerType,     CWrandomizer::creator     );
    m_nodeLibrary.registerNode( CWrenderSettingsType, CWrenderSettings::creator );
}

QObject* CWscene::createNode( const QString* typeName, const QString* name, QObject* parent )
{
    assert( parent == 0 || qobject_cast<NWnode*>( parent ) );

    static QString defaultName( "" );

    return NWgraph::createNode
    (
        m_nodeLibrary.findNodeType( *typeName ),
        name   ? *name                           : defaultName,
        parent ? qobject_cast<NWnode*>( parent ) : 0
    );
}

void CWscene::createDefaultNodes( void )
{
    assert( m_cellSampler          == 0 );
    assert( m_renderSettings       == 0 );
    assert( m_defaultSurfaceShader == 0 );

    m_cellSampler = qobject_cast<CWcellSampler*>
    (
        NWgraph::createNode( CWcellSamplerType, "cellSampler", 0, true, true )
    );

    m_defaultSurfaceShader = qobject_cast<CWsurfaceShader*>
    (
        NWgraph::createNode( CWsurfaceShaderType, "defaultSurfaceShader", 0, true, false )
    );

    m_renderSettings = qobject_cast<CWrenderSettings*>
    (
        NWgraph::createNode( CWrenderSettingsType, "renderSettings", 0, true, true )
    );
}

CWcell* CWscene::createCell( const CFcircle& shape, const CFcolor& color )
{
    CWcell* newCell = new CWcell( shape, m_newCellId++, color );
    
    CFtopoPoint* newPoint = static_cast<CFtopoPoint*>( newCell );
    
    if( ! m_cells.Insert( *newPoint ) )
    {
        delete newCell;
        return 0;
    }
    
    return newCell;
}

void CWscene::deleteContent( CWcell* cell )
{
    CFtopoPoint* point = static_cast<CFtopoPoint*>( cell );
    
    m_cells.Remove( *point );
    
    delete cell;
}

void CWscene::deleteContent( CFtopoWeightedSet& cells )
{
    CFtopoWeightedSet::ElemsIt cellIt = cells.GetElemsBegin();

    while( cellIt != cells.GetElemsEnd() )
    {
        deleteContent( static_cast<CWcell*>( cellIt->GetPoint() ) );
        cellIt++;
    }

    cells.Clear();
}

void CWscene::fillRandomly( void )
{
    int i = 60000;

    while( i-- )
    {
        CFcircle circle
        (
            4.0 * ( float( rand() ) / RAND_MAX ) - 2.0,
            4.0 * ( float( rand() ) / RAND_MAX ) - 2.0,
            0.005 + 0.08 * float( rand() ) / RAND_MAX
        );

        createCell( circle, CFcolor::kWhite );
    }
}


//////////////////////////////////////////////////////
//
//      compute all values for the current position
//
void CWscene::sample( const CFpoint& point, CFsample& result )
{
    //  Find the cell enclosing the current point
    //
    if( m_cells.IsEmpty() ) return;

    CFfloat d1;
    CWcell* cell = static_cast<CWcell*>( m_cells.FindCell( point, &d1 ) );


    //  Get shading informations from the graph
    //
    const CWsurfaceShader* shader( cell->shader() );
    if( ! shader ) shader = getDefaultSurfaceShader();

    const CFfloat smoothness = shader->input( CWsurfaceShader::kSmoothness ).value<CFfloat>();

    CFfloat  centerDistance = cell->DistanceToCenter( point, smoothness );
    CFfloat  contourDistance = 1.0 - centerDistance;


    //  Pass sampling informations to the graph
    //  through the cellSampler node
    //
    m_cellSampler->output( CWcellSampler::kId              ).setValue<int    >( cell->id()           );
    m_cellSampler->output( CWcellSampler::kRadius          ).setValue<CFfloat>( cell->GetRadius()    );
    m_cellSampler->output( CWcellSampler::kCenterDistance  ).setValue<CFfloat>( centerDistance       );
    m_cellSampler->output( CWcellSampler::kContourDistance ).setValue<CFfloat>( contourDistance      );

    result.color  = shader->input( CWsurfaceShader::kColor  ).value<CFcolor>();
    result.height = shader->input( CWsurfaceShader::kHeight ).value<CFfloat>();
}

void CWscene::render( const CFcircle& point, CFcolor& result )
{
     //  Get surface informations
    //  ( averaged over given area )
    //
    const CFenum&       antialiasingType = m_renderSettings->input( CWrenderSettings::kAntialiasing   ).value<CFenum>();
    int                 maxSamples       = m_renderSettings->input( CWrenderSettings::kNbSamples      ).value<int>();

    CFsurfaceIntegral   integral( point, antialiasingType, maxSamples );

    do { CWscene::sample( integral.NextPoint(), integral.ShadingResult() ); } while( ! integral.End() );

    const CFsample& area = integral.Result();

    //  Get light information
    //
    CFfloat3    lightPos        ( 0.0, 0.0, 1.0 );
    CFfloat     lightIntensity  ( 1.0 );

    CFfloat3    lightDir
    (
        lightPos[0] - point[0],
        lightPos[1] - point[1],
        lightPos[2] - area.height
    );

    CFfloat norm = lightDir.norm();
    if( norm != 0.0 ) lightDir /= norm;

    //  Compute resulting color
    //
    CFfloat diffuse = lightIntensity *
    (
        1.0 - acos( lightDir[0] * area.normal[0] +
                    lightDir[1] * area.normal[1] +
                    lightDir[2] * area.normal[2] ) / 3.14159
    );

    result = area.color * diffuse;

    result.ClampRGB();
}

void CWscene::Draw( const CWcell::DrawStyle& cellDrawStyle, bool drawDelauney ) const
{
    //  Draw the uv layer
    //
    //if( CAcontext::GetDrawUvs() ) m_uvMesh.Draw();

    //  Draw cells
    //
    CFtopoMesh::ConstPointsIt cellIt( m_cells.GetPointsBegin() );

    while( cellIt != m_cells.GetPointsEnd() )
    {
        CWcell* cell( static_cast<CWcell*>( *cellIt++ ) );
        cell->Draw( getDefaultSurfaceShader(), cellDrawStyle );
    }

    //  Draw Delauney mesh
    //
    if( drawDelauney )
    {
        glColor3f( 0.1f, 0.3f, 0.2f );

        m_cells.DrawDelauney();
    }

    //  Draw origin
    //
    glColor3f( 0.6f, 0.9f, 0.6f );

    glBegin( GL_LINE_STRIP );

    glVertex2f(  0.00f, 0.0f );
    glVertex2f(  0.00f, 0.8f );
    glVertex2f( -0.07f, 0.8f );
    glVertex2f(  0.00f, 1.0f );
    glVertex2f(  0.07f, 0.8f );
    glVertex2f(  0.00f, 0.8f );

    glEnd();
}

///////////////////////////////////////
//
//  Input / Output
//
void CWscene::rename( QString* fileName )
{
    m_fileName = *fileName;

    emit renamed( fileName );
}

void CWscene::load( QString* fileName )
{
    //  Open the file
    //
    QFile file( *fileName );

    if( ! file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        CFlog::instance()->addMessage
        (
            QString( "Could not open %1 for reading." ).arg( *fileName ),
            CFlog::kError
        );
        return;
    }

    CFxmlIn xml( &file );

    //  Clear the scene
    //
    if( ! clear() ) return;

    //  Load the graph
    //
    NWgraph::loadFrom( xml );

    //  Load the cells
    //
    assert( xml.tokenType() == CFxmlIn::StartElement && xml.name() == "cells" );

    CFxmlIn::TokenType tokenType = xml.readAfterWhitespaces();

    assert( tokenType == CFxmlIn::Characters );

    std::string        cellsData      ( xml.text().toString().toStdString() );
    std::istringstream cellsDataStream( cellsData );

    CFcircle circle;
    CFcolor  color;

    m_cells.SetCheck( false );

    while( true )
    {
        cellsDataStream >> circle;
        cellsDataStream >> color;

        if( cellsDataStream.eof() ) break;

        createCell( circle, color );
    }

    m_cells.SetCheck( true );

    //  Close the scene file
    //
    file.close();

    rename( fileName );

    CFlog::instance()->addMessage
    (
        QString( "Scene %1 loaded." ).arg( *fileName ),
        CFlog::kInfo
    );
}

void CWscene::save( void ) const
{
    //  Check scene content
    //
    CFtopoMesh::ConstPointsIt pointIt = m_cells.GetPointsBegin();

    if( pointIt == m_cells.GetPointsEnd() )
    {
        CFlog::instance()->addMessage
        (
            QString( "Cannot save an empty scene." ),
            CFlog::kError
        );
        return;
    }

    //  Open the file
    //
    std::ofstream file;

    file.open( m_fileName.toUtf8() );

    if( ! file )
    {
        CFlog::instance()->addMessage
        (
            QString( "Could not open %1 for writing." ).arg( m_fileName ),
            CFlog::kError
        );
        return;
    }

    CFxmlOut xml( file );

    xml.stream() << "<scene type=\"Cellule\" version=\"0.1 alpha\">"; xml.indent();

    //  Save the graph
    //
    NWgraph::saveTo( xml );

    //  Save cells
    //
    file << std::endl << "  <cells>";

    for( ; pointIt != m_cells.GetPointsEnd(); pointIt++ )
    {
        CFtopoPoint*    point   = *pointIt;
        const CWcell&   cell    = static_cast<const CWcell&>( *point );

        file << std::endl << "    " << cell;
    }

    file << std::endl << "  </cells>";

    //  Close scene file
    //
    xml.unIndent(); xml.newLine(); xml.stream() << "</scene>";

    file.close();

    CFlog::instance()->addMessage
    (
        QString( "Scene saved as %1" ).arg( m_fileName ),
        CFlog::kInfo
    );
}

void CWscene::saveAs( QString* fileName )
{
    rename( fileName );

    save();
}

