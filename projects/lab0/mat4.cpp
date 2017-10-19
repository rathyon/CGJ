#include "mat4.h"
#include "mat3.h"
#include <math.h>

#define PI 3.1415

mat4::mat4() {
	mat[0][0] = 0.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f;
	mat[1][0] = 0.0f; mat[1][1] = 0.0f; mat[1][2] = 0.0f;
	mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 0.0f;
}

mat4::mat4(float m0, float m1, float m2, float m3, 
	float m4, float m5, float m6, float m7, 
	float m8, float m9, float m10, float m11,
	float m12, float m13, float m14, float m15) {

	mat[0][0] = m0; mat[0][1] = m1; mat[0][2] = m2; mat[0][3] = m3; 
	mat[1][0] = m4; mat[1][1] = m5; mat[1][2] = m6; mat[1][3] = m7; 
	mat[2][0] = m8; mat[2][1] = m9; mat[2][2] = m10; mat[2][3] = m11;
	mat[3][0] = m12; mat[3][1] = m13; mat[3][2] = m14; mat[3][3] = m15;
}

mat4::mat4(mat3 m) {
	mat[0][0] = m[0][0]; mat[0][1] = m[0][1]; mat[0][2] = m[0][2]; mat[0][3] = 0;
	mat[1][0] = m[1][0]; mat[1][1] = m[1][1]; mat[1][2] = m[1][2]; mat[1][3] = 0;
	mat[2][0] = m[2][0]; mat[2][1] = m[2][1]; mat[2][2] = m[2][2]; mat[2][3] = 0;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
}

mat3 toMat3(mat4 mat) {
	mat3 m;

	m[0][0] = mat[0][0]; m[0][1] = mat[0][1]; m[0][2] = mat[0][2];
	m[1][0] = mat[1][0]; m[1][1] = mat[1][1]; m[1][2] = mat[1][2];
	m[2][0] = mat[2][0]; m[2][1] = mat[2][1]; m[2][2] = mat[2][2];

	return m;

}

void toGLFormat(mat4 m, float mat[]) {
	mat[0] = m[0][0]; mat[1] = m[0][1]; mat[2] = m[0][2]; mat[3] = m[0][3];
	mat[4] = m[1][0]; mat[5] = m[1][1]; mat[6] = m[1][2]; mat[7] = m[1][3];
	mat[8] = m[2][0]; mat[9] = m[2][1]; mat[10] = m[2][2]; mat[11] = m[2][3];
	mat[12] = m[3][0]; mat[13] = m[3][1]; mat[14] = m[3][2]; mat[15] = m[3][3];
}


mat4::~mat4()
{
}
mat4 mat4_identity() {
	mat4 mat;

	mat[0][0] = 1.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f; mat[0][3] = 0.0f;
	mat[1][0] = 0.0f; mat[1][1] = 1.0f; mat[1][2] = 0.0f; mat[1][3] = 0.0f;
	mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 1.0f; mat[2][3] = 0.0f;
	mat[3][0] = 0.0f; mat[3][1] = 0.0f; mat[3][2] = 0.0f; mat[3][3] = 1.0f;

	return mat;
}

mat4 mat4_scale(float x, float y, float z) {
	mat4 mat;

	mat[0][0] = x; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;
	mat[1][0] = 0; mat[1][1] = y; mat[1][2] = 0; mat[1][3] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = z; mat[2][3] = 0;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;

	return mat;
}

mat4 mat4_scale(vec3 v) {
	mat4 mat;

	mat[0][0] = v.x; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;
	mat[1][0] = 0; mat[1][1] = v.y; mat[1][2] = 0; mat[1][3] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = v.z; mat[2][3] = 0;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;

	return mat;
}

mat4 mat4_rotation(float angle, vec3 a) {

	float theta = angle * ((float) PI / 180);

	mat3 I = mat3_identity();

	mat3 A = mat3(0, -a.z, a.y,
		a.z, 0, -a.x,
		-a.y, a.x, 0);

	mat3 A2 = A*A;
		
		/*= mat3(-(a.z*a.z) - (a.y*a.y), a.x*a.y, a.x*a.z,
		a.x*a.y, -(a.z*a.z) - (a.x*a.x), a.y*a.z,
		a.x*a.z, a.y*a.z, -(a.y*a.y) - (a.x*a.x));*/

	mat3 R = I + A*sin(theta) + A2*(1 - cos(theta));

	return mat4(R);
}

mat4 mat4_translation(float x, float y, float z) {
	mat4 mat;

	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = x;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = y;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = z;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;

	return mat;
}

mat4 mat4_translation(vec3 v) {
	mat4 mat;

	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = v.x;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = v.y;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = v.z;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;

	return mat;
}

float* mat4::operator [](int i) {

	if (i > 3)
		throw "Index out of bounds";
	else
		return mat[i];
}

void mat4::operator= (const mat4 &m) {
	mat[0][0] = m.mat[0][0];	mat[0][1] = m.mat[0][1];	mat[0][2] = m.mat[0][2];	mat[0][3] = m.mat[0][3];
	mat[1][0] = m.mat[1][0];	mat[1][1] = m.mat[1][1];	mat[1][2] = m.mat[1][2];	mat[1][3] = m.mat[1][3];
	mat[2][0] = m.mat[2][0];	mat[2][1] = m.mat[2][1];	mat[2][2] = m.mat[2][2];	mat[2][3] = m.mat[2][3];
	mat[3][0] = m.mat[3][0];	mat[3][1] = m.mat[3][1];	mat[3][2] = m.mat[3][2];	mat[3][3] = m.mat[3][3];
}

void mat4::operator= (const mat3 &m) {
	mat[0][0] = m.mat[0][0];	mat[0][1] = m.mat[0][1];	mat[0][2] = m.mat[0][2];	mat[0][3] = 0;
	mat[1][0] = m.mat[1][0];	mat[1][1] = m.mat[1][1];	mat[1][2] = m.mat[1][2];	mat[1][3] = 0;
	mat[2][0] = m.mat[2][0];	mat[2][1] = m.mat[2][1];	mat[2][2] = m.mat[2][2];	mat[2][3] = 0;
	mat[3][0] = 0;				mat[3][1] = 0;				mat[3][2] = 0;				mat[3][3] = 1;
}

bool mat4::operator== (const mat4 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) <= THRESHOLD &&
		abs(mat[0][1] - m.mat[0][1]) <= THRESHOLD &&
		abs(mat[0][2] - m.mat[0][2]) <= THRESHOLD &&
		abs(mat[0][3] - m.mat[0][3]) <= THRESHOLD &&

		abs(mat[1][0] - m.mat[1][0]) <= THRESHOLD &&
		abs(mat[1][1] - m.mat[1][1]) <= THRESHOLD &&
		abs(mat[1][2] - m.mat[1][2]) <= THRESHOLD &&
		abs(mat[1][3] - m.mat[1][3]) <= THRESHOLD &&

		abs(mat[2][0] - m.mat[2][0]) <= THRESHOLD &&
		abs(mat[2][1] - m.mat[2][1]) <= THRESHOLD &&
		abs(mat[2][2] - m.mat[2][2]) <= THRESHOLD &&
		abs(mat[2][3] - m.mat[2][3]) <= THRESHOLD &&

		abs(mat[3][0] - m.mat[3][0]) <= THRESHOLD &&
		abs(mat[3][1] - m.mat[3][1]) <= THRESHOLD &&
		abs(mat[3][2] - m.mat[3][2]) <= THRESHOLD &&
		abs(mat[3][3] - m.mat[3][3]) <= THRESHOLD)
		return true;
	else
		return false;
}

bool mat4::operator!= (const mat4 &m) {
	if (abs(mat[0][0] - m.mat[0][0]) > THRESHOLD ||
		abs(mat[0][1] - m.mat[0][1]) > THRESHOLD ||
		abs(mat[0][2] - m.mat[0][2]) > THRESHOLD ||
		abs(mat[0][3] - m.mat[0][3]) > THRESHOLD ||

		abs(mat[1][0] - m.mat[1][0]) > THRESHOLD ||
		abs(mat[1][1] - m.mat[1][1]) > THRESHOLD ||
		abs(mat[1][2] - m.mat[1][2]) > THRESHOLD ||
		abs(mat[1][3] - m.mat[1][3]) > THRESHOLD ||

		abs(mat[2][0] - m.mat[2][0]) > THRESHOLD ||
		abs(mat[2][1] - m.mat[2][1]) > THRESHOLD ||
		abs(mat[2][2] - m.mat[2][2]) > THRESHOLD ||
		abs(mat[2][3] - m.mat[2][3]) > THRESHOLD ||

		abs(mat[3][0] - m.mat[3][0]) > THRESHOLD ||
		abs(mat[3][1] - m.mat[3][1]) > THRESHOLD ||
		abs(mat[3][2] - m.mat[3][2]) > THRESHOLD ||
		abs(mat[3][3] - m.mat[3][3]) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator << (std::ostream &output, const mat4 &m) {
	output << "[" << roundf(m.mat[0][0]) << " " << roundf(m.mat[0][1]) << " " << roundf(m.mat[0][2]) << " " << roundf(m.mat[0][3]) << "]" << std::endl
		<< "[" << roundf(m.mat[1][0]) << " " << roundf(m.mat[1][1]) << " " << roundf(m.mat[1][2]) << " " << roundf(m.mat[1][3]) << "]" << std::endl
		<< "[" << roundf(m.mat[2][0]) << " " << roundf(m.mat[2][1]) << " " << roundf(m.mat[2][2]) << " " << roundf(m.mat[2][3]) << "]" << std::endl
		<< "[" << roundf(m.mat[3][0]) << " " << roundf(m.mat[3][1]) << " " << roundf(m.mat[3][2]) << " " << roundf(m.mat[3][3]) << "]" << std::endl;

	return output;
}

std::istream& operator >> (std::istream& input, mat4 &m) {
	input >> m[0][0] >> m[0][1] >> m[0][2] >> m[0][3];
	input >> m[1][0] >> m[1][1] >> m[1][2] >> m[1][3];
	input >> m[2][0] >> m[2][1] >> m[2][2] >> m[2][3];
	input >> m[3][0] >> m[3][1] >> m[3][2] >> m[3][3];
	return input;
}

mat4 mat4::operator+ (const mat4 &m) {
	mat4 res;
	res[0][0] = mat[0][0] + m.mat[0][0];
	res[0][1] = mat[0][1] + m.mat[0][1];
	res[0][2] = mat[0][2] + m.mat[0][2];
	res[0][3] = mat[0][3] + m.mat[0][3];

	res[1][0] = mat[1][0] + m.mat[1][0];
	res[1][1] = mat[1][1] + m.mat[1][1];
	res[1][2] = mat[1][2] + m.mat[1][2];
	res[1][3] = mat[1][3] + m.mat[1][3];

	res[2][0] = mat[2][0] + m.mat[2][0];
	res[2][1] = mat[2][1] + m.mat[2][1];
	res[2][2] = mat[2][2] + m.mat[2][2];
	res[2][3] = mat[2][3] + m.mat[2][3];

	res[3][0] = mat[3][0] + m.mat[3][0];
	res[3][1] = mat[3][1] + m.mat[3][1];
	res[3][2] = mat[3][2] + m.mat[3][2];
	res[3][3] = mat[3][3] + m.mat[3][3];

	return res;
}

mat4 mat4::operator- (const mat4 &m) {
	mat4 res;
	res[0][0] = mat[0][0] - m.mat[0][0];
	res[0][1] = mat[0][1] - m.mat[0][1];
	res[0][2] = mat[0][2] - m.mat[0][2];
	res[0][3] = mat[0][3] - m.mat[0][3];

	res[1][0] = mat[1][0] - m.mat[1][0];
	res[1][1] = mat[1][1] - m.mat[1][1];
	res[1][2] = mat[1][2] - m.mat[1][2];
	res[1][3] = mat[1][3] - m.mat[1][3];

	res[2][0] = mat[2][0] - m.mat[2][0];
	res[2][1] = mat[2][1] - m.mat[2][1];
	res[2][2] = mat[2][2] - m.mat[2][2];
	res[2][3] = mat[2][3] - m.mat[2][3];

	res[3][0] = mat[3][0] - m.mat[3][0];
	res[3][1] = mat[3][1] - m.mat[3][1];
	res[3][2] = mat[3][2] - m.mat[3][2];
	res[3][3] = mat[3][3] - m.mat[3][3];

	return res;
}

mat4 mat4::operator* (const float &f) {
	mat4 res;

	res[0][0] = mat[0][0] * f;
	res[0][1] = mat[0][1] * f;
	res[0][2] = mat[0][2] * f;
	res[0][3] = mat[0][3] * f;

	res[1][0] = mat[1][0] * f;
	res[1][1] = mat[1][1] * f;
	res[1][2] = mat[1][2] * f;
	res[1][3] = mat[0][3] * f;

	res[2][0] = mat[2][0] * f;
	res[2][1] = mat[2][1] * f;
	res[2][2] = mat[2][2] * f;
	res[2][3] = mat[0][3] * f;

	res[3][0] = mat[3][0] * f;
	res[3][1] = mat[3][1] * f;
	res[3][2] = mat[3][2] * f;
	res[3][3] = mat[3][3] * f;

	return res;
}

vec4 mat4::operator* (const vec4 &v) {
	vec4 res;

	res.x = mat[0][0] * v.x + mat[1][0] * v.y + mat[2][0] * v.z + mat[3][0] * v.w;
	res.y = mat[0][1] * v.x + mat[1][1] * v.y + mat[2][1] * v.z + mat[3][1] * v.w;
	res.z = mat[0][2] * v.x + mat[1][2] * v.y + mat[2][2] * v.z + mat[3][2] * v.w;
	res.z = mat[0][3] * v.x + mat[1][3] * v.y + mat[2][3] * v.z + mat[3][3] * v.w;

	return res;
}

mat4 mat4::operator* (const mat4 &m) {
	mat4 res;

	res[0][0] = mat[0][0] * m.mat[0][0] + mat[0][1] * m.mat[1][0] + mat[0][2] * m.mat[2][0] + mat[0][3] * m.mat[3][0];
	res[0][1] = mat[0][0] * m.mat[0][1] + mat[0][1] * m.mat[1][1] + mat[0][2] * m.mat[2][1] + mat[0][3] * m.mat[3][1];
	res[0][2] = mat[0][0] * m.mat[0][2] + mat[0][1] * m.mat[1][2] + mat[0][2] * m.mat[2][2] + mat[0][3] * m.mat[3][2];
	res[0][3] = mat[0][0] * m.mat[0][3] + mat[0][1] * m.mat[1][3] + mat[0][2] * m.mat[2][3] + mat[0][3] * m.mat[3][3];

	res[1][0] = mat[1][0] * m.mat[0][0] + mat[1][1] * m.mat[1][0] + mat[1][2] * m.mat[2][0] + mat[1][3] * m.mat[3][0];
	res[1][1] = mat[1][0] * m.mat[0][1] + mat[1][1] * m.mat[1][1] + mat[1][2] * m.mat[2][1] + mat[1][3] * m.mat[3][1];
	res[1][2] = mat[1][0] * m.mat[0][2] + mat[1][1] * m.mat[1][2] + mat[1][2] * m.mat[2][2] + mat[1][3] * m.mat[3][2];
	res[1][3] = mat[1][0] * m.mat[0][3] + mat[1][1] * m.mat[1][3] + mat[1][2] * m.mat[2][3] + mat[1][3] * m.mat[3][3];

	res[2][0] = mat[2][0] * m.mat[0][0] + mat[2][1] * m.mat[1][0] + mat[2][2] * m.mat[2][0] + mat[2][3] * m.mat[3][0];
	res[2][1] = mat[2][0] * m.mat[0][1] + mat[2][1] * m.mat[1][1] + mat[2][2] * m.mat[2][1] + mat[2][3] * m.mat[3][1];
	res[2][2] = mat[2][0] * m.mat[0][2] + mat[2][1] * m.mat[1][2] + mat[2][2] * m.mat[2][2] + mat[2][3] * m.mat[3][2];
	res[2][3] = mat[2][0] * m.mat[0][3] + mat[2][1] * m.mat[1][3] + mat[2][2] * m.mat[2][3] + mat[2][3] * m.mat[3][3];

	res[3][0] = mat[3][0] * m.mat[0][0] + mat[3][1] * m.mat[1][0] + mat[3][2] * m.mat[2][0] + mat[3][3] * m.mat[3][0];
	res[3][1] = mat[3][0] * m.mat[0][1] + mat[3][1] * m.mat[1][1] + mat[3][2] * m.mat[2][1] + mat[3][3] * m.mat[3][1];
	res[3][2] = mat[3][0] * m.mat[0][2] + mat[3][1] * m.mat[1][2] + mat[3][2] * m.mat[2][2] + mat[3][3] * m.mat[3][2];
	res[3][3] = mat[3][0] * m.mat[0][3] + mat[3][1] * m.mat[1][3] + mat[3][2] * m.mat[2][3] + mat[3][3] * m.mat[3][3];


	return res;
}

void transpose(mat4 &m) {
	mat4 temp = m;

	m[1][0] = temp[0][1];
	m[0][1] = temp[1][0];

	m[2][0] = temp[0][2];
	m[0][2] = temp[2][0];

	m[3][0] = temp[0][3];
	m[0][3] = temp[3][0];

	m[2][1] = temp[1][2];
	m[1][2] = temp[2][1];

	m[3][1] = temp[1][3];
	m[3][2] = temp[3][1];

	m[3][2] = temp[2][3];
	m[2][3] = temp[3][2];
}

float mat4::determinant() {

	return mat[0][0] * (mat3(mat[1][1], mat[1][2], mat[1][3],
		mat[2][1], mat[2][2], mat[2][3],
		mat[3][1], mat[3][2], mat[3][3]).determinant())

		- mat[0][1] * (mat3(mat[1][0], mat[1][2], mat[1][3],
			mat[2][0], mat[2][2], mat[2][3],
			mat[3][0], mat[3][2], mat[3][3]).determinant())

		+ mat[0][2] * (mat3(mat[1][0], mat[1][1], mat[1][3],
			mat[2][0], mat[2][1], mat[2][3],
			mat[3][0], mat[3][1], mat[3][3]).determinant())
		- mat[0][3] * (mat3(mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2],
			mat[3][0], mat[3][1], mat[3][2]).determinant());
}

