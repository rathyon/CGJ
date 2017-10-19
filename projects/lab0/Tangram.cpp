///////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Modern OpenGL.
// A "hello world" of Modern OpenGL.
//
// Assignment : Create Shader Abstraction 
//					(e.g. check compilation/linkage for errors, read from file) 
//			  : Manage Multiple Drawable Entities (using your vertex and matrix classes)
//              Draw a set of 7 TANs (i.e. TANGRAM shapes) of different colors: 
//              (1) 3 different TAN geometric shapes at the origin:
//					- right triangle
//					- square
//					- parallelogram
//              (2) 7 TANs of different colors put together to form a shape of your choice:
//					- 2 big right triangles
//					- 1 medium right triangle
//					- 2 small right triangles
//					- 1 square
//					- 1 parallelogram;
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

#define CAPTION "Hello Modern 2D World"

int WinX = 640, WinY = 640;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1

Shader sp;

const int objCount = 7;
int objID = 0;

float matrix[16];

GLuint vao_ID[objCount];
GLuint vbo_ID[objCount][2];



GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

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

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 Matrix;\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = Matrix * in_Position;\n"
	"	ex_Color = in_Color;\n"
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 ex_Color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = ex_Color;\n"
	"}\n"
};

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

	UniformId = glGetUniformLocation(sp.programID, "Matrix");

	sp.shaderInfoLog(Shader::VERTEX_SHADER);
	sp.shaderInfoLog(Shader::FRAGMENT_SHADER);
	sp.programInfoLog();

	checkOpenGLError("ERROR: Could not create shaders.");
}

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

/////////////////////////////////////////////////////////////////////// MESH DATA

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

//red
Vertex tri1[] =
{
	{ { -(sqrt(2.0f)/4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { (sqrt(2.0f) / 4.0f), -(sqrt(2.0f) / 4.0f), 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } }
};

GLubyte tri1_i[] =
{
	0,1,2
};

//green
Vertex tri2[] =
{
	{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } }
};

GLubyte tri2_i[] =
{
	0,1,2
};

//blue
Vertex tri3[] =
{
	{ { -0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
};

GLubyte tri3_i[] =
{
	0,1,2
};

//cyan
Vertex tri4[] =
{
	{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } }
};

GLubyte tri4_i[] =
{
	0,1,2
};

//magenta
Vertex tri5[] =
{
	{ { -0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	{ { 0.25f, -0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
	{ { 0.0f, -0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } }
};

GLubyte tri5_i[] =
{
	0,1,2
};

//yellow
Vertex quad[] =
{
	{ { -sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { sqrt(2.0f) / 8.0f, -sqrt(2.0f) / 8.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -sqrt(2.0f) / 8.0f, sqrt(2.0f) / 8.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
};

GLubyte quad_i[] =
{
	0,1,2,2,3,0
};

//orange
Vertex para[] =
{
	{ { -((1.0f/4.0f) + 0.125f), -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	{ { 1.0f /4.0f - 0.125f, -0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	{ { 1.0f /4.0f + 0.125f, 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
	{ { -(1.0f /4.0f - 0.125f), 0.125f, 0.0f, 1.0f },{ 1.0f, 0.5f, 0.0f, 1.0f } },
};

GLubyte para_i[] =
{
	0,1,2,2,3,0
};

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void computeVAO(int objID, Vertex vertices[], int vSize, GLubyte indices[], int iSize) {
	glBindVertexArray(vao_ID[objID]);

	glGenBuffers(2, vbo_ID[objID]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ID[objID][0]);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(vertices[0].XYZW));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ID[objID][1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);

}

void createBufferObjects(){

	glGenVertexArrays(objCount, vao_ID);

	computeVAO(objID++, tri1, sizeof(tri1), tri1_i, sizeof(tri1_i));
	computeVAO(objID++, tri2, sizeof(tri2), tri2_i, sizeof(tri2_i));
	computeVAO(objID++, tri3, sizeof(tri3), tri3_i, sizeof(tri3_i));
	computeVAO(objID++, tri4, sizeof(tri4), tri4_i, sizeof(tri4_i));
	computeVAO(objID++, tri5, sizeof(tri5), tri5_i, sizeof(tri5_i));
	computeVAO(objID++, quad, sizeof(quad), quad_i, sizeof(quad_i));
	computeVAO(objID, para, sizeof(para), para_i, sizeof(para_i));

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
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, matrix);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_BYTE, (GLvoid*)0);
}

void drawScene()
{
	// medium red triangle
	objID = 0;
	toGLFormat(mat4_translation(-0.25f, 0.6f, 0.0f)*mat4_rotation(45, vec3(0,0,1)), matrix);
	drawObj(3);

	//large green triangle
	objID = 1;
	toGLFormat(mat4_translation(0.25f, -0.4f, 0)*mat4_rotation(-90, vec3(0, 0, 1)), matrix);
	drawObj(3);

	//large blue triangle
	objID = 2;
	toGLFormat(mat4_translation(-0.25f, 0.1f, 0)*mat4_rotation(90, vec3(0, 0, 1)), matrix);
	drawObj(3);

	//small cyan triangle
	objID = 3;
	toGLFormat(mat4_translation(0.0f, -1.0f + sqrt(2.0f) / 4.0f, 0)*mat4_rotation(90, vec3(0, 0, 1)), matrix);
	drawObj(3);

	//small magenta triangle
	objID = 4;
	toGLFormat(mat4_translation(0, -1.0f + sqrt(2.0f)/4.0f, 0), matrix);
	drawObj(3);

	// yellow quad
	objID = 5; 
	toGLFormat(mat4_scale(vec3(1.0f, 0.5f, 0.5f))*mat4_translation(0.0f, 1.3f, 0.0f), matrix);
	drawObj(6);

	// orange parallelogram
	objID = 6; 
	toGLFormat(mat4_translation(0.0f, 0.9f, 0.0f)*mat4_rotation(90, vec3(0,0,1))*mat4_scale(vec3(0.5f,0.5f,0.5f)), matrix);
	drawObj(6);

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

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
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