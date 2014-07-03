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
#include "CFglImage.h"
#include "CFpoint.h"
#include "CFcolor.h"

#include <assert.h>

const unsigned int ADAM7_NB_PASSES = 7;


// ---------------------------------------------------------------------------------- //
// --------------------------------  CFglImageBlock  -------------------------------- //
// ---------------------------------------------------------------------------------- //

// Information on Adam7 progressive display algorithm can be found at:
// http://www.w3.org/TR/REC-png  -> section [10.9]
const unsigned int CFglImageBlock::Adam7_Starting_Row [7] = { 0, 0, 4, 0, 2, 0, 1 };
const unsigned int CFglImageBlock::Adam7_Starting_Col [7] = { 0, 4, 0, 2, 0, 1, 0 };
const unsigned int CFglImageBlock::Adam7_Row_Increment[7] = { 8, 8, 8, 4, 4, 2, 2 };
const unsigned int CFglImageBlock::Adam7_Col_Increment[7] = { 8, 8, 4, 4, 2, 2, 1 };
const unsigned int CFglImageBlock::Adam7_Block_Width  [7] = { 8, 4, 4, 2, 2, 1, 1 };
const unsigned int CFglImageBlock::Adam7_Block_Height [7] = { 8, 8, 4, 4, 2, 2, 1 };


CFglImageBlock::CFglImageBlock( QImage* parent,
                                unsigned int blockOffsetX,  unsigned int blockOffsetY,
                                unsigned int blockWidth,    unsigned int blockHeight ) :
                                m_parent(parent),
                                m_blockOffsetX( blockOffsetX ), m_blockOffsetY( blockOffsetY ),
                                m_blockWidth( blockWidth ),     m_blockHeight( blockHeight )
{
    glGenTextures( 1, &m_glId );
    glBindTexture   ( GL_TEXTURE_2D, m_glId );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    m_pixels   = new unsigned char [ BLOCK_SIZE * BLOCK_SIZE * 3 ];
}


CFglImageBlock::~CFglImageBlock()
{
    if( m_pixels )
    {
        glDeleteTextures( 1, &m_glId );
        delete[] m_pixels;
    }
}


void CFglImageBlock::ItReset( void )
{
    m_currentPass   = 0;
    m_itX = Adam7_Starting_Col[0];
    m_itY = Adam7_Starting_Row[0];
}

void CFglImageBlock::Fill( const CFcolor& color )
{
    unsigned char rgb[3];

    rgb[0] = static_cast< unsigned char >( color.asRGB()[0] * 255.9 );
    rgb[1] = static_cast< unsigned char >( color.asRGB()[1] * 255.9 );
    rgb[2] = static_cast< unsigned char >( color.asRGB()[2] * 255.9 );

    m_parent->setPixel( m_itX + m_blockOffsetX, m_itY + m_blockOffsetY, qRgb( rgb[0], rgb[1], rgb[2] ) );

    unsigned int fillWidth  = std::min( Adam7_Block_Width [ m_currentPass ], m_blockWidth  - m_itX );
    unsigned int fillHeight = std::min( Adam7_Block_Height[ m_currentPass ], m_blockHeight - m_itY );

    for( unsigned int x = m_itX; x < m_itX + fillWidth;  x++ )
    {
        for( unsigned int y = m_itY; y < m_itY + fillHeight; y++ )
        {
            std::memcpy( &m_pixels[ x * 3 + y * BLOCK_SIZE * 3 ], rgb, 3 );
        }
    }
}


void CFglImageBlock::ItNext( bool&  endPass )
{
    m_itX += Adam7_Col_Increment[m_currentPass];

    if( m_itX >= m_blockWidth )
    {
        m_itY += Adam7_Row_Increment[m_currentPass];
        if( m_itY >= m_blockHeight )                        // End of current pass
        {
            if( m_currentPass < ADAM7_NB_PASSES-1 );
            {
                m_currentPass++;
                m_itX = Adam7_Starting_Col[m_currentPass];
                m_itY = Adam7_Starting_Row[m_currentPass];
                if( m_itX >= m_blockWidth || m_itY >= m_blockHeight )   // Block is smaller than 4 pixels: clamp to the last valid pixel
                {
                    m_itX = m_blockWidth-1;
                    m_itY = m_blockHeight-1;
                }
            }

            // Load current pass into texture
            glBindTexture( GL_TEXTURE_2D, m_glId );
            glTexImage2D ( GL_TEXTURE_2D, 0, 3, BLOCK_SIZE, BLOCK_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixels );

            endPass = true;
            return;
        }

        m_itX = Adam7_Starting_Col[m_currentPass];
    }

    endPass = false;
}

void CFglImageBlock::ItPixel( unsigned int& x, unsigned int& y ) const
{
    x   = m_itX + m_blockOffsetX;
    y   = m_itY + m_blockOffsetY;
}

void CFglImageBlock::Draw( const CFpoint& pos, const CFpoint& iDirPix, const CFpoint& jDirPix ) const
{
    assert( m_pixels );

    CFpoint corner[4];
    CFpoint uvMax;
    // Compute UVs according block size
    uvMax[0] = static_cast<CFfloat>( m_blockWidth  ) / BLOCK_SIZE;
    uvMax[1] = static_cast<CFfloat>( m_blockHeight ) / BLOCK_SIZE;

    // Compute world coordinates of the 4 corners of the block
    corner[0] = pos + iDirPix * m_blockOffsetX + jDirPix * m_blockOffsetY;
    corner[1] = corner[0] + iDirPix * m_blockWidth;
    corner[2] = corner[1] + jDirPix * m_blockHeight;
    corner[3] = corner[0] + jDirPix * m_blockHeight;

    glBindTexture( GL_TEXTURE_2D, m_glId );
    glBegin( GL_QUADS );
        glTexCoord2f( 0.0f,       0.0f      ); corner[0].Draw();
        glTexCoord2f( uvMax[0],   0.0f      ); corner[1].Draw();
        glTexCoord2f( uvMax[0],   uvMax[1]  ); corner[2].Draw();
        glTexCoord2f( 0.0f,       uvMax[1]  ); corner[3].Draw();
    glEnd(); 
}



// ---------------------------------------------------------------------------------- //
// --------------------------------     CFglImage    -------------------------------- //
// ---------------------------------------------------------------------------------- //

CFglImage::CFglImage( unsigned int width, unsigned int height ) :
    QImage( width, height, QImage::Format_RGB32 )
{

    //  Partition in blocks 
    //
    unsigned int blockCountX = width / CFglImageBlock::BLOCK_SIZE;
    unsigned int lastBlockWidth = CFglImageBlock::BLOCK_SIZE;
    if( width % CFglImageBlock::BLOCK_SIZE != 0 )
    { 
        blockCountX++; 
        lastBlockWidth = width % CFglImageBlock::BLOCK_SIZE; 
    }

    unsigned int blockCountY = height / CFglImageBlock::BLOCK_SIZE;
    unsigned int lastBlockHeight = CFglImageBlock::BLOCK_SIZE;
    if( height % CFglImageBlock::BLOCK_SIZE != 0 )
    { 
        blockCountY++; 
        lastBlockHeight = height % CFglImageBlock::BLOCK_SIZE; 
    }

    for( unsigned int blockX = 0; blockX < blockCountX; blockX++ )
    for( unsigned int blockY = 0; blockY < blockCountY; blockY++ )
    {
        unsigned blockWidth  = (blockX == blockCountX-1) ? lastBlockWidth  : CFglImageBlock::BLOCK_SIZE;
        unsigned blockHeight = (blockY == blockCountY-1) ? lastBlockHeight : CFglImageBlock::BLOCK_SIZE;

        m_blocks.push_back( new CFglImageBlock( this,
                                                blockX * CFglImageBlock::BLOCK_SIZE,
                                                blockY * CFglImageBlock::BLOCK_SIZE,
                                                blockWidth, blockHeight ) );
    }

    ItReset();
}


CFglImage::~CFglImage( void )
{
    // Delete all blocks
    std::vector<CFglImageBlock*>::iterator itBlock;
    for( itBlock = m_blocks.begin(); itBlock != m_blocks.end(); itBlock++ )
        delete (*itBlock);
}


void CFglImage::ItReset( void )
{
    m_currentBlock = m_blocks.begin();
    m_currentPass = 0;
    m_itDone = false;

    std::vector<CFglImageBlock*>::iterator itBlock;
    for( itBlock = m_blocks.begin(); itBlock != m_blocks.end(); itBlock++ )
        (*itBlock)->ItReset();
}


void CFglImage::ItNext( bool& endPass )
{
    bool blockEndPass;

    (*m_currentBlock)->ItNext(blockEndPass);

    if( blockEndPass )                                  // End of bloc
    {
        m_currentBlock++;
        if( m_currentBlock == m_blocks.end() )          // End of image
        {
            m_currentBlock = m_blocks.begin();

            if( m_currentPass == ADAM7_NB_PASSES-1 )    // End of last path
                m_itDone = true;
            else
                m_currentPass++;

            glBindTexture( GL_TEXTURE_2D, 0 );
            endPass = true;
            return;
        }

    }
    endPass = false;
}

void CFglImage::SetPixel( const CFcolor& color )
{
    (*m_currentBlock)->Fill( color );
}

void CFglImage::ItPixel( unsigned int& x, unsigned int& y ) const
{
    (*m_currentBlock)->ItPixel( x, y );

    assert( static_cast<int>(x) < width()  );
    assert( static_cast<int>(y) < height() );
}

void CFglImage::Draw( const CFpoint&  pos, const CFpoint&  iDir, const CFpoint&  jDir ) const
{
    glColor3f( 1.0f, 1.0f, 1.0f );

    CFpoint iDirPix = iDir / width();
    CFpoint jDirPix = jDir / height();

    std::vector<CFglImageBlock*>::const_iterator itBlock;
    for( itBlock = m_blocks.begin(); itBlock != m_blocks.end(); itBlock++ )
        (*itBlock)->Draw( pos, iDirPix, jDirPix );

    glBindTexture( GL_TEXTURE_2D, 0 );
}

