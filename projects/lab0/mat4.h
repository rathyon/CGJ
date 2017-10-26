#pragma once
#include <iostream>
#include "vec4.h"
#include "vec3.h"
#include "mat3.h"
#include "GL/glew.h"

#define THRESHOLD 0.00001f
#define PI 3.14159265358979323846f

class mat4 {

public:

	float mat[4][4];

	mat4();
	mat4(float, float, float, float,
		float, float, float, float,
		float, float, float, float,
		float, float, float, float);
	mat4(mat3); //conversion
	~mat4();

	//conversion
	friend mat3 toMat3(mat4);
	friend void toGLFormat(mat4, float[]); // do I have to turn to column major? opengl can do it for me

	//pre-set matrices
	friend mat4 mat4_identity();
	friend mat4 mat4_scale(float x, float y, float z);
	friend mat4 mat4_scale(vec3 v);
	friend mat4 mat4_rotation(float angle, vec3 axis); //vec3 or vec4? ?
	friend mat4 mat4_translation(float x, float y, float z);
	friend mat4 mat4_translation(vec3 v);
	friend mat4 lookAt(vec3 pos, vec3 look, vec3 up);
	friend mat4 perspective(float fov, float ratio, float near, float far);
	friend mat4 ortho(float left, float right, float bottom, float top, float near, float far);

	//subscript operator
	float* operator[](int); //returns the row: a float array

	void operator= (const mat4 &m);
	void operator= (const mat3 &m); //conversion
	bool operator== (const mat4 &m);
	bool operator!= (const mat4 &m);

	friend std::ostream& operator <<(std::ostream &output, const mat4 &m);
	friend std::istream& operator >>(std::istream &input, mat4 &m);

	//basic math operations

	mat4 operator+ (const mat4 &m);
	mat4 operator- (const mat4 &m);

	mat4 operator* (const float &f);
	vec4 operator* (const vec4 &v);
	mat4 operator* (const mat4 &m);

	friend void transpose(mat4 &m);
	float determinant();

	//friend mat4 inverse(const mat4 m);
};

