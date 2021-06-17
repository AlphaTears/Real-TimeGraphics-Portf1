// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <glew/glew.h>
#include <glew/wglew.h>
#include <GL\freeglut.h>
#include <CoreStructures\CoreStructures.h>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "src/CGTexturedQuad.h"
#include "src/aiWrapper.h"
#include "src/CGPrincipleAxes.h"
#include "src/texture_loader.h"
#include "VBOMaker.h"
#include "src/shader_setup.h"

using namespace std;
using namespace CoreStructures;

#pragma region Scene variables and resources

// variables used to keep track of position of the mouse, also extra variables for movement of the player and to make the window full screen
int								mouse_x, mouse_y;
bool							mDown = false;
float							playerX = 0, playerY = 5, playerZ = -200;
bool							Wkeydown, aKeyDown, sKeyDown, dKeyDown;


GUClock* mainClock = nullptr;

//
// Main scene resources
//
GUPivotCamera* mainCamera = nullptr;
CGPrincipleAxes* principleAxes = nullptr;
CGTexturedQuad* texturedQuad = nullptr;

//Objects
const aiScene* aiBeast;
GLuint		beastTexture;
VBOMaker	*beast;

const aiScene* aiTarget;
GLuint	TargetTexture;
VBOMaker* target;

const aiScene* aiMap;
GLuint MapTexture;
VBOMaker* Map;

const aiScene* aiRunner;
GLuint RunnerTexture;
VBOMaker* Runner;


//Graphics shader handler
GLuint		meshShader;

//lights!! Not used! 
/*GLuint							LocLcolour;
GLuint							LocL;
GLuint							LocLk;
GLuint							LocLdirect;
GLuint							LocLdisp;

GLuint							LocLtype;*/


//Screen Size 
int windowWidth = 800;
int windowHeight = 600;
float viewportAspect = (float)windowWidth / (float)windowHeight;
GUMatrix4 T;

//camera variables 
GUVector4 camFocalPoint = GUVector4(0.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion


#pragma region Function Prototypes

void init(int argc, char* argv[]); // initialiser for the main scene.
void update(void); // Main scene update function
void display(void); // Main scene render

// functions used to handle different events
void mouseButtonDown(int button_id, int state, int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);
void keyDown(unsigned char key, int x, int y);
void closeWindow(void);
void reportContextVersion(void);
void reportExtensions(void);

#pragma endregion


int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();

	// Stopping the clock and data.
	if (mainClock) {

		mainClock->stop();
		mainClock->reportTimingData();
		mainClock->release();
	}

	return 0;
}


void init(int argc, char* argv[]) {

	// Starting FreeGLUT
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE/* | GLUT_MULTISAMPLE*/);
	glutSetOption(GLUT_MULTISAMPLE, 4);

	// setting up our window
	
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("Sample Scene");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// setting up the callback functions
	glutIdleFunc(update); // Main scene update function
	glutDisplayFunc(display); // Main render function
	glutKeyboardFunc(keyDown); // Key down handler
	glutMouseFunc(mouseButtonDown); // Mouse button handler
	glutMotionFunc(mouseMove); // Mouse move handler
	glutMouseWheelFunc(mouseWheel); // Mouse wheel event handler
	glutCloseFunc(closeWindow); // Main resource cleanup handler


	// Starting glew 
	glewInit();

	// starting the GL functions

	wglSwapIntervalEXT(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW); // Default anyway

	// Setup colour to clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Setup main camera
	mainCamera = new GUPivotCamera(0.0f, 0.0f, 50.0f, 55.0f, viewportAspect, 0.1f);

	//geting all the data that we need from the objects and textures
	//beast
	aiBeast = aiImportModel(string("beast.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	beastTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\beast_texture.bmp", TextureProperties(false));
	beast = new VBOMaker(aiBeast);
	
	//target
	aiTarget = aiImportModel(string("Target.fbx"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	TargetTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\TargetTexture.jpg", TextureProperties(false));	
	target = new VBOMaker(aiTarget);


	//map
	aiMap = aiImportModel(string("CityMap.fbx"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	MapTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\CityTextureFineshed.jpg", TextureProperties(false));
	Map = new VBOMaker(aiMap);

	//character
	aiRunner = aiImportModel(string("Charactertryout.fbx"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	RunnerTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\CharacterTextureFinished.jpg", TextureProperties(false));
	Runner = new VBOMaker(aiRunner);



	//setup for the shader that we are using in our repository.
	meshShader = setupShaders(string("shaders\\Mesh_shader.vs"), string("shaders\\Mesh_shader.fs"));

	// starting a new clock
	mainClock = new GUClock();

	//Lights!! Not used!
	/*LocLcolour = glGetUniformLocation(meshShader, "Lcolour");
	LocL = glGetUniformLocation(meshShader, "L");
	LocLk = glGetUniformLocation(meshShader, "Lk");
	LocLdirect = glGetUniformLocation(meshShader, "Ldirect");
	LocLdisp = glGetUniformLocation(meshShader, "Ldisp");

	LocLtype = glGetUniformLocation(meshShader, "Ltype");


	// position
	float L[] = { -22.0f, 70.0f, 55.0f,
				-27.0f, -5.0f, 28.0f };		
	glUseProgram(meshShader);
	glUniform3fv(LocL, 2, (GLfloat*)&L);


	//attenuation
	float Lk[] = { 1.0f, 0.000f, 0.0f ,
				1.0f, 0.000f, 0.0f };		
	glUseProgram(meshShader);
	glUniform3fv(LocLk, 2, (GLfloat*)&Lk);


	//colour reference 
	float Lcolour[] = { 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f, 0.0f };		
	glUseProgram(meshShader);
	glUniform3fv(LocLcolour, 2, (GLfloat*)&Lcolour);


	//direction 
	float Ldirect[] = { 0.0f, 1.0f, 0.0f,
						0.0f, -1.0f, 0.0f };		
	glUseProgram(meshShader);
	glUniform3fv(LocLdirect, 2, (GLfloat*)&Ldirect);


	//display
	float Ldisp[] = { (3.142 / 180.0 * 45.0),
					  (3.142 / 180.0 * 65.0) };		
	glUseProgram(meshShader);
	glUniform1fv(LocLdisp, 2, (GLfloat*)&Ldisp);

	//type of light, spotlight, directional, point
	int Ltype[] = { 1, 2 };
	glUseProgram(meshShader);
	glUniform1iv(LocLtype, 2, (GLint*)&Ltype);*/
}

// Main scene update function (called by FreeGLUT's main event loop every frame) 
void update(void) {
	int dx;

	int dy;

	// Update clock
	mainClock->tick();

	// Redraw the screen
	display();
	
	// Update the window title to show current frames-per-second and seconds-per-frame data
	char timingString[256];
	sprintf_s(timingString, 256, "CIS5013. Average fps: %.0f; Average spf: %f", mainClock->averageFPS(), mainClock->averageSPF() / 1000.0f);
	glutSetWindowTitle(timingString);

	//display of the inputs from the keyboard
	if (Wkeydown == true)
	{
		playerZ = playerZ + 0.5;
		Wkeydown = false;
		camFocalPoint.z = playerZ + 1.0f;
		cout << playerZ << endl;
		
	}
	if (sKeyDown == true)
	{
		playerZ = playerZ - 0.5;
		sKeyDown = false;
	
	}
	if (aKeyDown == true)
	{
		playerX = playerX + 0.5f;
		aKeyDown = false;
		cout << playerX << endl;
	}
	if (dKeyDown == true)
	{
		playerX = playerX - 0.5f;
		dKeyDown = false;
		cout << playerX << endl;
	}
	
	
	
}

//rendering our main scene
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setting our view port to the current size given to it which can then be changed pressing a key
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Get view-projection transform as a GUMatrix4 and using the player position to move the camera with it.
	T = mainCamera->projectionTransform() * mainCamera->viewTransform() * GUMatrix4::translationMatrix(-playerX, -playerY, -playerZ);

	if (principleAxes)
		principleAxes->render(T);

	// Rendering all the objects we have in the scene with all of their oen Matrixes.
	
	
	glBindTexture(GL_TEXTURE_2D, TargetTexture);
	glEnable(GL_TEXTURE_2D);
	static GLint mvpLocation1 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMatZ = GUMatrix4::translationMatrix(20, -7, 0) * GUMatrix4::scaleMatrix(1.0f, 1.0f, 1.0f) * GUMatrix4::rotationMatrix(4.7f, 0.0f , 0.0f);		//x,y,z		//x,y,z
	GUMatrix4 mvpMatrix1 = T * modelMatZ;
	glUniformMatrix4fv(mvpLocation1, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix1.M));
	target->render();


	glBindTexture(GL_TEXTURE_2D, MapTexture);
	glEnable(GL_TEXTURE_2D);
	static GLint mvpLocation2 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMatX = GUMatrix4::translationMatrix(0, -25, 0) * GUMatrix4::scaleMatrix(2.0f, 2.0f, 2.0f) * GUMatrix4::rotationMatrix(4.7f, 0.0f, 0.0f);		//x,y,z		//x,y,z
	GUMatrix4 mvpMatrix2 = T * modelMatX;
	glUniformMatrix4fv(mvpLocation2, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix2.M));
	Map->render();

	glBindTexture(GL_TEXTURE_2D, RunnerTexture);
	glEnable(GL_TEXTURE_2D);
	static GLint mvpLocation3 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMatC = GUMatrix4::translationMatrix(0, 10, 0) * GUMatrix4::scaleMatrix(0.1f, 0.1f, 0.1f) * GUMatrix4::rotationMatrix(4.7f, 0.0f, 0.0f);		//x,y,z		//x,y,z
	GUMatrix4 mvpMatrix3 = T * modelMatC;
	glUniformMatrix4fv(mvpLocation3, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix3.M));
	Runner->render();

	glBindTexture(GL_TEXTURE_2D, beastTexture);
	glEnable(GL_TEXTURE_2D);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);*/
	static GLint mvpLocation = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMat = GUMatrix4::translationMatrix(playerX, playerY, playerZ) * GUMatrix4::scaleMatrix(25.0f, 25.0f, 25.0f);		//x,y,z		//x,y,z
	GUMatrix4 mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));
	beast->render();
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);


	glutSwapBuffers();
}



#pragma region Event handling functions
//event hendler for mouse buttons
void mouseButtonDown(int button_id, int state, int x, int y) {

	if (button_id == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN) {

			mouse_x = x;
			mouse_y = y;

			mDown = true;

		}
		else if (state == GLUT_UP) {

			mDown = false;
		}
	}
}

//tracking the movement and placing of the mouse
void mouseMove(int x, int y) {

	int dx = x - mouse_x;
	int dy = y - mouse_y;

	if (mainCamera)
		mainCamera->transformCamera((float)-dy, (float)-dx, 0.15f);

	mouse_x = x;
	mouse_y = y;
	
}

//event handler for the wheel of the mouse
void mouseWheel(int wheel, int direction, int x, int y) {


	

	if (mainCamera) {

		if (direction < 0)
			mainCamera->scaleCameraRadius(1.1f);
		else if (direction > 0)
			mainCamera->scaleCameraRadius(0.9f);
		
		



	}
}


void keyDown(unsigned char key, int x, int y) {

	

	// Toggle fullscreen, used also for track when keys are pressed
	if (key == 'f')
		glutFullScreenToggle();
	if (key == 'w')
		Wkeydown = true;
	if (key == 's')
		sKeyDown = true;
	if (key == 'a')
		aKeyDown = true;
	if (key == 'd')
		dKeyDown = true;
	

	

}


void closeWindow(void) {

	//cleaning up any resources within our scene

	if (mainCamera)
		mainCamera->release();

	if (principleAxes)
		principleAxes->release();

	if (texturedQuad)
		texturedQuad->release();

	
}


#pragma region Helper Functions
//reporting the gl version used.
void reportContextVersion(void) {

	int majorVersion, minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	cout << "OpenGL version " << majorVersion << "." << minorVersion << "\n\n";
}
//getting all the extentision used
void reportExtensions(void) {

	cout << "Extensions supported...\n\n";

	const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);

	char* strEnd = (char*)glExtensionString + strlen(glExtensionString);
	char* sptr = (char*)glExtensionString;

	while (sptr < strEnd) {

		int slen = (int)strcspn(sptr, " ");
		printf("%.*s\n", slen, sptr);
		sptr += slen + 1;
	}
}

#pragma endregion


#pragma endregion

