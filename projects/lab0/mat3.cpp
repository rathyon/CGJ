#include "mat3.h"
#include "mat2.h"
#include <math.h>

#define PI 3.1415

mat3::mat3(){
	mat[0][0] = 0.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f;
	mat[1][0] = 0.0f; mat[1][1] = 0.0f; mat[1][2] = 0.0f;
	mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 0.0f;
}

mat3::mat3(float m0, float m1, float m2, 
		float m3, float m4, float m5,
		float m6, float m7, float m8) {

	mat[0][0] = m0; mat[0][1] = m1; mat[0][2] = m2;
	mat[1][0] = m3; mat[1][1] = m4; mat[1][2] = m5;
	mat[2][0] = m6; mat[2][1] = m7; mat[2][2] = m8;
}

mat3::mat3(mat2 m) {
	mat[0][0] = m[0][0]; mat[0][1] = m[0][1]; mat[0][2] = 0;
	mat[1][0] = m[1][0]; mat[1][1] = m[1][1]; mat[1][2] = 0;
	mat[2][0] = 0;	     mat[2][1] = 0;       mat[2][2] = 1;
}


mat3::~mat3()
{
}
mat3 mat3_identity() {
	mat3 mat;

	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;

	return mat;
}

mat3 mat3_scale(float x, float y, float z) {
	mat3 mat;

	mat[0][0] = x; mat[0][1] = 0; mat[0][2] = 0;
	mat[1][0] = 0; mat[1][1] = y; mat[1][2] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = z;

	return mat;
}

mat3 mat3_scale(vec3 v) {
	mat3 mat;

	mat[0][0] = v.x; mat[0][1] = 0; mat[0][2] = 0;
	mat[1][0] = 0; mat[1][1] = v.y; mat[1][2] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = v.z;

	return mat;
}

mat3 mat3_rotation(float angle, vec3 axis) {

	float theta = angle * (180 / (float)PI);

	mat3 I = mat3_identity();

	mat3 A = mat3( 0, -axis.z, axis.y,
				   axis.z, 0, -axis.x,
				   -axis.y, axis.x, 0);

	mat3 A2 = mat3( -(axis.z*axis.z) - (axis.y*axis.y), axis.x*axis.y, axis.x*axis.z,
					axis.x*axis.y, -(axis.z*axis.z) - (axis.x*axis.x), axis.y*axis.z,
					axis.x*axis.z, axis.y*axis.z, -(axis.y*axis.y) - (axis.x*axis.x));

	return I + A*sin(theta) + A2*(1 - cos(theta));
}

float* mat3::operator [](int i) {

	if (i > 2)
		throw "Index out of bounds";
	else
		return mat[i];
}

void mat3::operator= (const mat3 &m) {
	mat[0][0] = m.mat[0][0]; mat[0][1] = m.mat[0][1]; mat[0][2] = m.mat[0][2];
	mat[1][0] = m.mat[1][0]; mat[1][1] = m.mat[1][1]; mat[1][2] = m.mat[1][2];
	mat[2][0] = m.mat[2][0]; mat[2][1] = m.mat[2][1]; mat[2][2] = m.mat[2][2];
}

void mat3::operator= (const mat2 &m) {
	mat[0][0] = m.mat[0][0]; mat[0][1] = m.mat[0][1]; mat[0][2] = 0;
	mat[1][0] = m.mat[1][0]; mat[1][1] = m.mat[1][1]; mat[1][2] = 0;
	mat[2][0] = 0;			 mat[2][1] = 0;			  mat[2][2] = 0;
}

bool mat3::operator== (const mat3 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) <= THRESHOLD &&
		abs(mat[0][1] - m.mat[0][1]) <= THRESHOLD &&
		abs(mat[0][2] - m.mat[0][2]) <= THRESHOLD &&
		abs(mat[1][0] - m.mat[1][0]) <= THRESHOLD &&
		abs(mat[1][1] - m.mat[1][1]) <= THRESHOLD &&
		abs(mat[1][2] - m.mat[1][2]) <= THRESHOLD &&
		abs(mat[2][0] - m.mat[2][0]) <= THRESHOLD &&
		abs(mat[2][1] - m.mat[2][1]) <= THRESHOLD &&
		abs(mat[2][2] - m.mat[2][2]) <= THRESHOLD)
		return true;
	else
		return false;
}

bool mat3::operator!= (const mat3 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) > THRESHOLD ||
		abs(mat[0][1] - m.mat[0][1]) > THRESHOLD ||
		abs(mat[0][2] - m.mat[0][2]) > THRESHOLD ||
		abs(mat[1][0] - m.mat[1][0]) > THRESHOLD ||
		abs(mat[1][1] - m.mat[1][1]) > THRESHOLD ||
		abs(mat[1][2] - m.mat[1][2]) > THRESHOLD ||
		abs(mat[2][0] - m.mat[2][0]) > THRESHOLD ||
		abs(mat[2][1] - m.mat[2][1]) > THRESHOLD ||
		abs(mat[2][2] - m.mat[2][2]) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator << (std::ostream &output, const mat3 &m) {
	output << "[" << m.mat[0][0] << " " << m.mat[0][1] << " " << m.mat[0][2] << "]" << std::endl
		<< "[" << m.mat[1][0] << " " << m.mat[1][1] << " " << m.mat[1][2] << "]" << std::endl
		<< "[" << m.mat[2][0] << " " << m.mat[2][1] << " " << m.mat[2][2] << "]" << std::endl;
	return output;
}

std::istream& operator >> (std::istream& input, mat3 &m) {
	input >> m[0][0] >> m[0][1] >> m[0][2];
	input >> m[1][0] >> m[1][1] >> m[1][2];
	input >> m[2][0] >> m[2][1] >> m[2][2];
	return input;
}

mat3 mat3::operator+ (const mat3 &m) {
	mat3 res;
	res[0][0] = mat[0][0] + m.mat[0][0];
	res[0][1] = mat[0][1] + m.mat[0][1];
	res[0][2] = mat[0][2] + m.mat[0][2];

	res[1][0] = mat[1][0] + m.mat[1][0];
	res[1][1] = mat[1][1] + m.mat[1][1];
	res[1][2] = mat[1][2] + m.mat[1][2];

	res[2][0] = mat[2][0] + m.mat[2][0];
	res[2][1] = mat[2][1] + m.mat[2][1];
	res[2][2] = mat[2][2] + m.mat[2][2];

	return res;
}

mat3 mat3::operator- (const mat3 &m) {
	mat3 res;
	res[0][0] = mat[0][0] - m.mat[0][0];
	res[0][1] = mat[0][1] - m.mat[0][1];
	res[0][2] = mat[0][2] - m.mat[0][2];

	res[1][0] = mat[1][0] - m.mat[1][0];
	res[1][1] = mat[1][1] - m.mat[1][1];
	res[1][2] = mat[1][2] - m.mat[1][2];

	res[2][0] = mat[2][0] - m.mat[2][0];
	res[2][1] = mat[2][1] - m.mat[2][1];
	res[2][2] = mat[2][2] - m.mat[2][2];

	return res;
}

mat3 mat3::operator* (const float &f) {
	mat3 res;

	res[0][0] = mat[0][0] * f;
	res[0][1] = mat[0][1] * f;
	res[0][2] = mat[0][2] * f;

	res[1][0] = mat[1][0] * f;
	res[1][1] = mat[1][1] * f;
	res[1][2] = mat[1][2] * f;

	res[2][0] = mat[2][0] * f;
	res[2][1] = mat[2][1] * f;
	res[2][2] = mat[2][2] * f;

	return res;
}

vec3 mat3::operator* (const vec3 &v) {
	vec3 res;

	res.x = mat[0][0] * v.x + mat[1][0] * v.y + mat[2][0] * v.z;
	res.y = mat[0][1] * v.x + mat[1][1] * v.y + mat[2][1] * v.z;
	res.z = mat[0][2] * v.x + mat[1][2] * v.y + mat[2][2] * v.z;

	return res;
}

mat3 mat3::operator* (const mat3 &m) {
	mat3 res;

	res[0][0] = mat[0][0] * m.mat[0][0] + mat[0][1] * m.mat[1][0] + mat[0][2] * m.mat[2][0];
	res[0][1] = mat[0][0] * m.mat[0][1] + mat[0][1] * m.mat[1][1] + mat[0][2] * m.mat[2][1];
	res[0][2] = mat[0][0] * m.mat[0][2] + mat[0][1] * m.mat[1][2] + mat[0][2] * m.mat[2][2];

	res[1][0] = mat[1][0] * m.mat[0][0] + mat[1][1] * m.mat[1][0] + mat[1][2] * m.mat[2][0];
	res[1][1] = mat[1][0] * m.mat[0][1] + mat[1][1] * m.mat[1][1] + mat[1][2] * m.mat[2][1];
	res[1][2] = mat[1][0] * m.mat[0][2] + mat[1][1] * m.mat[1][2] + mat[1][2] * m.mat[2][2];

	res[2][0] = mat[2][0] * m.mat[0][0] + mat[2][1] * m.mat[1][0] + mat[2][2] * m.mat[2][0];
	res[2][1] = mat[2][0] * m.mat[0][1] + mat[2][1] * m.mat[1][1] + mat[2][2] * m.mat[2][1];
	res[2][2] = mat[2][0] * m.mat[0][2] + mat[2][1] * m.mat[1][2] + mat[2][2] * m.mat[2][2];


	return res;
}

void transpose(mat3 &m) {
	mat3 temp = m;

	m[1][0] = temp[0][1];
	m[0][1] = temp[1][0];
	m[2][0] = temp[0][2];
	m[0][2] = temp[2][0];
	m[2][1] = temp[1][2];
	m[1][2] = temp[2][1];
}

float mat3::determinant() {

	return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
		- mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
		+ mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

mat3 inverse(mat3 m) {

	//step 1: matrix of minors

	mat2 m11 = mat2(m[1][1], m[1][2], m[2][1], m[2][2]);
	mat2 m12 = mat2(m[1][0], m[1][2], m[2][0], m[2][2]);
	mat2 m13 = mat2(m[1][0], m[1][1], m[2][0], m[2][1]);

	mat2 m21 = mat2(m[0][1], m[0][2], m[2][1], m[2][2]);
	mat2 m22 = mat2(m[0][0], m[0][2], m[2][0], m[2][2]);
	mat2 m23 = mat2(m[0][0], m[0][1], m[2][0], m[2][1]);

	mat2 m31 = mat2(m[0][1], m[0][2], m[1][1], m[1][2]);
	mat2 m32 = mat2(m[0][0], m[0][2], m[1][0], m[1][2]);
	mat2 m33 = mat2(m[0][0], m[0][1], m[1][0], m[1][1]);

	mat3 minors = mat3(m11.determinant(), m12.determinant(), m13.determinant(),
		m21.determinant(), m22.determinant(), m23.determinant(),
		m31.determinant(), m32.determinant(), m33.determinant());

	//step 2: matrix of co factors

	minors[0][1] = -minors[0][1];
	minors[1][0] = -minors[1][0];
	minors[1][2] = -minors[1][2];
	minors[2][1] = -minors[2][1];

	//step 3: adjugate (?)

	transpose(minors);

	return minors*(1 / (m.determinant()));
}
