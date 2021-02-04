#ifndef AIRPORT_H
#define AIRPORT_H

#include <stdlib.h>
#include "glut.h"
#include "Lights.h"


class Airport
{

public:
	virtual bool render(GLfloat spinAnble, lightClass* lightFlare, bool flareSet, bool normalOn, GLuint texHandles);
	materialClass* matGreenPlastic;
	materialClass* matBlackRubber;
	materialClass* matRedPlastic;
	materialClass* matJade;
	materialClass* matYellowPlastic;
	materialClass* matSilver;
	materialClass* matWhiteRubber;

	Airport();
	~Airport();

protected:
	GLUquadricObj* quadric2;


	void drawGround(int len, int widt, int dep);
	void drawRunway();
	void windMill(GLfloat spinAngle);
	void oneBlade();
	void controlTower();
	void drawTree();
	void storage(GLuint texHandles);
	void fountain();

	bool flare(lightClass* lightFlare, bool flareSet);

private:
	vec3 rocketVel=vec3(0, 0, 0);
	vec3 rocketPos = vec3 (0, 0, 0);
	vec3 prevRocketPos=vec3(10, -0.5, 10);;
	GLfloat	rocketAngle=0;

	int burnCount=1;
	int frameCount = 1;
	vec3 gravity= vec3(0, -0.67, 0);
	vec3 engineThrust= vec3 (0.8, 2, 0);
	vec3 aim=vec3(0, 0, 0);
	bool flying=true;


};

#endif