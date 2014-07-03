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
#include "NWintWidget.h"

#include "NWplug.h"
#include "NWinputAttribute.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>

NWintWidget::NWintWidget( NWplug* intPlug, QWidget* parent )
:
    QWidget    ( parent                    ),
    m_intPlug  ( intPlug                   ),
    m_validator( new QIntValidator( this ) ),
    m_textEntry( new QLineEdit    ( this ) ),
    m_slider   ( 0                         )
{
    int intValue = m_intPlug->value<int>();

    //  Layout
    //
    QHBoxLayout* layout = new QHBoxLayout;
    
    layout->setContentsMargins( 0, 0, 0, 0    );
    layout->setAlignment      ( Qt::AlignLeft );
    
    //  Text entry
    //
    m_textEntry->setMaximumWidth( 60          );
    m_textEntry->setValidator   ( m_validator );

    m_textEntry->setText( QString( "%1" ).arg( intValue ) );
    
    connect( m_textEntry, SIGNAL( editingFinished() ), SLOT( updateFromText( void ) ) );

    layout->addWidget( m_textEntry );

    //  Slider
    //
    if( m_intPlug->direction() == NWplug::kInput )
    {
        const NWinputAttribute* attr( static_cast<const NWinputAttribute*>( intPlug->attribute() ) );

        m_slider = new QSlider( Qt::Horizontal, this );

        m_slider->setMinimum( attr->minimum().isValid() ? attr->minimum().value<int>() : 0   );
        m_slider->setMaximum( attr->maximum().isValid() ? attr->maximum().value<int>() : 100 );

        updateFromText();
        
        connect( m_slider, SIGNAL( valueChanged( int ) ), SLOT( updateFromSlider( void ) ) );

        layout->addWidget( m_slider  );
    }
    
    setLayout( layout );
}

void NWintWidget::updateFromText( void )
{
    int value = m_textEntry->text().toInt();

    //  Set plug
    //
    m_intPlug->setValue<int>( value );

    //  Set slider
    //
         if( value < m_slider->minimum() ) m_slider->setMinimum( 2 * value - m_slider->maximum() );
    else if( value > m_slider->maximum() ) m_slider->setMaximum( 2 * value - m_slider->minimum() );

    m_slider->blockSignals( true );

    m_slider->setValue( value );

    m_slider->blockSignals( false );
}

void NWintWidget::updateFromSlider()
{
    if( ! m_slider ) return;

    int value = m_slider->value();

    //  Set plug
    //
    m_intPlug->setValue<int>( value );

    //  Set text entry
    //
    m_textEntry->blockSignals( true );

    m_textEntry->setText( QString( "%1" ).arg( value ) );

    m_textEntry->blockSignals( false );
}
