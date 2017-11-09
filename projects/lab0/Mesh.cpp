#include "Mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


void Mesh::setVAO(GLuint vao){
	VAO = vao;
}
void Mesh::setVertexCount(int vc) {
	vertexCount = vc;
}

GLuint Mesh::getVAO() {
	return VAO;
}
int Mesh::getVertexCount() {
	return vertexCount;
}