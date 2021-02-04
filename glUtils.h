
#ifndef _glUtils_
#define _glUtils_

#include "utility.h"

typedef GLfloat RGBA1[4];
typedef GLfloat Vec3f[3];
typedef GLfloat Vec4f[4];

extern RGBA1 White;
extern RGBA1 Black;

/* Alpha is set to 1 by default */
extern void SetColor (RGBA1 color, GLfloat r, GLfloat g, GLfloat b);
extern void CopyColor (RGBA1 dest, RGBA1 source);

extern void CheckGL (void);

extern void BitmapString (void * font, float x, float y, String msg);

extern float Min (float f, float limit);
extern float Max (float f, float limit);
extern float Clamp (float f, float min, float max);

#endif
