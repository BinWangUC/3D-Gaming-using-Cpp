#include "airport.h"
#include "BasicGeo.h"
#include "Jet.h"

BasicGeo basicGeo1;



Airport::Airport (){
	matGreenPlastic = new materialClass();
	matGreenPlastic->setMaterialGreenPlastic();

	matBlackRubber = new materialClass();
	matBlackRubber->setMaterialBlackRubber();

	matRedPlastic = new materialClass();
	matRedPlastic->setMaterialRedPlastic();

	matJade = new materialClass();
	matJade->setMaterialJade();

	matYellowPlastic = new materialClass();
	matYellowPlastic->setMaterialYellowPlastic();

	matSilver = new materialClass();
	matSilver->setMaterialSilver();

	matWhiteRubber= new materialClass();
	matWhiteRubber->setMaterialWhiteRubber();

	quadric2 = gluNewQuadric();

	/*
	vec3 rocketVel(0, 0, 0);
	vec3 rocketPos(10, 0, 10);
	vec3 prevRocketPos(10, -0.5, 10);
	GLfloat	rocketAngle = 0;
	//vec3 gravity(0, -0.6, 0);
	vec3 engineThrust(0.3, 2, 0);
	vec3 aim(0, 0, 0);
	bool flying = false;
	int burnCount = 0;
	*/
}

Airport::~Airport() {
	gluDeleteQuadric(quadric2);
}


bool Airport::render(GLfloat spinAngle, lightClass* lightFlare, bool flareSet, bool normalOn, GLuint texHandles) {
	
	drawGround(102, 3, 102);
	glPushMatrix();
	drawRunway();
	glTranslatef(-20, 0, 0);
	drawRunway();
	glPopMatrix();
	windMill(spinAngle);
	glPushMatrix();
	controlTower();
	fountain();
	storage(texHandles);
	glPopMatrix();
	//if (flareSet)flying1 = true;
	if (flareSet){
		flareSet=flare(lightFlare,flareSet);
	}
	return flareSet;
}

void Airport::drawGround(int len, int dep, int widt) {
	matWhiteRubber->doMaterial();
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(0, -dep / 2, 0);

	/*
	for (int i = 1; i <= len; i++) {

		for (int j = 1; j <= widt; j++) {
			basicGeo1.renderCube();
			glTranslatef(1, 0, 0);

		}
		glTranslatef(0, 0, 1);

	}
	*/
	glScalef(len, dep, widt);
	basicGeo1.renderCube();
	glPopMatrix();

}

void Airport::drawRunway() {

	glPushMatrix();
	matYellowPlastic->doMaterial();
	glColor3f(1, 1, 1);
	glTranslatef(-25, 0.5, 45);
	glScalef(0.8, 0.5, 4);
	basicGeo1.renderCube();

	for (int i = 1; i < 12; i++) {
		glTranslatef(0,0,-2);
		basicGeo1.renderCube();
	}
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glScalef(0.8, 0.5, 100);
	glTranslatef(-27, 1, 0);
	basicGeo1.renderCube();
	glTranslatef(-10, 0, 0);
	basicGeo1.renderCube();

	glPopMatrix();
}

void Airport::windMill(GLfloat spinAngle) {
	matSilver->doMaterial();
	glPushMatrix();

	glTranslatef(20, 0, -50);
	glColor3f(0, 0, 0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(2, 2, 10);
	basicGeo1.renderCone(0.1);
	glPopMatrix();
	
	// three blades
	glTranslatef(0, 10, 0);
	glRotatef(spinAngle, 0, 0, 1);
	glPushMatrix();
	oneBlade();
	glRotatef(120, 0, 0, 1);
	oneBlade();
	glRotatef(120, 0, 0, 1);
	oneBlade();
	glPopMatrix();

	glPopMatrix();
}
void Airport::oneBlade() {
	matSilver->doMaterial();
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 0.5, 7);
	basicGeo1.renderCone(0);
	glPopMatrix();
}

void Airport::controlTower() {
	matJade->doMaterial();
	glPushMatrix();
	glTranslatef(30,0,35);
	glColor3f(0.4, 0.4, 0);
	
	glPushMatrix();
	glScalef(10, 25, 10);
	glRotatef(-90, 1, 0, 0);
	basicGeo1.renderCone(0.5);
	glPopMatrix();
	
	//top
	glPushMatrix();
	glTranslatef(0, 35, 0);
	glScalef(15, 15, 15);
	glRotatef(90, 1, 0, 0);
	basicGeo1.renderCone(0.2);
	glPopMatrix();
	
	//trees around the control tower
	for (int i = 0; i <= 9; i++) {


		glPushMatrix();
		glTranslatef(15, 0, 0);
		glScalef(2, 2, 2);
		drawTree();
		glPopMatrix();
		
		glRotatef(36, 0, 1, 0);
	}


	glPopMatrix();
}
void Airport::drawTree() {
	
	glPushMatrix();
	matGreenPlastic->doMaterial();
	glTranslatef(0, 1, 0);
	glPushMatrix();
	glColor3f(0, 0.5, 0);
	glScalef(0.5, 0.5, 0.5);
	basicGeo1.renderCube();
	glPopMatrix();


	glPushMatrix();
	matGreenPlastic->doMaterial();
	glTranslatef(0, 2, 0);
	glRotatef(-90, 1, 0, 0);
	basicGeo1.renderCone(0);

	glTranslatef(0, 0, -1.5);
	glScalef(2, 2, 2);
	basicGeo1.renderCone(0);
	glPopMatrix();

	glPopMatrix();
}

void Airport::storage(GLuint texHandles) {
	matSilver->doMaterial();
	glColor3f(0.82, 0.82, 0.80);

	glTranslatef(30, 0, -30);

	//left and right
	glPushMatrix();
	glTranslatef(0, 6, 0);
	glScalef(20, 12, 2);
	basicGeo1.renderCube();
	glTranslatef(0, 0, 15);
	basicGeo1.renderCube();
	glPopMatrix();
	
	//behind
	glPushMatrix();
	glTranslatef(11,6,15);
	glScalef(2, 12, 32);
	basicGeo1.renderCube();
	glPopMatrix();

	//top
	glPushMatrix();
	glTranslatef(1, 13, 15);
	glScalef(21, 2, 32);
	basicGeo1.drawTexture(texHandles);
	//basicGeo1.renderCube();
	glPopMatrix();
}

bool Airport::flare(lightClass* lightFlare, bool flareSet) {
	//update position-----------------
	
	if (flying ==true) {
		rocketPos.x = 0; rocketPos.y = 0; rocketPos.z = 0;
		rocketVel.x = aim.x; rocketVel.y = aim.y; rocketVel.z = aim.z;
		frameCount = 0;
		burnCount = 1;
		rocketAngle = 0;
		flying = false;
		flareSet = false; }

	frameCount++;
	glColor3f(1, 1, 1);
	if (frameCount%10==0 && flying == false){//&& flying==true
	prevRocketPos = rocketPos;
	rocketVel = rocketVel + gravity;
	if (burnCount <= 5)
	{
		rocketVel = rocketVel + engineThrust;
		burnCount++;
	}
	rocketPos = rocketPos + rocketVel;
	if (rocketPos.y <= 0)
	{
		flying = true;
		flareSet = false;
		rocketPos.y = 0;
	}
	rocketAngle = atan2(rocketPos.x - prevRocketPos.x, rocketPos.y - prevRocketPos.y);
	rocketAngle = (-1) * rocketAngle * 180.0 / 3.1416;

	}
	//--------------------------------



	glPushMatrix();
	glTranslatef(-50, 0.5, -30);
	glTranslatef(rocketPos.x, rocketPos.y, rocketPos.z);
	glRotatef(rocketAngle, 0.0, 0.0, 1.0);
	
	glPushMatrix();
	glScalef(1, 2, 1);
	basicGeo1.renderCube();
	lightFlare->setPos(rocketPos.x - 50, rocketPos.y+0.5, rocketPos.z - 30, 0);
	glPopMatrix();

	glPopMatrix();
	
	return flareSet;
}

void Airport::fountain() {
	matGreenPlastic->doMaterial();
	glPushMatrix();
	glTranslatef(5, 0, 40);
	glColor3f(0.1, 0.4, 0.1);

	//top
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glScalef(10, 1, 10);
	glRotatef(90, 1, 0, 0);
	basicGeo1.renderCone(0.4);
	glPopMatrix();

	glPopMatrix();

}
