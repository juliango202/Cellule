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
#ifndef __NWintWidget_h__
#define __NWintWidget_h__

#include <QtGui/QLineEdit>
#include <QtGui/QIntValidator>

class NWplug;
class QSpinBox;
class QSlider;

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class NWintWidget : public QWidget
{
    Q_OBJECT

public:
                            NWintWidget         ( NWplug*               intPlug,
                                                  QWidget*              parent      );

protected slots:

    void                    updateFromText      ( void                              );
    void                    updateFromSlider    ( void                              );

protected:

    NWplug*                 m_intPlug;

    QIntValidator*          m_validator;
    QLineEdit*              m_textEntry;
    QSlider*                m_slider;
    int                     m_range[2];
};

#endif //__NWintWidget_h__
