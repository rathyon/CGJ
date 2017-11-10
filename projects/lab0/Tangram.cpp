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

Mesh cube_mesh;
SceneNode* cube;
SceneNode* mr_child;
SceneNode* sniff;

float step_x = 0.0f;
float step_y = 0.0f;
float step_z = 0.0f;

// Camera variables
Camera* cam;

vec3 camDir = vec3(0.0f, 0.0f, -5.0f);

qtrn qRotY = qtrn(0.0f, vec3(0,1,0));
qtrn qRotX = qtrn(0.0f, vec3(-1.0f, 0, 0));
qtrn qRotState = qtrn(1.0f, 0, 0, 0);

bool camForward = false;
bool camBackward = false;

int cameraType = PERSPECTIVE;

/////////////////////////////////////////////////////////////////////// MESH DATA

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

//red
Vertex tri1[] =
{
	/*0*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*1*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

	/*0*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*3*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*4*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },

	/*0*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } }, 
	/*4*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*1*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

	/*1*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*4*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },

	/*1*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*3*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*3*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*0*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

	/*3*/{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },
	/*4*/{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -0.2f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } },

	/*{ { -(sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -2.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 3
	{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), -2.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 4
	{ { 0.0f, 0.0f, -2.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } } // 5*/
};

GLubyte tri_i[] =
{
	0,1,2,
	0,3,4,
	0,4,1,
	1,4,5,
	1,5,2,
	2,5,3,
	2,3,0,
	3,5,4
};

//green
Vertex tri2[] =
{
	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },

	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },

	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },

	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },

	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },

	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	
};

//blue
Vertex tri3[] =
{
	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },

	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },

	/*1*/{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*0*/{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

	/*3*/{ { -0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*4*/{ { 0.5f, -0.5f, -0.2f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	
};

//cyan
Vertex tri4[] =
{
	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },

	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },

	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
};

//magenta
Vertex tri5[] =
{
	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },

	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },

	/*1*/{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },

	/*2*/{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*0*/{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

	/*3*/{ { -0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*5*/{ { 0.0f, 0.0f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*4*/{ { 0.25f, -0.25f, -0.2f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
};

//yellow - original
//mixed - quaternions
Vertex quad[] =
{

	//FRONT
	/*0*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*1*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*2*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*2*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*3*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	/*0*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

	//BOTTOM
	/*0*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*4*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*1*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	/*4*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*5*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.0f, 1.0f } },
	/*1*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },

	//RIGHT
	/*1*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*2*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	/*5*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*6*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.0f, 0.5f, 1.0f } },
	/*2*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

	//TOP
	/*2*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*6*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*3*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	/*6*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*7*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.0f, 0.5f, 1.0f } },
	/*3*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

	//LEFT
	/*3*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*7*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*0*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	/*7*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*4*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.0f, 0.5f, 0.5f, 1.0f } },
	/*0*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.4f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

	//BACK
	/*4*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },
	/*7*/{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },
	/*6*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },
	/*6*/{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },
	/*5*/{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },
	/*4*/{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, -0.4f, 1.0f },{ 0.5f, 0.5f, 0.0f, 1.0f } },

};

GLubyte quad_i[] =
{
	//top
	0,1,2,
	2,3,0,

	//front
	0,4,1,
	4,5,1,

	//right
	1,5,2,
	5,6,2,

	//back
	2,6,3,
	6,7,3,

	//left
	3,7,0,
	7,4,0,

	//bottom
	4,7,6,
	6,5,4
};

//orange
Vertex para[] =
{
	/*0*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*1*/{ { 1.0f / 4.0f - 0.125f, -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*2*/{ { 1.0f / 4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*2*/{ { 1.0f / 4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*3*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*0*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*0*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*4*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*1*/{ { 1.0f / 4.0f - 0.125f, -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*4*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*5*/{ { 1.0f / 4.0f - 0.125f, -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*1*/{ { 1.0f / 4.0f - 0.125f, -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*1*/{ { 1.0f / 4.0f - 0.125f, -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*5*/{ { 1.0f / 4.0f - 0.125f, -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*2*/{ { 1.0f / 4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*5*/{ { 1.0f / 4.0f - 0.125f, -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*6*/{ { 1.0f / 4.0f + 0.125f, 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*2*/{ { 1.0f / 4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*2*/{ { 1.0f / 4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*6*/{ { 1.0f / 4.0f + 0.125f, 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*3*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*6*/{ { 1.0f / 4.0f + 0.125f, 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*7*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*3*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*3*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	/*7*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*0*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*7*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*4*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*0*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },

	/*4*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*7*/{ { -(1.0f / 4.0f - 0.125f), 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*6*/{ { 1.0f / 4.0f + 0.125f, 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },

	/*6*/{ { 1.0f / 4.0f + 0.125f, 0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*5*/{ { 1.0f / 4.0f - 0.125f, -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	/*4*/{ { -((1.0f / 4.0f) + 0.125f), -0.125f, -0.2f, 1.0f },{ 0.5f, 0.25f, 0.0f, 1.0f } },
	
};


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
	sp.setAttributeName(Shader::COLORS_ATTRIB, "in_Color");
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


/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void computeVAO(int objID, Vertex vertices[], int vSize, GLubyte indices[], int iSize) {
	glBindVertexArray(vao_ID[objID]);

	//glGenBuffers(2, vbo_ID[objID]);
	glGenBuffers(1, vbo_ID[objID]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ID[objID][0]);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(vertices[0].XYZW));

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ID[objID][1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);*/

}

void setupScene(){

	glGenVertexArrays(objCount, vao_ID);

	computeVAO(objID, quad, sizeof(quad), quad_i, sizeof(quad_i));

	cam = new Camera();
	cam->setProjectionMatrix(perspective(30.0f, (1.0f * WinX) / WinY, 0.1f, 1000.0f));

	scene.setCamera(cam);
	scene.setShader(sp.programID);

	cube = new SceneNode("cube :D");
	mr_child = new SceneNode("mr child");
	sniff = new SceneNode("sniff");

	cube_mesh.setVAO(vao_ID[objID]);
	cube_mesh.setVertexCount(36);

	cube->setShader(sp.programID);
	cube->setMesh(cube_mesh);

	mr_child->setShader(sp.programID);
	mr_child->setMesh(cube_mesh);

	sniff->setShader(sp.programID);
	sniff->setMesh(cube_mesh);

	mr_child->add(sniff);
	cube->add(mr_child);

	scene.add(cube);

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

	if (camForward) {
		camDir.z += 0.01f;
	} 
	else if (camBackward) {
		camDir.z -= 0.01f;
	}
}

void drawScene()
{
	/*
	/*\------------------/ THE FIRST IS THE LAST \------------------/*\
	/*\------------------/ THE LAST IS THE FIRST \------------------/*\
	*/
	animateCamera();

	cam->setViewMatrix(mat4_translation(camDir)*toMat4(qRotState));

	// rendering using new classes:

	cube->setMatrix(
		mat4_identity()
	);

	mr_child->setMatrix(
		translate(1,0,0)*rotate(45, 0, 1, 0)
	);

	step_y += 0.1f;

	sniff->setMatrix(
		translate(0,1,0)*rotate(45.0f + step_y,0,1,0)
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

	case 'p': if (cameraType == PERSPECTIVE) cameraType = ORTHOGRAPHIC; else cameraType = PERSPECTIVE; break;
	case 'w': camForward = true; break;
	case 's': camBackward = true; break;
	case 'r': qRotState = qtrn(1, 0, 0, 0);
	
	}
}

void keyUp(unsigned char key, int xx, int yy) {

	switch (key) {

	case 'w': camForward = false; break;
	case 's': camBackward = false; break;
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