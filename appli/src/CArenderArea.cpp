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
#include <QMessageBox>

#include "CArenderArea.h"

#include "CAapplication.h"
#include "CAview2D.h"
#include "CWscene.h"
#include "CWrenderSettings.h"
#include "CWcamera2D.h"


CArenderArea::CArenderArea( QObject* parent )
: 
    QObject                 ( parent     ),
    m_visible               ( false      ),
    m_resolutionX           ( 500        ),
    m_resolutionY           ( 500        ),
    m_worldTopLeftCorner    ( -1.0,  1.0 ),
    m_worldTopRightCorner   (  1.0,  1.0 ),
    m_worldBottomRightCorner(  1.0, -1.0 ),
    m_worldBottomLeftCorner ( -1.0, -1.0 ),
    m_image                 ( 0          )
{
}

void CArenderArea::setGeometry( const CFpoint& c1, const CFpoint& c2 )
{
    const CWscene&    scene    = CAapplication::instance()->getScene();
    const CWcamera2D* camera2D = scene.getDefaultCamera2D();

    CFpoint cornerMin, cornerMax;
    if( c1[0] < c2[0] ) { cornerMin[0] = c1[0]; cornerMax[0] = c2[0]; }
    else                { cornerMin[0] = c2[0]; cornerMax[0] = c1[0]; }
    if( c1[1] < c2[1] ) { cornerMin[1] = c1[1]; cornerMax[1] = c2[1]; }
    else                { cornerMin[1] = c2[1]; cornerMax[1] = c1[1]; }

    CFpoint corner( cornerMin ); m_worldTopLeftCorner     = camera2D->ScreenToWorld(corner);
    corner[0] = cornerMax[0];    m_worldTopRightCorner    = camera2D->ScreenToWorld(corner);
    corner[1] = cornerMax[1];    m_worldBottomRightCorner = camera2D->ScreenToWorld(corner);
    corner[0] = cornerMin[0];    m_worldBottomLeftCorner  = camera2D->ScreenToWorld(corner);

    
    unsigned int windowX = static_cast<unsigned int>( cornerMax[0] - cornerMin[0] ) + 1;
    unsigned int windowY = static_cast<unsigned int>( cornerMax[1] - cornerMin[1] ) + 1;

    const CWrenderSettings* rt = scene.getRenderSettings();
    if( rt->input( CWrenderSettings::kUseCustomResolution ).value<bool>() )
    {
        // Use specified resolution, preserving aspect ratio
        m_resolutionX = rt->input( CWrenderSettings::kWidth  ).value<int>();
        m_resolutionY = rt->input( CWrenderSettings::kHeight ).value<int>();

        if( m_resolutionY == 0 )
        {
            m_resolutionY = static_cast<unsigned int>( m_resolutionX * windowY / static_cast<float>( windowX ) );
        }
    }
    else
    {
        // Use same resolution as current camera view
        m_resolutionX = windowX;
        m_resolutionY = windowY;
    }
}

void CArenderArea::draw( void ) const
{
    if( ! m_visible ) return;

    //  Draw the render result
    m_image->Draw( m_worldTopLeftCorner, m_worldTopRightCorner - m_worldTopLeftCorner, m_worldBottomLeftCorner - m_worldTopLeftCorner );

    //  Draw the frame
    //
    glColor3f( 0.6f, 0.9f, 0.6f );

    glBegin( GL_LINE_STRIP );

        m_worldTopLeftCorner.Draw();
        m_worldTopRightCorner.Draw();
        m_worldBottomRightCorner.Draw();
        m_worldBottomLeftCorner.Draw();
        m_worldTopLeftCorner.Draw();
    glEnd();
}

void CArenderArea::saveAs( const QString* fileName ) const
{
    if( m_image ) m_image->save( *fileName );
}

void CArenderArea::render()
{
    if( m_image ) delete m_image;

    m_image = new CFglImage( m_resolutionX, m_resolutionY );

    m_visible = true;

    CWscene&        scene( CAapplication::instance()->getScene() );
    unsigned int    x, y;
    CFcolor         color;
    bool            endPass;

    float           pixelSize =
                    (
                        ( m_worldTopRightCorner   - m_worldTopLeftCorner ).Length() / static_cast<CFfloat>( m_resolutionX ) +
                        ( m_worldBottomLeftCorner - m_worldTopLeftCorner ).Length() / static_cast<CFfloat>( m_resolutionY )

                    ) / 2.0;

    CFcircle        ray( 0.0, 0.0, pixelSize );

    m_image->ItReset();

    CAview2D* view2D( qobject_cast<CAview2D*>( parent() ) );
    view2D->resetAbortState();

    bool abort = false;
    while( !abort && !m_image->ItDone() )
    {
        m_image->ItPixel( x, y );   // Fetch current pixel

        ray = m_worldTopLeftCorner +
              ( m_worldTopRightCorner   - m_worldTopLeftCorner ) * ( 0.5 + static_cast<CFfloat>( x ) ) / m_resolutionX +
              ( m_worldBottomLeftCorner - m_worldTopLeftCorner ) * ( 0.5 + static_cast<CFfloat>( y ) ) / m_resolutionY;

        scene.render( ray, color );

        m_image->SetPixel( color );

        m_image->ItNext( endPass );

        if( endPass )
        {
            CAapplication::processEvents();
            abort = view2D->getAbortState();
            view2D->updateGL();   // Draw finished pass
        }
    }
}



