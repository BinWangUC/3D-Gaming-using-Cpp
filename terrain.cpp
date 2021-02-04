// terrain.cpp

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "vec3.h"
#include "glut.h"
#include "terrain.h"


// -----------------------------------------------------------------------------------

// X22_0112.TXT

//GLfloat xsize;
//GLfloat zsize;
//int xsteps;
//int zsteps;

//materialClass* mat;
//materialClass* matLines;




int terrain::renderNorm(GLfloat(*mapNorm)[102*102][3], GLfloat(*map)[102][102], bool normalOn) {
	glPushMatrix();
	glColor3f(0.1, 0.5, 0.1);
	matGrass->doMaterial();
	GLfloat xdelta =1;
	GLfloat zdelta = 1;
	GLfloat ydelta = 0.009;

	//glNormal3f(0, 1, 0); // yes veronica all normals point up (which is wrong) (students need to fix this)
	int pos;
	for (int x = 0; x < 102 - 1; x++)
		for (int z = 0; z < 102 - 1; z++)
		{
			glBegin(GL_TRIANGLES);
			pos = (x) * 102 + (z + 1);
			vec3 aa = ((*mapNorm)[pos][0], (*mapNorm)[pos][0], (*mapNorm)[pos][0]);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x][z]);
			glVertex3f(xdelta * x, ydelta*(*map)[x][z], zdelta * z);
			
			pos = (x+1) * 102 + (z + 1);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x + 1][z]);
			glVertex3f(xdelta * x + xdelta, ydelta*(*map)[x + 1][z], zdelta * z);

			pos = (x) * 102 + (z+1 + 1);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x][z + 1]);
			glVertex3f(xdelta * x,  ydelta*(*map)[x][z + 1], zdelta * z + zdelta);
			glEnd();

			glBegin(GL_TRIANGLES);
			pos = (x+1) * 102 + (z+1 + 1);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x + 1][z + 1]);
			glVertex3f(xdelta * x + xdelta,  ydelta*(*map)[x + 1][z + 1], zdelta * z + zdelta);

			pos = (x) * 102 + (z+1 + 1);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x][z + 1]);
			glVertex3f(xdelta * x,  ydelta*(*map)[x][z + 1], zdelta * z + zdelta);

			pos = (x+1) * 102 + (z + 1);
			glNormal3f((*mapNorm)[pos][0], (*mapNorm)[pos][1], (*mapNorm)[pos][2]);
			//setMaterialHeight(map[x + 1][z]);
			glVertex3f(xdelta * x + xdelta,  ydelta*(*map)[x + 1][z], zdelta * z);
			glEnd();
		}

	if (normalOn) {
		for (int x = 0; x < 102; x++)
			for (int z = 0; z < 102; z++) {

				vec3 normP1;
				// now add the normal which is a direction treated as a displacement vector
				normP1.x = xdelta * x, +(*mapNorm)[pos][0];
				normP1.y = ydelta * (*map)[x][z] + (*mapNorm)[pos][1];
				normP1.z = zdelta * z + (*mapNorm)[pos][2];

				// display the line for the normal
				glColor3f(0.1, 0.7, 0.18);
				glBegin(GL_LINES);
				glVertex3f(xdelta * x, ydelta * (*map)[x][z], zdelta * z);
				glVertex3f(normP1.x, normP1.y, normP1.z);
				glEnd();
			}
	}
	

	glPopMatrix();
	return true;
}



terrain::terrain()
{
	matGrass = new materialClass();
	matGrass->setMaterialGreenGrass();


}

terrain::~terrain()
{
delete matGrass;

}

// end


