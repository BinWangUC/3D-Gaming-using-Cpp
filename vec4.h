#ifndef _vec4_
#define _vec4_

#include <stdio.h>
#include <math.h>

#define pi 3.14159265359
#define degrees pi/180.0
#define radians 180.0/pi

/* Type definitions...  */
typedef float vec4[3];


// -----------  vec4 print  ----------------------------------------
// Print a vec4.
// If u is the vec4 {0.0400, 1.0000, 0.6800} , then the call
//   v4Print(u, "6.4f", "Displacement vec" 
// should print something like 
//   Displacement vec:  [0.0400, 1.0000, 0.6800]
void v4Print(const vec4 u, char fformat[], char stringLabel[]);


// -----------  vec4 creation  -------------------------------------
// Build a vector u from 3 scalars...
void v4FloatToVec(const float a, const float b, const float c, vec4 u);

// Set u to zero vector...
void v4Zero(vec4 u);

// Copy a vector: effectively  v = u...
void v4Copy(const vec4 u, vec4 v);


// -----------  Arithmetic Operations - In place  ------------------

// Change sign of vec: effectively  u = -u...
void v4ChangeSign (vec4 u);

// In-place scalar mult: effectively  u = a*u...
void v4Scale (const float a, vec4 u);

// In-place addition: effectively  v = v + u...
void v4AddToVec(const vec4 u, vec4 v);

// In-place subtraction: effectively  v = v - u...
void v4SubtractFromVec (const vec4 u, vec4 v);

// In-place accumulation of linear combination: effectively v = v + a*u...
void v4Accumulate (const float a, const vec4 u, vec4 v);

// In-place vec4 min: effectively  v = min(u,v)...
void v4UpdateMin(const vec4 u, vec4 v);

// In-place vec4 max: effectively  v = max(u,v)...
void v4UpdateMax(const vec4 u, vec4 v);


// -----------  Arithmetic Operations - To new vec  ----------------

// Scalar multiplication: effectively  v = a*u...
void v4ScalarMult(const vec4 u, const float a, vec4 v);

// Vector addition: effectively  w = u + v...
void v4Plus(const vec4 u, const vec4 v, vec4 w);

// Vector subtraction: effectively  w = u - v...
void v4Minus(const vec4 u, const vec4 v, vec4 w);

// Multiply components of vectors u, v to give vector w...
void v4ComponentwiseMult(vec4 u, vec4 v, vec4 w);


// -----------  Geometric Operations  -----------------------------

// Find length of a vector...
float v4Length(const vec4 u);

// Find displacement vector from point P to point Q...
void v4DisplacementVec(const vec4 P, const vec4 Q, vec4 u);

// Find distance between points P & Q...
float v4Distance(const vec4 P, const vec4 Q);

// Find dot product u.v of vectors u, v...
float v4DotProduct(const vec4 u, const vec4 v);

// Find cross product: effectively  w = u X v...
void v4CrossProduct(const vec4 u, const vec4 v, vec4 w);

// Normalise vector u: effectively  u = u / |u|...
void v4Normalise(vec4 u);

// Create unit vector v in direction of u: effectively  v = u / |u|...
void v4UnitVector(const vec4 u, vec4 v);

// Calculates unit normal to the plane containing points P1, P2, P3...
void v4PolygonNormal(const vec4 P1, const vec4 P2, const vec4 P3, vec4 normalVec);

#endif  // _vec4_
