
#include "jet.h"
#include "basicGeo.h"

BasicGeo basicGeo;
static GLint numOfJet = 4;

Jet::Jet()
{
	matSil = new materialClass();
	matSil->setMaterialSilver();

	matRedPlastic = new materialClass();
	matRedPlastic->setMaterialRedPlastic();
	//gluQuadricDrawStyle(quadric1, GLU_LINE); //wireframe mode


}

Jet::~Jet()
{
	delete matSil;

}



int Jet::render(GLfloat jetTakeOff)
{
	

	glPushMatrix();
	glTranslatef(10, 0, 20);
	if (jetTakeOff < numOfJet) {
		glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		drawJet();

		for (int i = 1; i <= numOfJet-1 - jetTakeOff; i++) {
			glTranslatef(-15, 0, 0);
			drawJet();
		}
		glPopMatrix();
	}
	
	glPopMatrix();
	return true;
}

int Jet::renderTakeOff( GLfloat count) {
	
		if (count < 400) {
			glTranslatef(-25, 1, -30+(count/10));
			drawJet();
		}
		else {
			int c = count - 400;
			glTranslatef(-25, 1 + (c / 20), -30 + (count / 10));
			drawJet();
		}

	
	return true;
}


void Jet::drawJet()
{
	matSil->doMaterial();
	glColor3f(0.23137, 0.28235, 0.45882);

	//glRotatef(60, 1, 1, 1);
	glPushMatrix();

		glTranslatef(0, 2, 0);
	
		glPushMatrix();
			drawWing();
			glTranslatef(6, 0, -1);
			glRotatef(20, 0, 1, 0);
			drawWing();
		glPopMatrix();

		glPushMatrix();
			drawHeadnBody();
		glPopMatrix();

		glPushMatrix();
			drawMissle();
			glTranslatef(-5.2, 0, 0);
			drawMissle();
		glPopMatrix();

	glPopMatrix();
	


}




void Jet::drawHeadnBody() {


	//head
	glPushMatrix();
	glScalef(2, 2, 2.5);
	basicGeo.renderCone(0);
	

	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0, 0, -7.0f);
	glScalef(2, 2, 7);
	basicGeo.renderCylinder(20, 5);

	glPopMatrix();



}



void Jet::drawWing() {
	
	glPushMatrix();

	glRotatef(-10, 0.0, 1.0, 0.0);
	glTranslatef(-3, 0, -0.8);
	glScalef(5, 0.2, 1.3);
	basicGeo.renderCube();

	glTranslatef(0,0,-3.5);
	glScalef(0.6, 0.6, 0.6);
	basicGeo.renderCube();


	glPopMatrix();


}


void Jet::drawMissle() {

	glPushMatrix();

	glTranslatef(2, -0.3, -0.5);
	glScalef(0.2, 0.2, 0.3);
	drawHeadnBody();

	glTranslatef(4, 0, 0);
	drawHeadnBody();

	glTranslatef(4, 0, 0);
	drawHeadnBody();

	glPopMatrix();
}

