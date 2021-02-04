#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Lights.h"
#include <string>
#include "Jet.h"
#include "Shapes.h"
#include "airport.h"
#include "grid.h"
#include "Bitmap.h"
#include "fileoperations3.h"
#include "terrain.h"
#include "vec5.h"
#include"Psystem.h"

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame = false;
bool jetTakeOff = false;
bool sunLightOn = false;
bool aimbientLightOn = false;
bool lightOn = false;
bool flareLightOn = false;
bool normalOn = false;
static GLfloat mapNor[102 * 102][3];


#define ESC	  27
#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

#define N_TEXTURES 2
char bmpFileNames[N_TEXTURES][256] = {
	"F:\\MITS_UC\\GraphicsVisualisationTechniques\\MarkTutorial\\MT2\\AirportLight\\mossyTexture.bmp",
	"F:\\MITS_UC\\GraphicsVisualisationTechniques\\MarkTutorial\\MT2\\AirportLight\\starTexture3.bmp"
};
GLuint texHandles[N_TEXTURES];

extern GLfloat heightField[102][102];
extern int heightField_width;
extern int heightField_depth;

//------------------------------------------------------------------------
static int	WinWidth, WinHeight;
static int	AppMenu;
//static GLfloat	Spin;
static GLfloat	SpinAngle;
static GLfloat cnt;
static GLfloat cntJet = 0;

static GLfloat	eyePoint[3];
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;

static GLfloat	color3[4];

Jet* jet;
Airport airport;
lightClass* lightSun;
lightClass* lightFlare;
terrain ter;

int showWireFrame = false;
GLUquadric* qobj = NULL; // this should really be declared first in 'C'


void initPsystemToDefault(PsystemStruct* ps, int num)
{
	ps->active = false;
	ps->activeticks = -1;
	ps->typeOfPsystem = 0;       // future expansion
	ps->texHandle = 0;           // sane value
	ps->billboard = false;       // sane value
	v3FloatToVecM(ps->pos, num % 4, 0, num / 4);
	ps->emissionRate = 1;      // number of particles per tick (can be fractional)
	ps->emmisionRateRandom = 0;  // a max random number which we div by 100 and add to emision rate
	ps->residualEmmision = 0;
	ps->maxParticles = 100;      // no more than this please
	ps->numParticles = 0;        // the actual number 
	ps->particleSize = 3;        // size in points 
	ps->particleSizeDelta = 0;   // no change
	ps->max_ticks = 120;         // after this many ticks die
	v3FloatToVec(-10000, 0, -10000, ps->dieIfLess);      // kill the particl if its less than any of these values
	v4FloatToVecM(ps->colour, 0.1, 0.8, 0.1, 1); // particle color       
	v4FloatToVecM(ps->colourChange, 0.06, 0.06, 0.06, 0);   // colour drift + added each tick
	v4FloatToVecM(ps->colourMax, 1, 1, 1, 1);
	v3FloatToVec(0, -0.02, 0, ps->gravity);    // velosity change due to gravity
	v3FloatToVec(0, 0, 0, ps->wind);           // movement due to wind
	v3FloatToVec(-0.025, 0.3, -0.025, ps->initialVelosity);
	v3FloatToVec(5, 5, 5, ps->initialVelosityRandom); // max random number which we dive by 100 and add to initial velosity 
	v3FloatToVec(0, 0, 0, ps->airResistance);       // how it slows
	v3FloatToVec(5, 5, 5, ps->absTerminalVelosity);    // max speed

	for (int i = 0; i < MAX_PSYSTEM; i++)
	{
		ps->p[i].active = false;   // active Y or n
		ps->p[i].time = 0;      // ticks of activity
		v3FloatToVec(0, 0, 0, ps->p[i].pos);      // position
		v3FloatToVec(0, 0, 0, ps->p[i].velosity); // how fast do i move
	}
}

void initLightTexture()
{

	lightSun = new lightClass();
	//lightSun->setKind(lightClass::spotL);
	//lightSun->setSpotDir(0,0,-1);
	lightSun->setLightId(GL_LIGHT1);
	lightSun->setAmbient(0.35f, 0.35f, 0.35f, 1.0f); // just a bit of ambient white
	lightSun->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f); // fair bit of diffuse white
	lightSun->setSpecular(1.0f, 1.0f, 1.0f, 1.0f); // not much shiny in this light
	//lightSun->setGlobalAmbient(1, 1, 1, 1);
	//lightSun->setPos(1,7,7,0);
	lightSun->setPos(0.0f, 20.0f, 10.0f, 1.0f);


	lightFlare = new lightClass();
	//lightFlare->setKind(lightClass::spotL);
	//lightFlare->setSpotDir(0,0,-1);
	lightFlare->setLightId(GL_LIGHT0);
	lightFlare->setAmbient(0.1f, 0.1f, 0.2f, 1.0f); // just a bit of ambient white
	lightFlare->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f); // fair bit of diffuse white
	lightFlare->setSpecular(1.0f, 1.0f, 1.0f, 1.0f); // not much shiny in this light

}


static void setProjection()
{
	GLfloat aspect;
	/* Set projection Mode */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspect = (float)WinWidth / (float)WinHeight;
	gluPerspective(fov, aspect, nearPlane, farPlane);
	/* Back to normal */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  //??
}

static void seteyePoint()
{
	gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
		lookAtPoint[0], lookAtPoint[1], lookAtPoint[2],
		upVec[0], upVec[1], upVec[2]);
}

static void SetUp3D()
{
	setProjection();
	seteyePoint();

	/* Sky black background */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearDepth(farPlane);
	glEnable(GL_DEPTH_TEST);

	
	glEnable(GL_NORMALIZE);

	glShadeModel(GL_SMOOTH);
}

static void drawWorld()
{
	glEnable(GL_DEPTH_TEST);
	if (lightOn) { glEnable(GL_LIGHTING); }
	else glDisable(GL_LIGHTING);

	if (sunLightOn) { lightSun->letThereBeLight(); }
	else { glDisable(GL_LIGHT1); }
	
	if (flareLightOn) {
		lightFlare->letThereBeLight();
		lightFlare->render();
	}
	else { 
		glDisable(GL_LIGHT0); 
	}
	
	if (aimbientLightOn) lightSun->setAmbient(0.35f, 0.35f, 0.35f, 1.0f);
	else lightSun->setAmbient(0.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	flareLightOn=airport.render(SpinAngle,lightFlare, flareLightOn, normalOn, texHandles[1]);
	glPopMatrix();


	glPushMatrix();

	for (int i = 1; i < 5; i++) {

		glRotatef(90, 0, 1, 0);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(-51, 10, 51);
		ter.renderNorm(&mapNor, &heightField, normalOn);
		glPopMatrix();
	}

	for (int i = 1; i < 5; i++) {
		glRotatef(90, 0, 1, 0);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(-51 - 102, 10, 51);
		ter.renderNorm(&mapNor, &heightField, normalOn);
		glPopMatrix();
	}

	glPopMatrix();


	glPushMatrix();
	jet->render(cntJet);
	if (jetTakeOff) {
		jet->renderTakeOff(cnt);
	}
	glPopMatrix();

	/* Wireframe mode */
	if (showWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void CheckGL()
{
	int  err;
	char msg[256];

	err = glGetError();
	if (err != GL_NO_ERROR) {
		sprintf_s(msg, "OpenGL error: %s", (String)gluErrorString(err));
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}



void showBMPInfo(CBitmap& bmp)
{
	printf("BMP data loaded:\n");
	printf("  Width: %d  \n", bmp.GetWidth());
	printf("  Height: %d \n", bmp.GetHeight());
	printf("  Bits per pixel: %d \n", bmp.GetBitCount());
}

void LoadBothTextures()
{
	// Read in a couple of BMPs and attach them to texture handles.
	// change the N_TEXTURES constant and bmpFileNames array for more

	glGenTextures(N_TEXTURES, texHandles); // Get some texture handles.
	for (int j = 0; j < N_TEXTURES; j++)
	{
		CBitmap bmpData(bmpFileNames[j]);  // Construct to hold BMP file data
		int texWidth = bmpData.GetWidth();
		int texHeight = bmpData.GetHeight();
		if (texWidth == 0 || texHeight == 0 || bmpData.GetBits() == NULL)
		{
			char response;
			printf("Unable to load BMP file %s \n", bmpFileNames[j]);
			printf("Press 'q' <ENTER> to exit: \n");
			char c = getchar();
			exit(1);
		}
		// OK, looks like we got BMP data.
		printf("Loaded BMP file %s \n ", bmpFileNames[j]);
		showBMPInfo(bmpData);
		// The file read has arranged it to be RGBA
		// (whereas 24-bit BMPs are natively in BGR).
		// The A (transparency) byte is probably worthless, but who cares?
		// Now attach it to OpenGL texture...
		glBindTexture(GL_TEXTURE_2D, texHandles[j]);
		printf("File num %d Attaching pixel data from %s to texture handle %d \n ", j, bmpFileNames[j], texHandles[j]);
		int returnCode = gluBuild2DMipmaps(
			GL_TEXTURE_2D,       // target type of texture
			GL_RGBA,              // format in which GL should store it
			texWidth, texHeight, // [obvious]
			GL_RGBA,             // format of data
			GL_UNSIGNED_BYTE,    // each R,G,B,A component is a byte
			bmpData.GetBits()    // pointer to location of pixel data
		);
		if (returnCode != 0)
			printf("Texture error --%d  %s\n", returnCode, gluErrorString(returnCode));
		// Alternative -- would NOT generate mipmaps.
		 //glTexImage2D(
		//    GL_TEXTURE_2D,       // target type of texture
		//    0,                   // mipmap level
		//    GL_RGBA,             // format in which GL should store texels
		//    texWidth, texHeight, // [obvious]
		//    0,                   // border width
		//    GL_RGBA,             // format of data
		//    GL_UNSIGNED_BYTE,    // each R,G,B component is a byte
		//    bmpData.GetBits()    // pointer to pixel data
		//  );
	}  // End of loop for texture file reading.
	// As the bmpData goes out of scope it will be destroyed.
	// But the pixels have been copied into OpenGL's memory area,
	// so that is okay.

}




static void display()
{
	SetUp3D();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	drawWorld();
	 drawPsystem(1);  // This is updated in the idleFunc & drawn here
	/* Check everything OK and update screen */
	CheckGL();
	glutSwapBuffers();
}


static void resize(int width, int height)
{
	/* Save for event handlers */
	WinWidth = width;
	WinHeight = height;

	/* Reset view in window. */
	glViewport(0, 0, WinWidth, WinHeight);
}


/****		User events		****/
static void menuChoice(int item)
{
	switch (item) {
	case cmdRed:
		color3[0] = 1.0f; // R
		color3[1] = 0.0f; // G
		color3[2] = 0.0f; // B
		color3[3] = 1.0f; // B
		break;
	case cmdGreen:
		color3[0] = 0.0f; // R
		color3[1] = 1.0f; // G
		color3[2] = 0.0f; // B
		color3[3] = 1.0f; // B
		break;
	case cmdExit:
		exit(0);
		break;
	default:
		break;
	}
}

static void asciiKey(unsigned char key, int x, int y)
{
	if (key == ESC)
		menuChoice(cmdExit);

	//if (key == 'a') showAxes = !showAxes;
	else if (key == 'w') showWireFrame = !showWireFrame;
	else if (key == 'b')          // 'b' moves us 'back'
		eyePoint[2] += 2;
	else if (key == 'f')         // 'f' moves us 'forwards'
		eyePoint[2] -= 2;
	else if (key == 'u')         // 'u' moves us 'up'
		eyePoint[1] += 2;
	else if (key == 'd')         // 'd' moves us 'down'
		eyePoint[1] -= 2;
	else if (key == 'l')         // 'l' moves us 'left'
		eyePoint[0] += 2;
	else if (key == 'r')         // 'r' moves us 'right'
		eyePoint[0] -= 2;
	else if (key == ' ')         // 'space' set jet to take off
	{
		jetTakeOff = true; cnt = 0; cntJet++;
	}
	else if (key == 'n' || key == 'N') normalOn = !normalOn; // this only shows the norm for height. Instead of show the triangles' 
															 // norm, it shows the corner's norm which is the average of the adjacent 
	                                                         //triangles' norm. This reveal the technique of average normalization.
	else if (key == '1') { lightOn = !lightOn; }
	else if (key == '2') { sunLightOn = !sunLightOn; }
	else if (key == '3') aimbientLightOn = !aimbientLightOn;
	else if (key == '4') { flareLightOn = !flareLightOn; } // double click 4 to make it happen
}

static void specialKey(int key, int x, int y)
{}

static void initGraphics(void)
{
	/* Black background */
	glClearColor(0, 0, 0, 0);

	/* colour green */
	color3[0] = 0.0f; // R
	color3[1] = 1.0f; // G
	color3[2] = 0.0f; // B
	color3[3] = 1.0f; // B

	/* Needed for vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Popup menu attached to right mouse button */
	AppMenu = glutCreateMenu(menuChoice);
	glutSetMenu(AppMenu);
	glutAddMenuEntry("Red", cmdRed);
	glutAddMenuEntry("Green", cmdGreen);
	glutAddMenuEntry("----", 0);
	glutAddMenuEntry("Exit", cmdExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//initiate texture
	LoadBothTextures();

	/* Start values */
	SpinAngle = 0.0;

	eyePoint[0] = 0.0;
	eyePoint[1] = 150.0;
	eyePoint[2] = 100.0;

	lookAtPoint[0] = 0;
	lookAtPoint[1] = 0;
	lookAtPoint[2] = 0;

	upVec[0] = 0;
	upVec[1] = 1;
	upVec[2] = 0;

	fov = 60; // field of view
	nearPlane = 0.5;
	farPlane = 300;


	menuChoice(cmdGreen);
	
	initLightTexture();

	qobj = gluNewQuadric();
	jet = new Jet();
	//airport = new Airport();


	// particle system
	for (int num = 0; num < 10; num++) initPsystemToDefault(&(psa[num]), num);
	psa[1].typeOfPsystem = 1;;

	psa[1].pos = vec3(5, 5, 40);
	/*
	psa[1].pos = vec3(0, 10, 10);
	psa[1].initialVelosity = vec3(0, 0.02, 0);
	psa[1].wind = vec3(0.0, 0.1, 0.1);
	psa[1].activeticks = 300;
	*/
	psa[1].active = true;
}

void timer(int value)
{
	timeForNextFrame = true;
	glutTimerFunc(timerMSecs, timer, 0);
}

static void spinDisplay(void)
{
	if (timeForNextFrame)
	{
		cnt += 4.0;
		SpinAngle += 2.0;
		if (SpinAngle > 360.0)
			SpinAngle -= 360.0;
		glutPostRedisplay();
		updatePsystem(&(psa[1]));
		timeForNextFrame = false;
	}
}


static void mapNormal(void) {
	readHeightField("F:\\MITS_UC\\GraphicsVisualisationTechniques\\MarkTutorial\\MT2\\AirportLight\\X20_22.txt");
	const int hw = heightField_depth;
	
	


	vec3 norm;
	vec3 p0, p1, p2;


	GLfloat xdelta = 102.0 / 102;
	GLfloat zdelta = 102.0 / 102;
	GLfloat ydelta = 0.009;

	for (int x = 0; x < 102; x++) {
		for (int z = 0; z < 102; z++) {
			norm = (0.0, 0.0, 0.0);
			p0 = vec3(xdelta * x, ydelta*heightField[x][z], zdelta * z);
			if ((x == 0 && z == 0) || (x == 101 && z == 101)) { // two coners 
				if (x == 0 && z == 0) {
					//1
					p1 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					norm = norm + polygonNormal(p0, p1, p2);
				}
				else {
					//4
					p1 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					norm = norm + polygonNormal(p0, p1, p2);
				}
			}

			else if ((x == 0 && z == 101) || (x == 101 && z == 0)) { // other two corners
				if (x == 0 && z == 101) {
					//2
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//3
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					norm = norm / 2;
				}
				else {
					//5
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//6
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					norm = norm / 2;
				}

			}

			else if (x == 0 || x == 101 || z == 0 || z == 101) {
				if (x == 0) {
					//1
					p1 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					norm = norm + polygonNormal(p0, p1, p2);
					//2
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//3
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					norm = norm / 3;
				}
				if (x == 101) {
					//4
					p1 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					norm = norm + polygonNormal(p0, p1, p2);
					//5
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//6
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					norm = norm / 3;
				}
				if (z == 0) {
					//5
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//6
					p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//1
					p1 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					norm = norm + polygonNormal(p0, p1, p2);
					norm = norm / 3;
				}
				else {

					//2
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
					p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//3
					p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					norm = norm + polygonNormal(p0, p1, p2);
					//4
					p1 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
					p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
					norm = norm + polygonNormal(p0, p1, p2);

					norm = norm / 3;
				}
			}

			else {
				//1
				p1 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
				p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
				norm = norm + polygonNormal(p0, p1, p2);
				//2
				p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z], zdelta * (z));
				p2 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
				norm = norm + polygonNormal(p0, p1, p2);
				//3
				p1 = vec3(xdelta * x + xdelta, ydelta*heightField[x + 1][z - 1], zdelta * z - zdelta);
				p2 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
				norm = norm + polygonNormal(p0, p1, p2);
				//4
				p1 = vec3(xdelta * x, ydelta*heightField[x][z - 1], zdelta * z - zdelta);
				p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
				norm = norm + polygonNormal(p0, p1, p2);
				//5
				p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z], zdelta * z);
				p2 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
				norm = norm + polygonNormal(p0, p1, p2);
				//6
				p1 = vec3(xdelta * x - xdelta, ydelta*heightField[x - 1][z + 1], zdelta * z + zdelta);
				p2 = vec3(xdelta * x, ydelta*heightField[x][z + 1], zdelta * z + zdelta);
				norm = norm + polygonNormal(p0, p1, p2);
				norm = norm / 6;

			}

			mapNor[x * 102 + (z + 1)][0] = norm[0];
			mapNor[x * 102 + (z + 1)][1] = norm[1];
			mapNor[x * 102 + (z + 1)][2] = norm[2];
		}
	}

	//return mapNormal[36][3];
}


int main(int argc, char* argv[])
{
	mapNormal();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Jet");

	initGraphics();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutKeyboardFunc(asciiKey);
	glutSpecialFunc(specialKey);

	glutIdleFunc(spinDisplay);
	glutTimerFunc(timerMSecs, timer, 0);

	glutMainLoop();
	/* Should never get here, but keeps compiler happy */
	return 0;
}
