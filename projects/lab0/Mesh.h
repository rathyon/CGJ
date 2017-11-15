#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define VERTICES 0
#define NORMALS 1
#define TEXCOORDS 2

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
	Mesh(std::string);
	~Mesh();

	GLuint getVAO();
	GLsizei getVertexCount();

	// real funcs
	bool load(std::string);
};

