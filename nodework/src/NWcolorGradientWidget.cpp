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
#include "NWcolorGradientWidget.h"

#include "NWplug.h"

#include "CFlog.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QColorDialog>

const int NWcolorGradientWidget::m_handleRadius   = 5;
const int NWcolorGradientWidget::m_handleSize     = 2 * m_handleRadius;
const int NWcolorGradientWidget::m_handleAreaSize = m_handleSize * 4;

NWcolorGradientWidget::NWcolorGradientWidget( NWplug* gradientPlug, QWidget* parent )
:
    QWidget          ( parent                   ),
    m_gradientPlug   ( gradientPlug             ),
    m_selectedHandle ( 0                        ),
    m_dragging       ( false                    )
{
    setMinimumHeight( 2 * m_handleAreaSize );

    //  Setup interpolation menu
    //
    m_interpolationMenu = new QMenu( this );

    QAction* constantAction = m_interpolationMenu->addAction( "Constant" );
    QAction* linearAction   = m_interpolationMenu->addAction( "Linear"   );
    QAction* smoothAction   = m_interpolationMenu->addAction( "Smooth"   );

    connect( constantAction, SIGNAL( triggered() ), SLOT( setConstantInterp() ) );
    connect( linearAction,   SIGNAL( triggered() ), SLOT( setLinearInterp()   ) );
    connect( smoothAction,   SIGNAL( triggered() ), SLOT( setSmoothInterp()   ) );

    //  Setup color handle menu
    //
    m_handleMenu = new QMenu( this );

    QAction* changeColorAction = m_handleMenu->addAction( "Color..." );
    m_deleteColorAction        = m_handleMenu->addAction( "Delete" );

    connect( changeColorAction,   SIGNAL( triggered() ), SLOT( changeColor() ) );
    connect( m_deleteColorAction, SIGNAL( triggered() ), SLOT( deleteColor() ) );
}

QSize NWcolorGradientWidget::sizeHint( void ) const
{
    return QSize( 200, 2 * m_handleAreaSize );
}

void NWcolorGradientWidget::resizeEvent( QResizeEvent* event )
{
    //  Compute the area where the gradient will be drawn
    //
    m_gradiantArea = rect();

    m_gradiantArea.adjust( m_handleRadius + 2, 1, -m_handleRadius - 3, -m_handleAreaSize - 2 );
}

void NWcolorGradientWidget::paintEvent( QPaintEvent* event )
{
    QPainter painter( this );
    
    //  Draw gradient
    //
    const CFcolorGradient& colorGradient( m_gradientPlug->value<CFcolorGradient>() );

    int pixel; for( pixel = m_gradiantArea.left(); pixel <= m_gradiantArea.right(); pixel++ )
    {
        painter.setPen( colorGradient.evaluate( toGradientSpace( pixel ) ).asQColor() );
        painter.drawLine
        (
            QPoint( pixel, m_gradiantArea.bottom() ),
            QPoint( pixel, m_gradiantArea.top()    )
        );
    }

    painter.setPen  ( Qt::black      );
    painter.drawRect( m_gradiantArea );
    
    //  For each color handle
    //
    int colorCount = colorGradient.keyCount();
    
    m_handlePositions.resize( colorCount );
    
    int handleIdx; for( handleIdx = 0; handleIdx < colorCount; handleIdx++ )
    {
        //  Compute the handle position
        //  ( integer widget space )
        //
        QPoint handlePos
        (
            toWidgetSpace( colorGradient[ handleIdx ].position() ),
            m_gradiantArea.bottom() + m_handleRadius + 4
        );
        
        //  If this handle is over previously drawn ones
        //  offset it to the bottom
        //
        int prevHandleIdx; for( prevHandleIdx = 0; prevHandleIdx < handleIdx; prevHandleIdx++ )
        {
            if( ( handlePos.x() > m_handlePositions[ prevHandleIdx ].x() - m_handleSize ) &&
                ( handlePos.x() < m_handlePositions[ prevHandleIdx ].x() + m_handleSize ) )
            {
                handlePos.setY( m_handlePositions[ prevHandleIdx ].y() + m_handleSize );
            }
        }
        
        //  Store handle position for later click detection
        //
        m_handlePositions[ handleIdx ] = handlePos;
        
        //  Draw the color handle
        //
        if( handleIdx == m_selectedHandle ) painter.setPen( QPen( Qt::white ) );
        else                                painter.setPen( QPen( Qt::black ) );

        painter.setBrush( QBrush( colorGradient[ handleIdx ].value().asQColor() ) );
        painter.drawEllipse( handlePos, m_handleRadius, m_handleRadius );
    }
}

void NWcolorGradientWidget::mousePressEvent( QMouseEvent* event )
{
    unsigned int i;

    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );
    
    //  If clicked on the gradient
    //
    if( m_gradiantArea.contains( event->pos(), true ) )
    {
        //  Insert a new color handle
        //
        if( event->button() == Qt::LeftButton )
        {
            CFfloat position = toGradientSpace( event->x() );
            
            m_selectedHandle = colorGradient.insertKey
            (
                CFcolorGradient::Key( position, colorGradient.evaluate( position ) )
            );

            m_gradientPlug->notifyChange();
            update();
    
            //  And begin dragging it
            //
            m_dragging = true;
        }
        else if( event->button() == Qt::RightButton )
        {
            m_interpolationMenu->exec( event->globalPos() );
        }
    }

    //  Else find ( if any ) the handle which was clicked on
    //
    else for( i = 0; i < m_handlePositions.size(); i++ )
    {
        if( ( event->pos() - m_handlePositions[ i ] ).manhattanLength() < ( m_handleRadius + 3 ) )
        {
            //  Select the handle which was clicked on
            //
            m_selectedHandle = i;

            update();

            //  Show context menu
            //
            if( event->button() == Qt::RightButton )
            {
                m_deleteColorAction->setEnabled( colorGradient.keyCount() > 1 );
                m_handleMenu->exec( event->globalPos() );
            }

            //  Or begin dragging the handle
            //
            else if( event->button() == Qt::LeftButton )
            {
                m_dragging = true;
            }

            break;
        }
    }
}

void NWcolorGradientWidget::mouseMoveEvent( QMouseEvent* event )
{
    if( m_dragging )
    {
        CFfloat position = toGradientSpace( event->x() );
        
        if( position < 0.0 ) position = 0.0;
        if( position > 1.0 ) position = 1.0;

        CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

        m_selectedHandle = colorGradient.moveKey( m_selectedHandle, position );

        m_gradientPlug->notifyChange();
        update();
    }
}

void NWcolorGradientWidget::mouseReleaseEvent( QMouseEvent* event )
{
    m_dragging = false;
}

void NWcolorGradientWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    changeColor();
}

void NWcolorGradientWidget::setConstantInterp( void )
{
    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

    colorGradient.setInterpolation( CFcolorGradient::kConstant );

    m_gradientPlug->notifyChange();
    update();
}

void NWcolorGradientWidget::setLinearInterp( void )
{
    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

    colorGradient.setInterpolation( CFcolorGradient::kLinear );

    m_gradientPlug->notifyChange();
    update();
}

void NWcolorGradientWidget::setSmoothInterp( void )
{
    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

    colorGradient.setInterpolation( CFcolorGradient::kSmooth );

    m_gradientPlug->notifyChange();
    update();
}

void NWcolorGradientWidget::changeColor( void )
{
    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

    QColor newColor( QColorDialog::getColor( colorGradient[ m_selectedHandle ].value().asQColor(), this ) );

    if( newColor.isValid() )
    {
        colorGradient.setKeyValue( m_selectedHandle, newColor );

        m_gradientPlug->notifyChange();
        update();
    }
}

void NWcolorGradientWidget::deleteColor( void )
{
    CFcolorGradient& colorGradient( m_gradientPlug->editableValue<CFcolorGradient>() );

    colorGradient.deleteKey( m_selectedHandle );
    
    m_gradientPlug->notifyChange();
    update();
}

CFfloat NWcolorGradientWidget::toGradientSpace( int position ) const
{
    return CFfloat( position - m_gradiantArea.left() ) / m_gradiantArea.width();
}

int NWcolorGradientWidget::toWidgetSpace( CFfloat position ) const
{
    return m_gradiantArea.left() + position * m_gradiantArea.width();
}
