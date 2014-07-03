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
#ifndef __NWrealWidget_h__
#define __NWrealWidget_h__

#include "CFtypes.h"

#include <QtGui/QLineEdit>
#include <QtGui/QDoubleValidator>

class NWplug;
class QDoubleSpinBox;
class QSlider;

//! \brief  Class description
//! \author Author
//! \date   Year Month Day
//!
class NWrealWidget : public QWidget
{
    Q_OBJECT

public:
                            NWrealWidget        ( NWplug*           realPlug,
                                                  QWidget*          parent          );

protected slots:

    void                    updateFromText      ( void                              );
    void                    updateFromSlider    ( void                              );

protected:

    NWplug*                 m_realPlug;

    QDoubleValidator*       m_validator;
    QLineEdit*              m_textEntry;
    QSlider*                m_slider;
    unsigned int            m_sliderPrecision;
    CFfloat                 m_sliderRange[2];
};

#endif //__NWrealWidget_h__
