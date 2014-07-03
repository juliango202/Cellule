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
#include "CAtoolManager.h"

#include "CAemitterTool.h"
#include "CAeraserTool.h"
#include "CAsizeTool.h"
#include "CAmagnetoTool.h"
#include "CArelaxTool.h"
#include "CArenderTool.h"
#include "CAcolorTool.h"
#include "CAshaderTool.h"

#include "CFpoint.h"
#include "CFlog.h"

CAtoolManager::CAtoolManager( QObject* parent ) : QObject( parent )
{
    QList<CAtool*> tools;

    //  Instanciate every tool
    //  Add your tools here
    //
    tools << new CAemitterTool;
    tools << new CAeraserTool;
    tools << new CAsizeTool;
    tools << new CAmagnetoTool;
    tools << new CArelaxTool;
    tools << new CArenderTool;
    tools << new CAcolorTool;
    tools << new CAshaderTool;
    //
    //-------------------------

    m_currentTool = tools[0];

    QListIterator<CAtool*> toolIt( tools );

    while( toolIt.hasNext() )
    {
        CAtool* tool = toolIt.next();

        m_toolNames << tool->getName();
        m_tools[ tool->getName() ] = tool;
    }
}

CAtoolManager::~CAtoolManager()
{
    QMapIterator<QString, CAtool*> toolIt( m_tools );

    while( toolIt.hasNext() ) delete toolIt.next().value();
}

void CAtoolManager::setCurrentTool( const QString& toolName )
{
    if( m_tools.contains( toolName ) )
    {
        m_currentTool->deactivate();

        m_currentTool = m_tools.value( toolName );

        m_currentTool->activate();
    }
    else CFlog::instance()->addMessage( QString( "Invalid tool name " ) + ( toolName ), CFlog::kError );
}

void CAtoolManager::setCurrentTool( QString* toolName )
{
    setCurrentTool( *toolName );
}

void CAtoolManager::clickCurrent( const CFpoint& position )
{
    m_currentTool->click( position );
}

void CAtoolManager::moveCurrent( const CFpoint& position )
{
    m_currentTool->move( position );
}

void CAtoolManager::dragCurrent( const CFpoint& position )
{
    m_currentTool->drag( position );
}

void CAtoolManager::unclickCurrent( const CFpoint& position )
{
    m_currentTool->unclick( position );
}
