#include "mat2.h"
#include <math.h>

#define PI 3.1415

mat2::mat2(){
	mat[0][0] = 0.0f; mat[0][1] = 0.0f;
	mat[1][0] = 0.0f; mat[1][1] = 0.0f;
}

mat2::mat2(float m0, float m1,
		   float m2, float m3) {

	mat[0][0] = m0; mat[0][1] = m1;
	mat[1][0] = m2; mat[1][1] = m3;
}


mat2::~mat2()
{
}

mat2 mat2_identity() {
	mat2 m;
	m[0][0] = 1; m[0][1] = 0;
	m[1][0] = 0; m[1][1] = 1;
	return m;
}

mat2 mat2_scale(float x, float y) {
	mat2 m;
	m[0][0] = x; m[0][1] = 0;
	m[1][0] = 0; m[1][1] = y;
	return m;
}

mat2 mat2_scale(vec2 v) {
	mat2 m;
	m[0][0] = v.x; m[0][1] = 0;
	m[1][0] = 0; m[1][1] = v.y;
	return m;
}

mat2 mat2_rotation(float angle) {
	mat2 m;

	float theta = angle * (180 / (float) PI);

	m[0][0] = cos(theta); m[0][1] = -sin(theta);
	m[1][0] = sin(theta); m[1][1] = cos(theta);
	return m;
}

float* mat2::operator [](int i) {

	if (i > 1)
		throw "Index out of bounds";
	else
		return mat[i];
}

void mat2::operator= (const mat2 &m) {
	mat[0][0] = m.mat[0][0]; mat[0][1] = m.mat[0][1];
	mat[1][0] = m.mat[1][0]; mat[1][1] = m.mat[1][1];
}

bool mat2::operator== (const mat2 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) <= THRESHOLD &&
		abs(mat[0][1] - m.mat[0][1]) <= THRESHOLD &&
		abs(mat[1][0] - m.mat[1][0]) <= THRESHOLD &&
		abs(mat[1][1] - m.mat[1][1]) <= THRESHOLD)
		return true;
	else
		return false;
}

bool mat2::operator!= (const mat2 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) > THRESHOLD ||
		abs(mat[0][1] - m.mat[0][1]) > THRESHOLD ||
		abs(mat[1][0] - m.mat[1][0]) > THRESHOLD ||
		abs(mat[1][1] - m.mat[1][1]) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator << (std::ostream &output, const mat2 &m) {
	output << "[" << m.mat[0][0] << " " << m.mat[0][1] << "]" << std::endl 
		<< "[" << m.mat[1][0] << " " << m.mat[1][1] << "]" << std::endl;
	return output;
}

std::istream& operator >> (std::istream &input, const mat2 &m) {
	input >> m.mat[0][0] >> m.mat[0][1];
	input >> m.mat[1][0] >> m.mat[1][1];
	return input;
}

mat2 mat2::operator+ (const mat2 &m) {
	mat2 res;
	res[0][0] = mat[0][0] + m.mat[0][0]; 
	res[0][1] = mat[0][1] + m.mat[0][1];
	res[1][0] = mat[1][0] + m.mat[1][0];
	res[1][1] = mat[1][1] + m.mat[1][1];

	return res;
}

mat2 mat2::operator- (const mat2 &m) {
	mat2 res;
	res[0][0] = mat[0][0] - m.mat[0][0];
	res[0][1] = mat[0][1] - m.mat[0][1];
	res[1][0] = mat[1][0] - m.mat[1][0];
	res[1][1] = mat[1][1] - m.mat[1][1];

	return res;
}

mat2 mat2::operator* (const float &f) {
	mat2 res;

	res[0][0] = mat[0][0] * f;
	res[0][1] = mat[0][1] * f;
	res[1][0] = mat[1][0] * f;
	res[1][1] = mat[1][1] * f;

	return res;
}

vec2 mat2::operator* (const vec2 &v) {
	vec2 res;

	res.x = mat[0][0] * v.x + mat[1][0] * v.y;
	res.y = mat[0][1] * v.x + mat[1][1] * v.y;

	return res;
}

mat2 mat2::operator* (const mat2 &m) {
	mat2 res;

	res[0][0] = mat[0][0] * m.mat[0][0] + mat[1][0] * m.mat[0][1];
	res[0][1] = mat[0][0] * m.mat[1][0] + mat[1][0] * m.mat[1][1];

	res[1][0] = mat[0][1] * m.mat[0][0] + mat[1][1] * m.mat[0][1];
	res[1][1] = mat[0][1] * m.mat[1][0] + mat[1][1] * m.mat[1][1];

	return res;
}

void transpose(mat2 &m) {
	mat2 temp = m;

	m[1][0] = temp[0][1];
	m[0][1] = temp[1][0];
}

float mat2::determinant() {
	
	return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

mat2 inverse(mat2 m) {
	mat2 res = mat2(m[1][1], -m[0][1], -m[1][0], m[0][0]);
	return res * m.determinant();
}