#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

mat4 Camera::getProjectionMatrix(){
	return ProjectionMatrix;
}
mat4 Camera::getViewMatrix() {
	return ViewMatrix;
}

void Camera::setProjectionMatrix(mat4 m) {
	ProjectionMatrix = m;
}
void Camera::setViewMatrix(mat4 m) {
	ViewMatrix = m;
}
