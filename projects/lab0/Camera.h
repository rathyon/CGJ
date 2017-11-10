#pragma once
#include "mat4.h"

class Camera
{
private:

	mat4 ProjectionMatrix;
	mat4 ViewMatrix;

public:
	Camera();
	~Camera();

	mat4 getProjectionMatrix();
	mat4 getViewMatrix();

	void setProjectionMatrix(mat4 m);
	void setViewMatrix(mat4 m);
};

