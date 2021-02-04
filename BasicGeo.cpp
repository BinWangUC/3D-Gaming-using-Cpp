/****		Cube in points-polygons (polyhedron) form	****/

#include "BasicGeo.h"

static GLfloat Verts[8][3] = {
  { -0.5,  0.5, -0.5 }, /* 0 left top rear */
  {  0.5,  0.5, -0.5 },	/* 1 right top rear */
  {  0.5, -0.5, -0.5 },	/* 2 right bottom rear */
  { -0.5, -0.5, -0.5 },	/* 3 left bottom rear */
  { -0.5,  0.5,  0.5 },	/* 4 left top front */
  {  0.5,  0.5,  0.5 },	/* 5 right top front */
  {  0.5, -0.5,  0.5 },	/* 6 right bottom front */
  { -0.5, -0.5,  0.5 }	/* 7 left bottom front */
};

static GLuint Faces[6][4] = {
  4, 5, 6, 7,	/* front */
  5, 1, 2, 6,	/* right */
  0, 4, 7, 3,	/* left */
  4, 0, 1, 5,	/* top */
  7, 6, 2, 3,	/* bottom */
  1, 0, 3, 2	/* rear */
};

static GLfloat Normals[6][3] = {
  0, 0, 1, 	/* front */
  1, 0, 0, 	/* right */
  -1, 0, 0, 	/* left */
  0, 1, 0, 	/* top */
  0, -1, 0, /* bottom */
  0, 0, -1   /* rear */
};

void copyVertex(GLfloat ToZ[3], GLfloat FromZ[3])
{
	ToZ[0] = FromZ[0];
	ToZ[1] = FromZ[1];
	ToZ[2] = FromZ[2];
}

static GLfloat spareVertex[3];


BasicGeo::BasicGeo()
{
	quadric3 = gluNewQuadric();
}

BasicGeo::~BasicGeo()
{
	gluDeleteQuadric(quadric3);
}

void BasicGeo::renderCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 0; vert < 4; vert ++)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void BasicGeo::renderCCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 3; vert >= 0; vert--)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void  BasicGeo::renderTheNormals()
{
	// this normal renderinmg routine is not general purpose and works only with the cube
	glBegin(GL_LINES);
	//glColor3f(1, 1, 0);
	for (int face = 0; face < 6; face++)
	{
		spareVertex[0] = Normals[face][0] / 2.0f; //x
		spareVertex[1] = Normals[face][1] / 2.0f; //y
		spareVertex[2] = Normals[face][2] / 2.0f; //z
		glVertex3fv(spareVertex);
		glVertex3fv(Normals[face]);
	}
	glEnd();
}
void BasicGeo::renderCube()
{
	//renderCCW();
	glutSolidCube(1);
	//renderTheNormals();
}

void BasicGeo::renderCone(GLfloat smallEndRadius) {
	glPushMatrix();
	gluCylinder(quadric3, 0.5, smallEndRadius, 1, 17, 4);
	glPopMatrix();
}

void BasicGeo::renderCylinder(int slices, int stacks)
{
	gluCylinder(quadric3, 0.5, 0.5, 1, slices, stacks);
}

void BasicGeo::renderDisk(GLfloat innerRadius)
{
	glPushMatrix();
	gluDisk(quadric3, innerRadius, 0.5, 17, 4);
	glPopMatrix();
}


void BasicGeo::drawTexture(GLuint texHandles)
{
	int face;
	GLfloat norm[3];
	GLfloat v0[3], v1[3], v2[3], v3[3]; // four verticies

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texHandles);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (face = 0; face < 6; face++)
	{
		copyVertex(v0, Verts[Faces[face][0]]);
		copyVertex(v1, Verts[Faces[face][1]]);
		copyVertex(v2, Verts[Faces[face][2]]);
		copyVertex(v3, Verts[Faces[face][3]]);

		v4PolygonNormal(v0, v2, v1, norm);

		glBegin(GL_QUADS);
		glNormal3fv(norm);
		glTexCoord2f(1.0, 0.0);

		glVertex3fv(v0);
		glTexCoord2f(1.0, 1.0);

		glVertex3fv(v1);
		glTexCoord2f(0.0, 1.0);

		glVertex3fv(v2);
		glTexCoord2f(0.0, 0.0);

		glVertex3fv(v3);
		glEnd();

		//displayNormal3(v0,v1,v2,norm);
	}

	for (face = 0; face < 6; face++)
	{
		copyVertex(v0, Verts[Faces[face][0]]);
		copyVertex(v1, Verts[Faces[face][1]]);
		copyVertex(v2, Verts[Faces[face][2]]);
		//	  copyVertex(v3, Verts[Faces[face][3]]);
		v4PolygonNormal(v0, v2, v1, norm);
		//displayNormal3(v0, v1, v2, norm);
	}
}