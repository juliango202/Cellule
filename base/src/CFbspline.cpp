
#include "CFbspline.h"
#include <iostream>
#include <cassert>
#include <limits>

#include <GL/glut.h>			// GLUT
#include <GL/glu.h>			// GLU
#include <GL/gl.h>			// OpenGL


void CFbspline::setCenter( const CFpoint& center )
{
    m_4[0] = evalCubic(0.0);
    m_4[1] = evalCubic(0.25);
    m_4[2] = evalCubic(0.5);
    m_4[3] = evalCubic(0.75);

    m_center = center;

    // If the given center lies outside the quadrilateral[eval(0);eval(0.25);eval(0.5);eval(0.75)], pull it inside
    // This is because evalIntersect cannot work if the center lies outside this quadrilateral
    CFpoint quadCenter = ( m_4[0] + m_4[2] )/2.0;
    CFpoint vec = center - quadCenter;

    for( unsigned int i = 0; i < 4; i++ ) {

        unsigned int i2 = (i+1)%4;
        CFfloat denominator = ( m_4[i2][1] - m_4[i][1] ) * vec[0] - ( m_4[i2][0] - m_4[i][0] ) * vec[1];
        if( denominator == 0 ) continue;

        CFfloat coordOnQuadEdge = ( ( vec[0] * (quadCenter[1]-m_4[i][1]) ) - ( vec[1] * (quadCenter[0]-m_4[i][0]) ) ) / denominator;

        CFfloat coordOnVec    = ( ( m_4[i2][0] - m_4[i][0] ) * (quadCenter[1]-m_4[i][1]) - ( m_4[i2][1] - m_4[i][1] ) * (quadCenter[0]-m_4[i][0]) ) / denominator;

        if( coordOnQuadEdge >= 0.0 && coordOnQuadEdge <= 1.0 && coordOnVec > 0.0 && coordOnVec < 1.2 ) {

            m_center = quadCenter + vec * 0.8 * coordOnVec;
            break;
        }
    }
}


const CFpoint CFbspline::evalCubic_(CFfloat T) const
{
    unsigned int start = static_cast<unsigned int>(T)-3;
    CFfloat val = fmod(T-start,m_points.size());

    CFpoint c = m_points[start++] * (1/6.0) * (4-val) * (4-val) * (4-val);
    val--;
    c += m_points[start++] * (-22/3.0 + 10 * val - 4 * val * val + 0.5 * val * val * val);
    val--;
    c += m_points[start++] * (1/6.0) * (4 - 12 * val + 12 * val * val - 3 * val * val * val);
    val--;
    c += m_points[start] * (1/6.0) * val * val * val;

    return c;
}


CFpoint CFbspline::evalIntersect( CFpoint p, CFfloat precision ) const {

  CFpoint vec = p - m_center;

  // First, find in which quarter there is an intersection
  CFfloat low  = 0.75*(getNbPoints()-3)+3;
  CFfloat high = getNbPoints();
  for( unsigned int i = 0; i < 3; i++ ) {

    CFfloat denominator = ( m_4[i+1][1] - m_4[i][1] ) * vec[0] - ( m_4[i+1][0] - m_4[i][0] ) * vec[1];
    if( denominator == 0 ) continue;

    CFfloat coordOnQuadEdge = ( ( vec[0] * (m_center[1]-m_4[i][1]) ) - ( vec[1] * (m_center[0]-m_4[i][0]) ) ) / denominator;

    CFfloat coordOnVec    = ( ( m_4[i+1][0] - m_4[i][0] ) * (m_center[1]-m_4[i][1]) - ( m_4[i+1][1] - m_4[i][1] ) * (m_center[0]-m_4[i][0]) ) / denominator;

    if( coordOnQuadEdge >= 0.0 && coordOnQuadEdge <= 1.0 && coordOnVec >= 0.0 ) {

      low = (i/4.0)*(getNbPoints()-3)+3;
      high = ((i+1)/4.0)*(getNbPoints()-3)+3;
      break;
    }
  }

  // Then do a binary search to get as close as possible to the intersection
  CFfloat mid;
  CFfloat cross;
  CFpoint cp;
  while( high-low > precision ) {

      mid = (low + high) / 2.0;
      cp = evalCubic_(mid);
      cross = vec[0] * (cp[1]-m_center[1]) - vec[1] * (cp[0]-m_center[0]);
      if( cross > 0 )
        high = mid;
      else if( cross < 0 )
        low = mid;
      else
        break; // found
   }

   return cp;
}


void CFbspline::draw( bool drawCtrlPts ) const {

        // min = curve_order-1
        // max = m_points.size()
        float step = (m_points.size()-3)/25.0;
        glBegin(GL_LINE_LOOP);
        for( float u = 3.0; u < m_points.size(); u += step ) {

          glColor3f(1.0, 1.0, 1.0);
          evalCubic_(u).Draw();
        }
        glEnd();

        if( !drawCtrlPts ) return;

        // Draw quarters 
        glColor3f( 1.f, 0.3f, 0.3f );
        glBegin(GL_LINES);
            m_center.Draw(); m_4[0].Draw();
            m_center.Draw(); m_4[1].Draw();
            m_center.Draw(); m_4[2].Draw();
            m_center.Draw(); m_4[3].Draw();
        glEnd();

        CFpoint quadCenter = ( m_4[0] + m_4[2] )/2.0;
        glPointSize(3.0);
        glBegin(GL_POINTS);

            glColor3f(1.0, 0.0, 1.0); quadCenter.Draw();
            glColor3f(1.0, 1.0, 0.0); m_center.Draw();  // Draw center
            glColor3f(0.3f, 1.f, 0.3f);
            for( unsigned int i = 0; i < m_points.size(); i++ ) {

                CFpoint p = m_points[i];
                p.Draw();
            }
        glEnd();
}









