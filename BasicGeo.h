#ifndef basicGeo_def 
#define basicGeo_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

#include "utility.h"
#include "glUtils.h"
#include "vec4.h"

class BasicGeo
{
protected:
	GLUquadricObj* quadric3;
public:
	BasicGeo(); // constructor
	~BasicGeo();


	void renderCube(); // draw it
	void renderCW(); // draw it CW
	void renderCCW(); // draw it CCW
	void drawTexture(GLuint texHandles);
	void renderTheNormals();

	void renderCone(GLfloat smallEndRadius);
	void renderCylinder(int slices, int stacks);
	void renderDisk(GLfloat innerRadius);
	
};

#endif
