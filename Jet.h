#ifndef __JetA__
#define __JetA__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Lights.h"

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "utility.h"
#include "glUtils.h"

class Jet 
{
protected:



void drawJet();
void drawHeadnBody();
void drawMissle();
void drawWing();



public:
materialClass* matSil;
materialClass* matRedPlastic;

virtual int render(GLfloat countJet);
virtual int renderTakeOff( GLfloat count);

Jet();
~Jet();

};

#endif
