#ifndef __LIGHTS_Z__
#define __LIGHTS_Z__

#include "renderable.h"

class materialClass
{
public:

float	dif[4];  //  = {0.78, 0.57, 0.11, 1.0}; // brass
float	amb[4];  //  = {0.33, 0.22, 0.027, 1.0};
float	spe[4];  //  = {0.99, 0.94, 0.8, 1.0};
float	eme[4];  //  = {0.78, 0.57, 0.11, 1.0}; 

int shiny;

void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setEmissive(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setShinyInt(int shinyZ);
void setShinyFloat(GLfloat shinyZ);

virtual void doMaterial();
materialClass();

void setMaterialJade();
void setMaterialRedPlastic();
void setMaterialWhiteRubber();
void setMaterialBlackRubber();
void setMaterialYellowPlastic();
void setMaterialGreenPlastic();
void setMaterialGreenGrass();
void setMaterialSilver();


};

class lightClass : public renderableParent
{	
public:
enum lightKind {normalL, spotL}; 

protected:
	GLUquadricObj *quadric1;
	//GLfloat red,green,blue;
	//GLfloat position[4];
	
	GLfloat	lpos[4];
    GLfloat	ldif[4];    
    GLfloat	lamb[4];    
    GLfloat	lspe[4]; 

	GLfloat lmodel_ambient[4]; // global Ambient
    GLfloat spotLightDir[3];//  
	GLfloat spotAngle;

	bool showAsSphere; // true or false
	GLfloat radius;
	lightKind kind;
	void init();
	int lightID;

	materialClass * mat;

public:

//void setColour(GLfloat r, GLfloat g, GLfloat b);
void setRadius(GLfloat radius);
void setKind(lightKind k);
void setLightId(int id); // eg GL_LIGHT0, GL_LIGHT1
void setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setGlobalAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void setSpotDir(GLfloat x, GLfloat y, GLfloat z);
void setSpotAngle(GLfloat a);

void letThereBeLight(); // put the light into the scenevoid 
void letThereBeDark(); // put the light into the scene
virtual int render(); // renders a sophere where the light is for debugging

lightClass();
~lightClass();

};

#endif
