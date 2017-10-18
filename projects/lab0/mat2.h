#pragma once
#include <iostream>
#include "vec2.h"

#define THRESHOLD 0.00001f

class mat2{

public:

	float mat[2][2];

	mat2();
	mat2(float, float, 
		 float, float);
	~mat2();

	//pre-set matrices
	friend mat2 mat2_identity();
	friend mat2 mat2_scale(float x, float y);
	friend mat2 mat2_scale(vec2 v);
	friend mat2 mat2_rotation(float angle);

	//subscript operator
	float* operator[](int); //returns the row: a float array

	void operator= (const mat2 &m);
	bool operator== (const mat2 &m);
	bool operator!= (const mat2 &m);

	friend std::ostream& operator <<(std::ostream &output, const mat2 &m);
	friend std::istream& operator >>(std::istream &input, const mat2 &m);

	//basic math operations

	mat2 operator+ (const mat2 &m);
	mat2 operator- (const mat2 &m);

	mat2 operator* (const float &f);
	vec2 operator* (const vec2 &v);
	mat2 operator* (const mat2 &m);

	friend void transpose(mat2 &m);
	float determinant();

	friend mat2 inverse(const mat2 m);
};

