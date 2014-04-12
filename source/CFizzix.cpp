#include "../include/CFizzix.h"

  CFizzix* CFizzix::pinstance = 0;// initialize pointer

  CFizzix* CFizzix::Instance () 
  {
    if (pinstance == 0)  // is it the first call?
    {  
      pinstance = new CFizzix; // create sole instance
    }
    return pinstance; // address of sole instance
  }

  CFizzix::CFizzix() 
  { 
    //... perform necessary instance initializations 
  }

  /**************************************************************************
    returns true if two circles are overlapping
    m_collision =  pFizzix->IsCircleCollision(m_object1, m_object2);
  **************************************************************************/
  bool CFizzix::IsCircleCollision(const CFIZZIX_CIRCLE& obj1, const CFIZZIX_CIRCLE& obj2){
    float distance, minDistance;
    distance = sqrt((obj2.point.x - obj1.point.x) * (obj2.point.x - obj1.point.x) + (obj2.point.y - obj1.point.y) * (obj2.point.y - obj1.point.y));
    minDistance = obj1.radius + obj2.radius;

    if(distance <= minDistance)
      return true;
    else
      return false;
  }

  /**************************************************************************
    returns true if two circles are overlapping
    returns reference to x and y values needed for collision response
    m_collision =  pFizzix->IsCircleCollision(m_object1, m_object2);
  **************************************************************************/
  bool CFizzix::IsCircleCollision(const CFIZZIX_CIRCLE& obj1, const CFIZZIX_CIRCLE& obj2, float& x, float& y){
    float distance, minDistance;

    //calculate distance to move to clear collision
    distance = sqrt((obj2.point.x - obj1.point.x) * (obj2.point.x - obj1.point.x) + (obj2.point.y - obj1.point.y) * (obj2.point.y - obj1.point.y));    
    minDistance = obj1.radius + obj2.radius;
    float diff = minDistance - distance;    
    x = 0;
    y = y;

    if(diff < 0)
      return false;

    //angle in radians
    float dx, dy, angle;
    dy = obj2.point.y - obj1.point.y;
    dx = obj2.point.x - obj1.point.x;
    if(dx == 0)
      angle = atan(dy/0.0001f);
    else
      angle = atan(dy/dx);

    if(obj2.point.y <= obj1.point.y){
      x = sin(angle) * diff;
      y = cos(angle) * diff;
    }
    else{
      x = -sin(angle) * diff;
      y = -cos(angle) * diff;
    }

    if(diff >= 0)
      return true;
    else
      return false;
  }

  /********************************************************************************
    Determines if point is inside polygon (up to 10 points)
  ********************************************************************************/
 bool CFizzix::IsPolygonCollision(const CFIZZIX_POLYGON& poly, const CFIZZIX_POINT& pt, float rad[4]){
    
    float angle = 0;
    float totalAngle = 0;

    for(int i = 0; i < poly.numPoints; ++i){
      if(i < poly.numPoints - 1)
        angle = CalcVecAngle(poly.point[i], pt, poly.point[i+1]);
      else
        angle = CalcVecAngle(poly.point[i], pt, poly.point[0]);
      totalAngle += angle;
      
      if(i == 0)
        rad[0] = angle;
      else if(i == 1)
        rad[1] = angle;
      else if(i == 2)
        rad[2] = angle;
      else if(i == 3)
        rad[3] = angle;


    }
    //rad = totalAngle;

    if(totalAngle > 6.25 && totalAngle < 6.31) //triangle = 5.0
      return true;
    else
      return false;
  }

  /********************************************************************************
    Find the point on a line closest to another point. This point is at it's
    closest point of approach.
    pt1 and pt2 define line
    pt3 defines point in space
    Thanks to http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
  ********************************************************************************/
  CFIZZIX_POINT CFizzix::CalcPointOnLine(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3){
  //CFIZZIX_POINT CFizzix::CalcPointOnLine(CFIZZIX_POINT pt1, CFIZZIX_POINT pt2, CFIZZIX_POINT pt3){
    float u = (pt3.x - pt1.x) * (pt2.x - pt1.x) + (pt3.y - pt1.y) * (pt2.y - pt1.y);
    CFIZZIX_POINT pt = CalcPointDiff(pt2, pt1);
    float len = CalcLength(pt);
    u = u / (len * len);
    CFIZZIX_POINT ret;
    ret.x = pt1.x + u * (pt2.x - pt1.x);
    ret.y = pt1.y + u * (pt2.y - pt1.y);
    return ret;
  }

  /********************************************************************************
    Find the distance to the line from a point that is perpendicular to the line.
    pt1 and pt2 define line
    pt3 defines point in space
    pt is the point on the line
  ********************************************************************************/
  float CFizzix::CalcDistanceToLine(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3){
    CFIZZIX_POINT pt = CalcPointOnLine(pt1, pt2, pt3);
    CFIZZIX_POINT pt4 = CalcPointDiff(pt, pt3);
    return CalcLength(pt4);
  }

  /********************************************************************************
    Returns a point (x and y) that a pt must be moved to move it outside a
    polygon once the point has collided with the polygon. The key is to find the
    side with the shortest distance. NOTE: Call this function only if the point
    is inside the polygon. IsPolygonCollision(...)
  ********************************************************************************/
  CFIZZIX_POINT CFizzix::CalcCirclePolygonMove(const CFIZZIX_POLYGON& poly, const CFIZZIX_POINT& pt){
    CFIZZIX_POINT ptMove;
    ptMove.x = 0;
    ptMove.y = 0;
    float distance = 0, minDistance = 1000;

    //look for shortest distance
    for(int i = 0; i < poly.numPoints; ++i){
      if(i < poly.numPoints - 1){
        distance = CalcDistanceToLine(poly.point[i], poly.point[i + 1], pt);
        if(distance < minDistance){
          minDistance = distance;
          ptMove = CalcPointOnLine(poly.point[i], poly.point[i + 1], pt);
        }
      }
      else{
        distance = CalcDistanceToLine(poly.point[poly.numPoints - 1], poly.point[0], pt);
        if(distance < minDistance){
          minDistance = distance;
          ptMove = CalcPointOnLine(poly.point[poly.numPoints - 1], poly.point[0], pt);
        }
      }
    }

    return CalcPointDiff(ptMove, pt);
  }

  //*************************************************************************************************
  //HELPER FUNCTIONS
  //*************************************************************************************************
  
  //calculates length (hypotenuse)
  float CFizzix::CalcLength(CFIZZIX_POINT pt){
    return sqrt ((pt.x * pt.x + pt.y * pt.y));   
  }
  float CFizzix::CalcLength(float x, float y){
    return sqrt ((x * x + y * y));   
  }

  //subtracts pt2 - pt1
  CFIZZIX_POINT CFizzix::CalcPointDiff(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2){
    CFIZZIX_POINT result;
    result.x = pt2.x - pt1.x;
    result.y = pt2.y - pt1.y;
    return result;
  }

  //calculate DOT product
  float CFizzix::CalcVecDotProduct(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2){
    return (pt1.x * pt2.x + pt1.y * pt2.y);
  }
 
  //find angle between two vectors
  float CFizzix::CalcVecAngle(const CFIZZIX_POINT& pt1, const CFIZZIX_POINT& pt2, const CFIZZIX_POINT& pt3){
    CFIZZIX_POINT vec1 = CalcPointDiff(pt2, pt1);
    CFIZZIX_POINT vec2 = CalcPointDiff(pt2, pt3);
    float lenA = CalcLength(vec1);
    float lenB = CalcLength(vec2);
    return acos(CalcVecDotProduct(vec1, vec2) / (lenA * lenB));    
  }

  //calc angle between two sides
  float CFizzix::CalcAngle(float x1, float y1, float x2, float y2){
    if(x2 - x1 == 0)
      return static_cast<float>(atan((y2 - y1)/0.001));
    else
      return static_cast<float>(atan((y2 - y1)/(x2 - x1)));
  }

  //calc angle between two sides
  float CFizzix::CalcAngle(float dx, float dy){
    if(dx == 0)
      return static_cast<float>(atan((dy)/0.001));
    else
      return static_cast<float>(atan((dy)/(dx)));
  }


//calc angle measured on graph from +X
float CFizzix::CalcGraphAngle(float x1, float y1, float x2, float y2){
  float angle = CalcAngle(x1, y1, x2, y2);

  if(x2 > x1 && y2 > y1)  //quad I
    angle = angle;
  else if(x2 > x1 && y1 == y2) //+x axis
    angle = 0;
  else if(x2 > x1 && y2 < y1) //quad IV
    angle = TWO_PI - angle;
  else if(x2 < x1 && y2 > y1) //quad II
    angle = PI/2  + angle;
  else if(x2 < x1 && y2 == y1) //-x axis
    angle = PI;
  else if(x2 < x1 && y2 < y1) //quad III
    angle = PI + angle;
  else if(x2 == x1 && y2 > y1)//+y axis
    angle = PI / 2;
  else if(x2 == x1 && y2 < y1)//-y axis
    angle = 3 * PI / 2;

  return angle;
}

float CFizzix::CalcGraphAngle2(float x1, float y1, float x2, float y2){
  float angle = CalcAngle(x1, y1, x2, y2);

  if(x2 > x1 && y2 > y1)  //quad I
    angle = angle;
  else if(x2 > x1 && y1 == y2) //+x axis
    angle = 0;
  else if(x2 > x1 && y2 < y1) //quad IV
    angle = TWO_PI + angle;
  else if(x2 < x1 && y2 > y1) //quad II
    angle = angle + PI;
  else if(x2 < x1 && y2 == y1) //-x axis
    angle = PI;
  else if(x2 < x1 && y2 < y1) //quad III
    angle = PI + angle;
  else if(x2 == x1 && y2 > y1)//+y axis
    angle = PI / 2;
  else if(x2 == x1 && y2 < y1)//-y axis
    angle = 3 * PI / 2;

  return angle;
}

float CFizzix::CalcGraphAngle2(float dx, float dy){
  float angle = CalcAngle(dx, dy);

  if(dx > 0 && dy > 0)
    angle = angle;
  else if(dx > 0 && dy == 0)
    angle = 0;
  else if(dx > 0 && dy < 0)
    angle = TWO_PI + angle;
  else if(dx < 0 && dy > 0)
    angle = angle + PI;
  else if(dx > 0 && dy == 0)
    angle = PI;
  else if(dx < 0 && dy < 0)
    angle = PI + angle;
  else if(dx == 0 && dy > 0)
    angle = PI / 2;
  else if(dx == 0 && dy < 0)
    angle = 3 * PI / 2;

  return angle;
}



float CFizzix::CalcDistance(float x1, float y1, float x2, float y2){
  float dx = x2 - x1;
  float dy = y2 - y1;
  return sqrt (dx * dx + dy * dy);   
}

double CFizzix::GetAngleBetweenAngles(float x1, float y1, float x2, float y2)
{
   float dtheta,theta1,theta2;

   theta1 = atan2(y1,x1);
   theta2 = atan2(y2,x2);
   dtheta = theta2 - theta1;
   while (dtheta > PI)
      dtheta -= TWO_PI;
   while (dtheta < -PI)
      dtheta += TWO_PI;

   return(dtheta);
}
