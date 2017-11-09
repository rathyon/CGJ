#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

class Mesh
{
private:
	GLuint VAO;
	int vertexCount;

public:
	Mesh();
	~Mesh();

	void setVAO(GLuint vao);
	void setVertexCount(int vc);

	GLuint getVAO();
	int getVertexCount();
};

