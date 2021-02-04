#ifndef __Terrain_grid_ZZ__
#define __Terrain_grid_ZZ__
// terrain.h 

#include <windows.h>
#include "Lights.h"
#include "glut.h"

class terrain 
{



public:

materialClass* matGrass;


virtual int renderNorm(GLfloat (*mapNorm)[102*102][3], GLfloat(*map)[102][102], bool normalOn);
terrain();
~terrain();

};


#endif
