//  (C) Copyright 2005-2008 Benjamin Godon ( aka bEngo )
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
#ifndef __CFlog_h__
#define __CFlog_h__

#include <QtCore/QObject>

/**
Log debug, information, warning, and error messages.

	@author 
*/
class CFlog : public QObject
{
    Q_OBJECT

protected:
                        CFlog           ( QObject*          parent      );
public:
    static CFlog*       instance        ( void                          );

public:

    enum MsgType
    {
        kDebug,
        kInfo,
        kWarning,
        kError
    };

public slots:

    void                addMessage      ( const QString&    content,
                                          MsgType           type = kDebug  );
signals:

    void                textAdded       ( const QString&    newText        );
    void                htmlAdded       ( const QString&    newHtml        );

protected:

    static CFlog*       m_instance;
};

#endif //__CFlog_h__
