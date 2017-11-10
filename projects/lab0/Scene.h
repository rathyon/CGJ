#pragma once
#include "SceneNode.h"
#include "Camera.h"
#include <vector>
#include <string>
#include "GL/freeglut.h"
#include "GL/glew.h"

class Scene
{
private:

	SceneNode* root; // root of scenegraph

	GLuint shader;
	Camera* camera;

public:

	Scene();
	Scene(std::string);
	~Scene();

	SceneNode* getRoot();
	GLuint getShader();
	Camera* getCamera();

	void setRoot(SceneNode* n);
	void setShader(GLuint sh);
	void setCamera(Camera* c);

	void add(SceneNode*);
	void render();
};

