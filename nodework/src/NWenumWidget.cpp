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
#include "NWenumWidget.h"

#include "NWplug.h"

#include "CFenum.h"

NWenumWidget::NWenumWidget( NWplug* enumPlug, QWidget* parent )
:
    QComboBox ( parent   ),
    m_enumPlug( enumPlug )
{
    unsigned int i;

    for( i = 0; i < m_enumPlug->value<CFenum>().valuesCount(); i++ )
    {
        addItem( m_enumPlug->value<CFenum>().valueName( i ) );
    }

    connect( this, SIGNAL( activated( int ) ), SLOT( updatePlug( int ) ) );
}

void NWenumWidget::paintEvent( QPaintEvent* event )
{
    setCurrentIndex( m_enumPlug->value<CFenum>().asInt() );

    QComboBox::paintEvent( event );
}

void NWenumWidget::updatePlug( int valueIndex )
{
    m_enumPlug->editableValue<CFenum>().set( valueIndex );
    m_enumPlug->notifyChange();
}
