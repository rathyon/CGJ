#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

// THESE MUST MATCH THE ORDER IN Shader.h
#define VERTICES 0
#define NORMALS 1
#define TEXCOORDS 2
#define COLORS 3

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLfloat u, v;
} Texcoord;

typedef struct {
	GLfloat nx, ny, nz;
} Normal;

class Mesh
{
private:
	GLuint VAO;
	GLuint Vertex_VBO;
	GLuint Texcoord_VBO;
	GLuint Normal_VBO;
	GLsizei vertexCount;

	//hard coded color just for this assignment
	float Color[4];

	std::vector <Vertex> Vertices, vertexData;
	std::vector <Texcoord> Texcoords, texcoordData;
	std::vector <Normal> Normals, normalData;

	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	//aux funcs
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);

public:
	Mesh();
	Mesh(std::string, float* color);
	~Mesh();

	GLuint getVAO();
	GLsizei getVertexCount();
	float* getColor();

	// real funcs
	bool load(std::string);
};

