///////////////////////////////////////////////////////////////////////
//
//  Loading OBJ mesh from external file
//
//	Final individual assignment:
//	1.	Create classes: Scene, Camera and Mesh (that loads a
//		mesh from a Wavefront OBJ file) and build a small
//		scenegraph of your tangram scene (you may create more 
//		classes if needed).
//	2.	Create a ground object and couple the tangram figure
//		to the ground. Press keys to move the ground about: 
//		the tangram figure must follow the ground.
//	3.	Animate between closed puzzle (initial square) and 
//		tangram figure by pressing a key.
//	4.	Spherical camera interaction through mouse. It should
//		be possible to interact while animation is playing.
//
//	Team assignment:
//	Pick your team (3 elements from one same lab) for the team
//	assignment you will be working until the end of the semester,
//	and register it online.
//
// (c) 2013-17 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"

//Student-made
#include "MathLib.h"
#include "Shader.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Mesh.h"

#define PRINT(s) std::cout << s << std::endl;
#define PERSPECTIVE 0
#define ORTHOGRAPHIC 1

#define CAPTION "Animation Viewer"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

//mouse variables

int startX, startY, tracking = 0;

#define VERTICES 0
#define COLORS 1

Shader sp;

//const int objCount = 7; /* for tangram*/
const int objCount = 1;
int objID = 0;

//GLSL stuff

GLuint vao_ID[objCount];
GLuint vbo_ID[objCount][2];

// Matrices
mat4 ModelMatrix;
mat4 ViewMatrix;
mat4 ProjectionMatrix;

// Scene Variables
Scene scene;

SceneNode* ground;

SceneNode* small_tri;
SceneNode* small_tri2;
SceneNode* medium_tri;
SceneNode* large_tri;
SceneNode* large_tri2;
SceneNode* cube;
SceneNode* parallelogram;

float rise = 0.0f;
float place = 0.0f;
float step = 0.005f;

bool placed = false;

bool play = false;
bool reverse = false;

float ground_z = 0.0f;
float ground_x = 0.0f;
float ground_step = 0.1f;

bool up = false;
bool down = false;
bool left = false;
bool right = false;

// Camera variables
Camera* cam;

vec3 camDir = vec3(0.0f, 0.0f, -100.0f);

qtrn qRotY = qtrn(0.0f, vec3(0,1,0));
qtrn qRotX = qtrn(0.0f, vec3(-1.0f, 0, 0));
qtrn qRotState = qtrn(1.0f, 0, 0, 0);

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.ignore();
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{
	sp.init();

	sp.loadShader(Shader::VERTEX_SHADER, "shaders/tangram.vert");
	if (!sp.isShaderCompiled(Shader::VERTEX_SHADER))
		PRINT("Vertex shader is not compiled!");
	sp.loadShader(Shader::FRAGMENT_SHADER, "shaders/tangram.frag");
	if (!sp.isShaderCompiled(Shader::FRAGMENT_SHADER))
		PRINT("Fragment shader is not compiled!");

	sp.setAttributeName(Shader::VERTICES_ATTRIB, "in_Position");
	sp.setAttributeName(Shader::NORMALS_ATTRIB, "in_Normal");
	sp.linkProgram();

	// error checking
	sp.shaderInfoLog(Shader::VERTEX_SHADER);
	sp.shaderInfoLog(Shader::FRAGMENT_SHADER);
	sp.programInfoLog();

	checkOpenGLError("ERROR: Could not create shaders.");
}

// unused: must correct
void destroyShaderProgram()
{
	/*glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");*/
}

void setupScene(){

	cam = new Camera();
	cam->setProjectionMatrix(perspective(30.0f, (1.0f * WinX) / WinY, 0.1f, 1000.0f));

	scene.setCamera(cam);
	scene.setShader(sp.programID);

	ground = new SceneNode("ground");

	cube = new SceneNode("cube");
	small_tri = new SceneNode("small_tri");
	small_tri2 = new SceneNode("small_tri2");
	medium_tri = new SceneNode("medium_tri");
	large_tri = new SceneNode("large_tri");
	large_tri2 = new SceneNode("large_tri2");
	parallelogram = new SceneNode("parallelogram");


	float red[4] = { 1,0,0,1 };
	float green[4] = { 0,1,0,1 };
	float blue[4] = { 0,0,1,1 };

	float cyan[4] = { 0,1,1,1 };
	float yellow[4] = { 1,1,0,1 };
	float magenta[4] = { 1,0,1,1 };

	float purple[4] = { 0.6f,0,1,1 };
	float gray[4] = { 0.5f , 0.5f, 0.5f, 1 };

	Mesh ground_m = Mesh("ground.obj", gray);

	Mesh small_tri_m = Mesh("small_triangle.obj", red);
	Mesh small_tri2_m = Mesh("small_triangle2.obj", green);
	Mesh medium_tri_m = Mesh("medium_triangle.obj", blue);
	Mesh large_tri_m = Mesh("large_triangle.obj", cyan);
	Mesh large_tri2_m = Mesh("large_triangle2.obj", yellow);
	Mesh parallelogram_m = Mesh("parallelogram.obj", magenta);
	Mesh cube_m = Mesh("cube.obj", purple);

	ground->setShader(sp.programID);
	ground->setMesh(ground_m);

	cube->setShader(sp.programID);
	cube->setMesh(cube_m);

	small_tri->setShader(sp.programID);
	small_tri->setMesh(small_tri_m);

	small_tri2->setShader(sp.programID);
	small_tri2->setMesh(small_tri2_m);

	medium_tri->setShader(sp.programID);
	medium_tri->setMesh(medium_tri_m);

	large_tri->setShader(sp.programID);
	large_tri->setMesh(large_tri_m);

	large_tri2->setShader(sp.programID);
	large_tri2->setMesh(large_tri2_m);

	parallelogram->setShader(sp.programID);
	parallelogram->setMesh(parallelogram_m);

	ground->add(small_tri);
	ground->add(small_tri2);
	ground->add(medium_tri);
	ground->add(large_tri);
	ground->add(large_tri2);
	ground->add(parallelogram);
	ground->add(cube);

	scene.add(ground);

	/* END OF NEW STUFF */

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	for (int i = 0; i < objCount; i++) {
		glBindVertexArray(vao_ID[i]);
		glDisableVertexAttribArray(VERTICES);
		glDisableVertexAttribArray(COLORS);
		glDeleteBuffers(2, vbo_ID[i]);
		glDeleteVertexArrays(1, &vao_ID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE
void animateCamera() {
}

void animateScene() {

	if (play) {
		if (!placed) {
			if (rise < 1.0f)
				rise += step;
			else if (place < 1.0f)
				place += step;
			else
				placed = true;
		}
		else {
			if (rise > 0.0f)
				rise -= step;
		}
	}

	else if (reverse) {
		if (placed) {
			if (rise < 1.0f)
				rise += step;
			else
				placed = false;
		}
		else {
			if (place > 0.0f)
				place -= step;
			else if (rise > 0.0f)
				rise -= step;
		}
	}

}

void animateGround() {

	if (up)
		ground_z -= ground_step;
	if (down)
		ground_z += ground_step;
	if (left)
		ground_x -= ground_step;
	if (right)
		ground_x += ground_step;
}

void drawScene()
{
	/*
	/*\------------------/ THE LAST TRANSFORM IS THE FIRST APPLIED \------------------/*\
	*/
	animateCamera();
	animateScene();
	animateGround();

	cam->setViewMatrix(mat4_translation(camDir)*toMat4(qRotState));

	// rendering using new classes:

	/** /
	scene.getRoot()->setMatrix(
		mat4_identity()
	);
	/**/

	ground->setMatrix(
		translate(ground_x, -5, ground_z)
	);

	//purple
	cube->setMatrix(
		translate(-1.7*place, -12 * rise,-10*place)*translate(ground_x, 0, ground_z)*rotate(-45*place,0,1,0)*translate(-ground_x, 5, -ground_z)
	);

	//red
	small_tri->setMatrix(
		translate(0, -8 * rise, 2 * place)*translate(ground_x, 0, ground_z)*rotate(90 * place, 0, 1, 0)*translate(-ground_x, 5, -ground_z)
	);

	//green
	small_tri2->setMatrix(
		translate(-2.5*place, -4 * rise, -0.5*place)*translate(ground_x, 0, ground_z)*rotate(0, 0, 1, 0)*translate(-ground_x, 5, -ground_z)
	);

	//blue
	medium_tri->setMatrix(
		translate(0, 0, -10 * place)*translate(ground_x, 0, ground_z)*rotate(-45 * place, 0, 1, 0)*translate(-ground_x, 5, -ground_z)
	);

	//cyan
	large_tri->setMatrix(
		translate(0, 4 * rise, 2 * place)*translate(ground_x, 0, ground_z)*rotate(90 * place, 0, 1, 0)*translate(-ground_x, 5, -ground_z)
	);

	//yellow
	large_tri2->setMatrix(
		translate(2.5*place, 8 * rise, -0.5*place)*translate(ground_x, 0, ground_z)*rotate(90 * place, 0, 1, 0)*translate(-ground_x, 5, -ground_z)
	);

	//magenta
	parallelogram->setMatrix(
		translate(-3.4*place, 12 * rise,-17 * place)*translate(ground_x, 0, ground_z)*rotate(75 * place,0,1,0)*translate(-ground_x, 5, -ground_z)
	);

	scene.render();

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	/*destroyShaderProgram();*/
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void keyDown(unsigned char key, int xx, int yy) {
	switch (key) {

	case 'w': up = true; break;
	case 's': down = true; break;
	case 'a': left = true; break;
	case 'd': right = true; break;

	case 'r': qRotState = qtrn(1, 0, 0, 0); break;

	case 'z': reverse = true; break;
	case 'c': play = true; break;
	
	}
}

void keyUp(unsigned char key, int xx, int yy) {

	switch (key) {

	case 'w': up = false; break;
	case 's': down = false; break;
	case 'a': left = false; break;
	case 'd': right = false; break;

	case 'z': reverse = false; break;
	case 'c': play = false; break;
	}

}

void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
	}
	else if (state == GLUT_UP) {
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		qRotY = qtrn(-deltaX*0.012f, vec3(0,1,0));
		qRotX = qtrn(-deltaY*0.012f, vec3(-1.0f, 0, 0));

		qRotState = qRotX*qRotY*qRotState;
	}

}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	createShaderProgram();
	setupScene();

	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////