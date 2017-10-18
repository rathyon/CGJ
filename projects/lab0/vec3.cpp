#include "vec3.h"
#include <math.h>  

vec3::vec3() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3::vec3(const float d) {
	x = d;
	y = d;
	z = d;
}

vec3::vec3(const float d1, const float d2, const float d3) {
	x = d1;
	y = d2;
	z = d3;
}

vec3::vec3(const vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

vec3::~vec3()
{
}

void vec3::operator=(const vec3 &v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

bool vec3::operator==(const vec3 &v) {
	if (abs(x - v.x) <= THRESHOLD && abs(y - v.y) <= THRESHOLD && abs(z - v.z) <= THRESHOLD)
		return true;
	else
		return false;
}

bool vec3::operator!=(const vec3 &v) {
	if (abs(x - v.x) > THRESHOLD || abs(y - v.y) > THRESHOLD || abs(z - v.z) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator <<(std::ostream &output, const vec3 &v) {
	output << "(" << v.x << "," << v.y << "," << v.z << ")";
	return output;
}

std::istream& operator >>(std::istream &input, const vec3 &v) {
	input >> v.x;
	input >> v.y;
	input >> v.z;
	return input;
}

vec3 vec3::operator +(const vec3& v) {
	vec3 vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;
	return vec;
}

vec3 vec3::operator -(const vec3& v) {
	vec3 vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;
	return vec;
}

vec3 vec3::operator *(const float d) {
	vec3 vec;
	vec.x = x * d;
	vec.y = y * d;
	vec.z = z * d;
	return vec;
}

vec3 vec3::operator /(const float d) {

	if (d == 0)
		throw "Error: cannot divide by zero.";

	vec3 vec;
	vec.x = x / d;
	vec.y = y / d;
	vec.z = z / d;
	return vec;
}

float vec3::quadrance() {
	return (x*x) + (y*y) + (z*z);
}

float vec3::norm() {
	return sqrt((x*x) + (y*y) + (z*z));
}

void vec3::normalize() {
	float norm = this->norm();
	x = x / norm;
	y = y / norm;
	z = z / norm;
}

float vec3::dot(const vec3& v) {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

vec3 vec3::cross(const vec3& v) {
	vec3 vec;
	vec.x = (y*v.z - z*v.y);
	vec.y = (z*v.x - x*v.z);
	vec.z = (x*v.y - y*v.x);
	return vec;
}
