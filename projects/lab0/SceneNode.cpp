#include "SceneNode.h"

SceneNode::SceneNode()
{
	name = "";
}

SceneNode::SceneNode(std::string s)
{
	name = s;
}

SceneNode::~SceneNode()
{
}



void SceneNode::setParent(SceneNode* node) {
	parent = node;
}

void SceneNode::setName(std::string s){
	name = s;
}

void SceneNode::setMatrix(mat4 mat){
	ModelMatrix = mat;
}

void SceneNode::setMesh(Mesh m){
	mesh = m;
}

void SceneNode::setVisible(bool b) {
	visible = b;
}

void SceneNode::setShader(GLuint sh) {
	shader = sh;
}



SceneNode* SceneNode::getParent() {
	return parent;
}
std::string SceneNode::getName() {
	return name;
}
mat4 SceneNode::getMatrix() {
	return ModelMatrix;
}
Mesh SceneNode::getMesh() {
	return mesh;
}
GLuint SceneNode::getShader() {
	return shader;
}
bool SceneNode::isVisible() {
	return visible;
}



void SceneNode::add(SceneNode* node) {
	node->setParent(this);
	nodes.push_back(node);
}

mat4 SceneNode::parentTransforms() {

	mat4 res;
	if (parent) {
		res = parent->getMatrix() * parent->parentTransforms();
	}
	else {
		res = mat4_identity();
	}

	return res;

}

void SceneNode::render() {
	// Could call render with some parameters, like names of GLSL variables!
	/**/
	for(SceneNode* node : nodes){
		node->render();
	}
	if(visible){
		glBindVertexArray(mesh.getVAO());
		glUseProgram(shader);

		mat4 total_transforms;

		/**/
		SceneNode* node = parent;
		total_transforms = ModelMatrix;
		while(node){
			total_transforms = total_transforms * node->getMatrix(); // transforms order: ( 3rd * 2nd) * 1st -> for scale
			//total_transforms = node->getMatrix() * total_transforms; // transforms order: 3rd * (2nd * 1st) -> for rotation
			node = node->getParent();
		}
		/**/

		GLint loc;
		loc = glGetUniformLocation(shader, "color");
		glUniform4fv(loc, 1, mesh.getColor());

		float matrix[16];
		toGLFormat(total_transforms, matrix);
		glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMatrix"), 1, GL_TRUE, matrix);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
	}
	/**/
}
