/*
 CFizzix is a physic class to manage all aspects of physics collision,
 reaction, etc.

 Singleton example from...
 http://www.inquiry.com/techtips/cpp_pro/10min/10min0200.asp

 Add d3d9.lib d3dx9.lib  to Linker Command Line
   
 CFizzix *pFizzix = CFizzix::Instance();

*/

#ifndef CFIZZIX_H
#define CFIZZIX_H

#include <d3d9.h>
#include <d3dx9.h>
#include <cmath>

const float PI = 3.14159f;
const float TWO_PI = 6.283185f;

//structures to assist in collisions
struct CFIZZIX_POINT{
  float x, y;
};
struct CFIZZIX_CIRCLE{
  CFIZZIX_POINT point;
  float radius;
  int red, green, blue;
};
struct CFIZZIX_POLYGON{
  CFIZZIX_POINT point[10];
  int numPoints;
};

class CFizzix 
  {
  public:
      static CFizzix* Instance();
      bool IsCircleCollision(const CFIZZIX_CIRCLE& obj1, const CFIZZIX_CIRCLE& obj2);
      bool IsCircleCollision(const CFIZZIX_CIRCLE& obj1, const CFIZZIX_CIRCLE& obj2, float& x, float& y);
      bool IsPolygonCollision(const CFIZZIX_POLYGON& poly, const CFIZZIX_POINT& pt, float rad[4]);
      CFIZZIX_POINT CalcPointOnLine(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3);
      float CalcDistanceToLine(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3);
      CFIZZIX_POINT CalcCirclePolygonMove(const CFIZZIX_POLYGON& poly, const CFIZZIX_POINT& pt);
      float CalcLength(float x, float y);
      float CalcLength(CFIZZIX_POINT pt);
      CFIZZIX_POINT CalcPointDiff(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2);
      float CalcVecDotProduct(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2);
      float CalcVecAngle(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3);
      float CalcAngle(float x1, float y1, float x2, float y2);
      float CalcAngle(float dx, float dy);
      float CalcGraphAngle(float x1, float y1, float x2, float y2);
      float CalcGraphAngle2(float x1, float y1, float x2, float y2);
      float CalcGraphAngle2(float dx, float dy);
      float CalcDistance(float x1, float y1, float x2, float y2);
      double GetAngleBetweenAngles(float x1, float y1, float x2, float y2);

  protected:
      CFizzix();
      CFizzix(const CFizzix&);
      CFizzix& operator= (const CFizzix&);

  private:
      static CFizzix* pinstance;
  };


#endif