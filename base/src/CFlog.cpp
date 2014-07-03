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
#include "CFlog.h"

#include <QtCore/QCoreApplication>

#include <iostream>
#include <assert.h>

CFlog* CFlog::m_instance = 0;

CFlog::CFlog( QObject* parent ) : QObject( parent ) {}

CFlog* CFlog::instance( void )
{
    if( ! m_instance )
    {
        QCoreApplication* appli= QCoreApplication::instance();
        
        assert( appli ); // QApplication must be instanciated first
        
        m_instance = new CFlog( appli );
    }

    return m_instance;
}

void CFlog::addMessage( const QString& content, MsgType type )
{
    QString text( "%1%2" );
    QString html( "<span style=\"color: %1;\">%2</span>" );

    QString header;
    QString color;

         if( type == kDebug   ) { header = "# [Debug] ";  color = "blue";   }
    else if( type == kInfo    ) { header = "# ";          color = "silver"; }
    else if( type == kWarning ) { header = "# Warning: "; color = "orange"; }
    else if( type == kError   ) { header = "# Error: ";   color = "red";    }

    text = text.arg( header ).arg( content );
    html = html.arg( color  ).arg( text    );

    std::cout << text.toStdString() << std::endl;

    emit textAdded( text );
    emit htmlAdded( html );
}
