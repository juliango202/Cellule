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
#include "NWrealWidget.h"

#include "NWplug.h"
#include "NWinputAttribute.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QSlider>

NWrealWidget::NWrealWidget( NWplug* realPlug, QWidget* parent )
:
    QWidget          ( parent                       ),
    m_realPlug       ( realPlug                     ),
    m_validator      ( new QDoubleValidator( this ) ),
    m_textEntry      ( new QLineEdit       ( this ) ),
    m_slider         ( 0                            ),
    m_sliderPrecision( 100                          )
{
    CFfloat realValue = m_realPlug->value<CFfloat>();

    //  Layout
    //
    QHBoxLayout* layout = new QHBoxLayout;
    
    layout->setContentsMargins( 0, 0, 0, 0    );
    layout->setAlignment      ( Qt::AlignLeft );

    //  Text entry
    //
    m_textEntry->setMaximumWidth( 60          );
    m_textEntry->setValidator   ( m_validator );

    m_textEntry->setText( QString( "%1" ).arg( realValue ) );
    
    connect( m_textEntry, SIGNAL( editingFinished() ), SLOT( updateFromText( void ) ) );

    layout->addWidget( m_textEntry );
    
    //  Slider
    //
    if( m_realPlug->direction() == NWplug::kInput )
    {
        const NWinputAttribute* attr = static_cast<const NWinputAttribute*>( m_realPlug->attribute() );

        m_slider = new QSlider( Qt::Horizontal, this );

        m_sliderRange[0] = attr->minimum().isValid() ? attr->minimum().value<CFfloat>() : 0.0;
        m_sliderRange[1] = attr->maximum().isValid() ? attr->maximum().value<CFfloat>() : 1.0;

        m_slider->setMinimum( 0                 );
        m_slider->setMaximum( m_sliderPrecision );

        updateFromText();
        
        connect( m_slider, SIGNAL( valueChanged( int ) ), SLOT( updateFromSlider( void ) ) );

        layout->addWidget( m_slider );
    }
    
    setLayout( layout );
}

void NWrealWidget::updateFromText( void )
{
    double value = m_textEntry->text().toDouble();

    //  Set plug
    //
    m_realPlug->setValue<CFfloat>( value );

    //  Set slider
    //
         if( value < m_sliderRange[0] ) m_sliderRange[0] = 2.0 * value - m_sliderRange[1];
    else if( value > m_sliderRange[1] ) m_sliderRange[1] = 2.0 * value - m_sliderRange[0];

    m_slider->blockSignals( true );

    m_slider->setValue( m_sliderPrecision * ( value - m_sliderRange[0] ) / ( m_sliderRange[1] - m_sliderRange[0] ) );

    m_slider->blockSignals( false );
}

void NWrealWidget::updateFromSlider()
{
    if( ! m_slider ) return;

    double value = m_sliderRange[0] + ( static_cast<CFfloat>( m_slider->value() ) / m_sliderPrecision ) * ( m_sliderRange[1] - m_sliderRange[0] );

    //  Set plug
    //
    m_realPlug->setValue<CFfloat>( value );

    //  Set text entry
    //
    m_textEntry->blockSignals( true );

    m_textEntry->setText( QString( "%1" ).arg( value ) );

    m_textEntry->blockSignals( false );
}
