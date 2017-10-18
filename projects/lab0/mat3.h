#pragma once
#include <iostream>
#include "vec3.h"
#include "mat2.h"

#define THRESHOLD 0.00001f

class mat3 {

public:

	float mat[3][3];

	mat3();
	mat3(float, float, float,
		float, float, float,
		float, float, float);
	mat3(mat2); //conversion
	~mat3();

	//pre-set matrices
	friend mat3 mat3_identity();
	friend mat3 mat3_scale(float x, float y, float z);
	friend mat3 mat3_scale(vec3 v);
	friend mat3 mat3_rotation(float angle, vec3 axis);

	//subscript operator
	float* operator[](int); //returns the row: a float array

	void operator= (const mat3 &m);
	void operator= (const mat2 &m); //conversion
	bool operator== (const mat3 &m);
	bool operator!= (const mat3 &m);

	friend std::ostream& operator <<(std::ostream &output, const mat3 &m);
	friend std::istream& operator >>(std::istream &input, mat3 &m);

	//basic math operations

	mat3 operator+ (const mat3 &m);
	mat3 operator- (const mat3 &m);

	mat3 operator* (const float &f);
	vec3 operator* (const vec3 &v);
	mat3 operator* (const mat3 &m);

	friend void transpose(mat3 &m);
	float determinant();

	friend mat3 inverse(const mat3 m);
};

