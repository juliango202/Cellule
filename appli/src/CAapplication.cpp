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
#include "CAapplication.h"

#include "CAview2D.h"

#include "NWgraphEditor.h"
#include "NWselection.h"
#include "NWnodeEditor.h"
#include "CFlog.h"

#include <kross/core/action.h>

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>

QString CAapplication::m_version( "0.1 alpha" );

CAapplication::CAapplication( int& argc, char** argv )
:
    QApplication( argc, argv )
{
    m_mainWin = new QMainWindow;

    m_mainWin->setWindowIcon( QIcon( "icons/48x48/cellule.png" ) );

    m_scriptEngine = new CAscriptEngine( this );

    connect( &m_scene, SIGNAL( renamed( QString* ) ), SLOT( setTitle( QString* ) ) );

    CFdata::registerBasicTypes();
    initGUI();

    m_scene.clear( true );
}

CAapplication::~CAapplication()
{
    delete m_mainWin;
}

CAapplication* CAapplication::instance( void )
{
    return static_cast<CAapplication*>( QApplication::instance() );
}

void CAapplication::initGUI( void )
{
    QString scriptFileName( "./scripts/initGUI.py" );

    Kross::Action script( 0, "init" );

    script.setFile  ( scriptFileName        );
    script.addObject( this, "CAapplication" );

    script.trigger();
}

int CAapplication::exec( void )
{
    m_mainWin->show();

    return QApplication::exec();
}

void CAapplication::setTitle( QString* fileName )
{
    m_mainWin->setWindowTitle( QString( "Cellule %1: %2" ).arg( m_version ).arg( *fileName ) );
}

QWidget* CAapplication::createView2D( QWidget* parent )
{
    return new CAview2D( parent, getScene().getDefaultCamera2D() );
}

QWidget* CAapplication::createGraphEditor( QWidget* parent )
{
    NWgraphEditor* graphEditor = new NWgraphEditor( parent, &m_scene );
    
    graphEditor->connect( &m_scene, SIGNAL( nodeAdded( NWnode* ) ), SLOT( addContent( NWnode* ) ) );
    
    return graphEditor;
}

QWidget* CAapplication::createNodeEditor( QWidget* parent )
{
    NWnodeEditor* nodeEditor = new NWnodeEditor( parent );

    nodeEditor->connect( &m_scene.getSelection(), SIGNAL( changed( const NWselection& ) ), SLOT( setContent( const NWselection& ) ) );

    return nodeEditor;
}

void CAapplication::about( void )
{
    //  Dialog window
    //
    QDialog dialog( m_mainWin );
    
    dialog.setWindowTitle( "About Cellule" );
    
    //  Header
    //
    QFrame header( &dialog );

    header.setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QLabel icon          ( &header );
    QLabel nameAndVersion( &header );

    icon          .setPixmap( QPixmap( "icons/64x64/cellule.png" )       );
    nameAndVersion.setText  ( QString( "<b><font size=20>Cellule</font><br />version " ) + m_version + "</b>" );
    
    QHBoxLayout headerLayout( &header );
    
    headerLayout.addWidget( &icon           );
    headerLayout.addWidget( &nameAndVersion );
    
    //  About page
    //
    QTextBrowser* about = new QTextBrowser;
    
    about->setOpenExternalLinks( true );

    about->setHtml
    (
        "<p>Cellule is a free, open-source, 2D procedural texture editor.<br />"
        "A set of tools to draw vector graphics interactively,<br />"
        "which can be loaded into an external 3D renderer,<br />"
        "and get textures with much more resolution than pixel maps.</p>"
        "<p>Homepage: <a href=\"http://cellule.homeip.net\">http://cellule.homeip.net</a></p>"
    );
    
    //  License page
    //
    QTextEdit* license = new QTextEdit;
    
    license->setReadOnly( true );
    
    license->setText
    (
        "<p>Copyright 2005-2008 Benjamin Godon and contributors.<br /><br />"
        "It is free software; you can redistribute it and/or modify<br />"
        "it under the terms of the GNU General Public License as published by<br />"
        "the Free Software Foundation; either version 2 of the License, or<br />"
        "(at your option) any later version.<br />"
        "<br />"
        "Cellule is distributed in the hope that it will be useful,<br />"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of<br />"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br />"
        "See the GNU General Public License for more details.<br />"
        "<br />"
        "You should have received a copy of the GNU General Public License<br />"
        "along with Cellule; if not, write to the Free Software<br />"
        "Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA</p>"
    );
    
    //  License page
    //
    QTextBrowser* credits = new QTextBrowser;
    
    credits->setOpenExternalLinks( true );

    credits->setHtml
    (
        "<p>Conception and Development:</p>"
        "<p style=\"margin-left: 1em;\">"
        "bEngo ( Benjamin Godon )<br />"
        "<a href=\"mailto:foolscreen@gmail.com\">foolscreen@gmail.com</a>"
        "</p>"
        "<p>Contributions:</p>"
        "<p style=\"margin-left: 1em;\">"
        "julianjon<br />"
        "<a href=\"mailto:julian.jonat@gmail.com\">julian.jonat@gmail.com</a>"
        "</p>"
    );

    //  Tabs
    //
    QTabWidget tabs( &dialog );

    tabs.addTab( about,   "About"   );
    tabs.addTab( license, "License" );
    tabs.addTab( credits, "Credits" );

    QPushButton button( QIcon( "icons/22x22/dialog-close.png" ), "Close", &dialog );

    button.setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QObject::connect( &button, SIGNAL( clicked( bool ) ), &dialog, SLOT( accept() ) );

    //  Layout
    //
    QVBoxLayout layout( &dialog );

    layout.setSpacing( 10 );

    layout.addWidget( &header );
    layout.addWidget( &tabs   );
    layout.addWidget( &button, 0, Qt::AlignCenter );

    dialog.resize( 500, 350 );

    dialog.exec();
}
