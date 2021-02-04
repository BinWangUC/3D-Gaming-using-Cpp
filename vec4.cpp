#include "vec4.h"



// -----------  vec4 print  ----------------------------------------
void v4Print(const vec4 u, char fformat[], char stringLabel[])
// Print vec4 something like "Displacement vec: [0.0400, 1.0000, 0.6800]" 
// according to given floating point format
{
  char s[200];
  sprintf(s, "%s:  [%%%s, %%%s, %%%s]\n", stringLabel, fformat, fformat, fformat);
  printf(s, u[0], u[1], u[2]);
}

// -----------  vec4 creation  -------------------------------------

void v4FloatToVec(const float a, const float b, const float c, vec4 u)
// Build a vector u from 3 scalars...
{
  u[0]=a; 
  u[1]=b; 
  u[2]=c;
}

void v4Zero(vec4 u)    
// Effectively:  u = 0
{
  u[0]=0.0; 
  u[1]=0.0; 
  u[2]=0.0;
}

void v4Copy(const vec4 u, vec4 v)
// Effectively: v = u
{
  v[0] = u[0]; 
  v[1] = u[1]; 
  v[2] = u[2];
}


// -----------  Arithmetic Operations - In place  ------------------

void v4ChangeSign (vec4 u)
// Change sign of vec: effectively  u = -u
{
  u[0] = -u[0];
  u[1] = -u[1];
  u[2] = -u[2];
}

void v4Scale (const float a, vec4 u)
// In-place scalar mult: effectively  u = a*u
{
  u[0] *= a;
  u[1] *= a;
  u[2] *= a;
}

void v4AddToVec(const vec4 u, vec4 v)
// In-place addition: effectively  v = v + u;
{
  v[0] += u[0]; 
  v[1] += u[1]; 
  v[2] += u[2];
}

void v4SubtractFromVec (const vec4 u, vec4 v)
// In-place subtraction: effectively  v = v - u;
{
  v[0] -= u[0]; 
  v[1] -= u[1]; 
  v[2] -= u[2];
}

void v4Accumulate (const float a, const vec4 u, vec4 v)
// In-place accumulation of linear combination: effectively v = v + a*u
{
  v[0] += a*u[0];  
  v[1] += a*u[1];  
  v[2] += a*u[2];
}

void v4UpdateMin(const vec4 u, vec4 v)
// In-place vec4 min: effectively  v = min(u,v)
{
   if (u[0]<v[0])  v[0] = u[0];
   if (u[1]<v[1])  v[1] = u[1];
   if (u[2]<v[2])  v[2] = u[2];
}

void v4UpdateMax(const vec4 u, vec4 v)
// In-place vec4 max: effectively  v = max(u,v)
{
   if (u[0]>v[0])  v[0] = u[0];
   if (u[1]>v[1])  v[1] = u[1];
   if (u[2]>v[2])  v[2] = u[2];
}

// -----------  Arithmetic Operations - To new vec  ----------------

void v4ScalarMult(const vec4 u, const float a, vec4 v)
// Scalar multiplication: effectively  v = a*u
{
  v[0] = a*u[0];
  v[1] = a*u[1];
  v[2] = a*u[2];
}

void v4Plus(const vec4 u, const vec4 v, vec4 w)
// Vector addition: effectively  w = u + v
{
  w[0] = u[0] + v[0]; 
  w[1] = u[1] + v[1]; 
  w[2] = u[2] + v[2];
}

void v4Minus(const vec4 u, const vec4 v, vec4 w)
// Vector subtraction: effectively  w = u - v
{
  w[0] = u[0] - v[0]; 
  w[1] = u[1] - v[1]; 
  w[2] = u[2] - v[2];
}


void v4ComponentwiseMult(vec4 u, vec4 v, vec4 w)
// Multiply components of vectors u, v to give vector w
{
  w[0] = u[0] * v[0];  
  w[1] = u[1] * v[1];  
  w[2] = u[2] * v[2];
}

// -----------  Geometric Operations  -----------------------------

float v4Length(const vec4 u)
// Find length of a vector...
{
  return (float) sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
}

void v4DisplacementVec(const vec4 P, const vec4 Q, vec4 u)
// Find displacement vector from point P to point Q.
// More or less the same as v4Minus, apart from order of args.
{
  u[0] = Q[0] - P[0]; 
  u[1] = Q[1] - P[1]; 
  u[2] = Q[2] - P[2];
}

float v4Distance(const vec4 P, const vec4 Q)
// Find distance between points P & Q.
// Note: symmetric in args -- order of P, Q doesn't matter.
{
  vec4 u;
  v4DisplacementVec(P, Q, u);
  return v4Length(u);
}

float v4DotProduct(const vec4 u, const vec4 v)
// Find dot product u.v of vectors u, v
{
  return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

void v4CrossProduct(const vec4 u, const vec4 v, vec4 w)
// Find cross product: effectively  w = u X v
{
  w[0] = u[1]*v[2] - u[2]*v[1];  
  w[1] = u[2]*v[0] - u[0]*v[2];  
  w[2] = u[0]*v[1] - u[1]*v[0];
}


void v4Normalise(vec4 u)
{
   // In-place normalisation of a 3-vector: effectively u = u / |u|.
   float len;
   len = v4Length(u);
   if (len != 0.0)
    v4Scale(1.0f / len, u);
}

void v4UnitVector(const vec4 u, vec4 v)
{
   // Create unit vector v in direction of u: effectively  v = u / |u|
   float len;
   len = v4Length(u);
   if (len == 0.0)         // Trap divide-by-zero & return zero vec.
     v4Zero(v);
   else
    v4ScalarMult(u, 1.0f / len, v);
}

void v4PolygonNormal(const vec4 P1, const vec4 P2, const vec4 P3, vec4 normalVec)
{
  // Calculates a unit normal to the plane containing points P1, P2, P3.
  // Points P1, P2, P3 specified in anti-clockwise order.
   vec4 u, v;
   v4DisplacementVec (P1, P2, u);
   v4DisplacementVec (P2, P3, v);
   v4CrossProduct (u, v, normalVec);
   v4Normalise(normalVec);
}
