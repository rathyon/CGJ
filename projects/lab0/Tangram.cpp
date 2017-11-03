///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// Assignment: 1. Create a class for Quaternions.
//             2. Create a scene with a camera rotating around an 
//                object at the origin and pointing towards it. 
//                Do NOT use "gluLookAt" to create the ViewMatrix, 
//                use rotation matrices!
//             3. Gimbal lock mode ON: use Translation + Rotation 
//                matrices (e.g. Euler angles, Rodrigues’ formula).
//             4. Gimbal lock mode OFF: use Quaternions to produce a 
//                transformation matrix and avoid gimbal lock.
//             5. Switch between modes by pressing the 'g' key.
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

#define PRINT(s) std::cout << s << std::endl;
#define PERSPECTIVE 0
#define ORTHOGRAPHIC 1

#define CAPTION "Quaternion Rotation App"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

//mouse variables

int startX, startY, tracking = 0;

#define VERTICES 0
#define COLORS 1

#define RODRIGUES 0
#define QUATERNIONS 1

Shader sp;

//const int objCount = 7; /* for tangram*/
const int objCount = 1;
int objID = 0;

float matrix[16];

GLuint vao_ID[objCount];
GLuint vbo_ID[objCount][2];

GLint ModelMatrix_ID;
GLint ViewMatrix_ID;
GLint ProjectionMatrix_ID;

mat4 ModelMatrix;
mat4 ViewMatrix;
mat4 ProjectionMatrix;

vec3 camDir = vec3(0.0f, 0.0f, -5.0f);
vec3 camFocus = vec3(0.0f);
vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
vec3 camRight = vec3(-1.0f, 0.0f, 0.0f);
float camAngle = 0.0f;
float camPitch = 0.0f;

qtrn qRotY = qtrn(camAngle, camUp);
qtrn qRotX = qtrn(camPitch, camRight);

qtrn qRotState = qtrn(1.0f, 0, 0, 0);

bool camForward = false;
bool camBackward = false;

int cameraType = PERSPECTIVE;
int rotationType = RODRIGUES;

// unused
GLuint VertexShaderId, FragmentShaderId, ProgramId; // please remove these!

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

	ModelMatrix_ID = glGetUniformLocation(sp.programID, "ModelMatrix");
	ViewMatrix_ID = glGetUniformLocation(sp.programID, "ViewMatrix");
	ProjectionMatrix_ID = glGetUniformLocation(sp.programID, "ProjectionMatrix");

	// error checking
	sp.shaderInfoLog(Shader::VERTEX_SHADER);
	sp.shaderInfoLog(Shader::FRAGMENT_SHADER);
	sp.programInfoLog();

	checkOpenGLError("ERROR: Could not create shaders.");
}

// unused: must correct
void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
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

void createBufferObjects(){

	glGenVertexArrays(objCount, vao_ID);

	// for tangram
	/*computeVAO(objID++, tri1, sizeof(tri1), tri_i, sizeof(tri_i));
	computeVAO(objID++, tri2, sizeof(tri2), tri_i, sizeof(tri_i));
	computeVAO(objID++, tri3, sizeof(tri3), tri_i, sizeof(tri_i));
	computeVAO(objID++, tri4, sizeof(tri4), tri_i, sizeof(tri_i));
	computeVAO(objID++, tri5, sizeof(tri5), tri_i, sizeof(tri_i));
	computeVAO(objID++, quad, sizeof(quad), quad_i, sizeof(quad_i));
	computeVAO(objID, para, sizeof(para), quad_i, sizeof(quad_i));*/ 


	computeVAO(objID, quad, sizeof(quad), quad_i, sizeof(quad_i));

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

void drawObj(int vertexCount) {
	glBindVertexArray(vao_ID[objID]);
	glUseProgram(sp.programID);
	toGLFormat(ModelMatrix, matrix);
	glUniformMatrix4fv(ModelMatrix_ID, 1, GL_TRUE, matrix);
	//glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void animateCamera() {

	if (camForward) {
		camDir.z += 0.002f;
	} 
	else if (camBackward) {
		camDir.z -= 0.002f;
	}
}

void drawScene()
{
	mat3 aux;
	mat4 T_inv;
	glUseProgram(sp.programID);
	// the array "matrix" is a float storage to send to OpenGL

	if (cameraType == PERSPECTIVE)
		ProjectionMatrix = perspective(30.0f, (1.0f * WinX) / WinY, 0.1f, 1000.0f);
	else
		ProjectionMatrix = ortho(-2, 2, -2, 2, 0.1f, 100.0f);

	toGLFormat(ProjectionMatrix, matrix);
	glUniformMatrix4fv(ProjectionMatrix_ID, 1, GL_TRUE, matrix);

	animateCamera();

	if (rotationType == RODRIGUES) {
		ViewMatrix = mat4_translation(camDir)*mat4_rotation(camPitch, camRight)*mat4_rotation(camAngle, camUp);
	}
	else { // QUATERNIONS
		ViewMatrix = mat4_translation(camDir)*toMat4(qRotState);
	}

	toGLFormat(ViewMatrix, matrix);
	glUniformMatrix4fv(ViewMatrix_ID, 1, GL_TRUE, matrix);

	// yellow quad for quaternions
	objID = 0;
	ModelMatrix = mat4_identity();
	drawObj(36);

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

	case 'g': if (rotationType == RODRIGUES) {
					rotationType = QUATERNIONS;
					std::cout << "Quaternion Rotation" << std::endl;
			  }
			  else {
				  rotationType = RODRIGUES;
				  std::cout << "Euler Rotation" << std::endl;
			  } 
			  break;

	case 'w': camForward = true; break;
	case 's': camBackward = true; break;
	case 'r': camAngle = 0.0f; camPitch = 0.0f; qRotState = qtrn(1, 0, 0, 0);
	
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

		camAngle -= deltaX*0.012f;
		camPitch -= deltaY*0.012f;

		qRotY = qtrn(-deltaX*0.012f, camUp);
		qRotX = qtrn(-deltaY*0.012f, camRight);

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
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////