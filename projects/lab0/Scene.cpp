#include "Scene.h"

Scene::Scene()
{
	root = new SceneNode();
	root->setMatrix(mat4_identity());
	root->setVisible(false); // TO AVOID RENDERING WHEN THERE'S NO MESH
}

Scene::Scene(std::string s)
{
	root = new SceneNode(s);
	root->setMatrix(mat4_identity());
	root->setVisible(false); // TO AVOID RENDERING WHEN THERE'S NO MESH
}



Scene::~Scene()
{
}


SceneNode* Scene::getRoot() {
	return root;
}
GLuint Scene::getShader() {
	return shader;
}
Camera* Scene::getCamera() {
	return camera;
}

void Scene::setRoot(SceneNode* n) {
	root = n;
}
void Scene::setShader(GLuint sh) {
	shader = sh;
}
void Scene::setCamera(Camera* c) {
	camera = c;
}

void Scene::add(SceneNode* node) {
	root->add(node);
}

void Scene::render() {
	glUseProgram(shader);

	float matrix[16];
	toGLFormat(camera->getProjectionMatrix(), matrix);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ProjectionMatrix"), 1, GL_TRUE, matrix);

	toGLFormat(camera->getViewMatrix(), matrix);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ViewMatrix"), 1, GL_TRUE, matrix);

	root->render();
}