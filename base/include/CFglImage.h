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
#ifndef __CFglImage_h__
#define __CFglImage_h__

#include "GL/gl.h"
#include <QtGui/QImage>

class CFpoint;
class CFcolor;
class CFglImageBlock;


//! \brief  A QImage partitionned in block to be able to display using textures ( support progressive display )
//! \author bEngo-JuL
//! \date   2008 April
//!
class CFglImage : public QImage
{
public:

                                CFglImage           ( unsigned int      width,
                                                      unsigned int      height  );

                                ~CFglImage          ( void                      );

    //  Functions to Iterate through the image pixels ( non-linear iteration for progressive display )
    void                        ItReset             ( void                      );
    bool                        ItDone              ( void                      ) const { return m_itDone; }
    void                        ItNext              ( bool&             endPass );

    //  Return the current coordinate of the current pixel
    void                        ItPixel             ( unsigned int&     x,
                                                      unsigned int&     y       ) const;

    //  Assign a color to the current pixel
    void                        SetPixel            ( const CFcolor&    color   );

    //  Draw the image according to the ( pos, iDir->, jDir-> ) base, in world coordinates
    void                        Draw                ( const CFpoint&    pos,
                                                      const CFpoint&    iDir,
                                                      const CFpoint&    jDir    ) const;
private:

    std::vector<CFglImageBlock*>                    m_blocks;
    std::vector<CFglImageBlock*>::iterator          m_currentBlock;
    unsigned int                                    m_currentPass;
    bool                                            m_itDone;
};



//! \brief  An Image block that can be loaded into one OpenGL texture(support Adam7 progressive display)
//! \author JuL
//! \date   2008 April
//!
class CFglImageBlock {

public:
    static const unsigned int BLOCK_SIZE = 64;
    static const unsigned int Adam7_Starting_Row[];
    static const unsigned int Adam7_Starting_Col[];
    static const unsigned int Adam7_Row_Increment[];
    static const unsigned int Adam7_Col_Increment[];
    static const unsigned int Adam7_Block_Width[];
    static const unsigned int Adam7_Block_Height[];


                                CFglImageBlock      ( QImage*           parent,
                                                      unsigned int      blockOffsetX,
                                                      unsigned int      blockOffsetY,
                                                      unsigned int      blockWidth,
                                                      unsigned int      blockHeight );

                                ~CFglImageBlock     ( void                          );

    void                        Draw                ( const CFpoint&    pos,
                                                      const CFpoint&    iDirPix,
                                                      const CFpoint&    jDirPix     ) const;

    void                        ItReset             ( void                          );
    void                        ItNext              ( bool&             endPass     );
    void                        ItPixel             ( unsigned int&     x,
                                                      unsigned int&     y           ) const;

    void                        Fill                ( const CFcolor&   color        );


private:
    QImage*         m_parent;
    unsigned int    m_blockOffsetX, m_blockOffsetY;
    unsigned int    m_blockWidth,   m_blockHeight;
    unsigned int    m_itX,          m_itY;
    unsigned int    m_currentPass;
    GLuint          m_glId;
    unsigned char*  m_pixels;
};


#endif //__CFglImage_h__
