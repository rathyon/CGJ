#pragma once
#include <vector>
#include <string>
#include "mat4.h"
#include "Mesh.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

class SceneNode
{
private:
	std::vector<SceneNode*> nodes;
	SceneNode* parent;

	std::string name;

	mat4 ModelMatrix;
	Mesh mesh;

	GLuint shader;

	bool visible = true;

	mat4 parentTransforms();

public:
	SceneNode();
	SceneNode(std::string);
	~SceneNode();

	//setters
	void setParent(SceneNode* node);
	void setName(std::string name);
	void setMatrix(mat4 mat);
	void setMesh(Mesh mesh);
	void setVisible(bool b);
	void setShader(GLuint shader);

	//getters
	SceneNode* getParent();
	std::string getName();
	mat4 getMatrix();
	Mesh getMesh();
	bool isVisible();

	//bool isTranslucid -> might come in handy?

	GLuint getShader();

	void add(SceneNode* node);
	void render();
};

