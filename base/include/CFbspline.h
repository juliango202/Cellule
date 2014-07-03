#ifndef _CFBSPLINE_H
#define _CFBSPLINE_H

//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#include "CFpoint.h"

#include <cmath>
#include <vector>

//! \brief  This class implements an Uniform Periodic Cubic Bspline curve
//! \author JuL
//! \date   2008 May
//!

class CFbspline
{

public:
	CFbspline                       ( void                          )       : m_smoothness(-1) {}

        void            clear           ( void                          )       { m_points.clear(); m_smoothness = -1;  }   
        bool            empty           ( void                          ) const { return m_points.empty();              }   

        void            addControlPoint ( const CFpoint& p              )       { m_points.push_back(p);                }   
        void            close           ( void                          )       { m_points.insert( m_points.end(), m_points.begin(), m_points.begin()+3 );  }    

        bool            is_valid        ( void                          ) const { return m_points.size() >= 4;          }   
        unsigned int    getNbPoints     ( void                          ) const { return m_points.size();               }   
        const CFpoint&  getCenter       ( void                          ) const { return m_center;                      }
   
        CFfloat         getSmoothness   ( void                          ) const { return m_smoothness;                  }   
        void            setSmoothness   ( CFfloat        smoothness     )       { m_smoothness = smoothness;            }   


        /// Assign a center to the curve
        void setCenter( const CFpoint& center );

        /// Draw the bspline using OpenGL
        void draw( bool drawCtrlPts = false ) const;

        /// Evaluates the bspline curve at specific parameter value
        /// Parameter range is  0 <= t < 1
        const CFpoint evalCubic (CFfloat T) const { return evalCubic_(T*(getNbPoints()-3)+3 ); }
        /// Parameter range is  (curve_order-1) <= t < m_points.size()
        const CFpoint evalCubic_(CFfloat T) const;

        /// Use Binary search algo to find intersection between the line(m_center -> p) and the bspline
        CFpoint evalIntersect( CFpoint p, CFfloat precision = 0.0001 ) const;


protected:

        /// The curve's control points
	std::vector<CFpoint> m_points;

        /// Smoothness of the curve: 0 -> control polygon, 1 -> ellipsoid
        CFfloat m_smoothness;

        /// Center of the spline (used for intersection computation)
        /// It is equal to the center of the voronoi cell in most cases
        CFpoint m_center;

        /// 4 points equally spaced along the curve
        /// This is used for initializing the binary search
        CFpoint m_4[4];
};



#endif // _CFBSPLINE_H
