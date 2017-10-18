#include "vec4.h"
#include <math.h>  

vec4::vec4() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

vec4::vec4(const float d) {
	x = d;
	y = d;
	z = d;
	w = d;
}

vec4::vec4(const float d1, const float d2, const float d3, const float d4) {
	x = d1;
	y = d2;
	z = d3;
	w = d4;
}

vec4::vec4(const vec4& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

vec4::~vec4()
{
}

void vec4::operator=(const vec4 &v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

bool vec4::operator==(const vec4 &v) {
	if (abs(x - v.x) <= THRESHOLD && abs(y - v.y) <= THRESHOLD && abs(z - v.z) <= THRESHOLD && abs(w - v.w) <= THRESHOLD)
		return true;
	else
		return false;
}

bool vec4::operator!=(const vec4 &v) {
	if (abs(x - v.x) > THRESHOLD || abs(y - v.y) > THRESHOLD || abs(z - v.z) > THRESHOLD || abs(w - v.w) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator <<(std::ostream &output, const vec4 &v) {
	output << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return output;
}

std::istream& operator >>(std::istream &input, const vec4 &v) {
	input >> v.x;
	input >> v.y;
	input >> v.z;
	input >> v.w;
	return input;
}

vec4 vec4::operator +(const vec4& v) {
	vec4 vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;
	vec.w = w + v.w;
	return vec;
}

vec4 vec4::operator -(const vec4& v) {
	vec4 vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;
	vec.w = w - v.w;
	return vec;
}

vec4 vec4::operator *(const float d) {
	vec4 vec;
	vec.x = x * d;
	vec.y = y * d;
	vec.z = z * d;
	vec.w = w * d;
	return vec;
}

vec4 vec4::operator /(const float d) {

	if (d == 0)
		throw "Error: cannot divide by zero.";

	vec4 vec;
	vec.x = x / d;
	vec.y = y / d;
	vec.z = z / d;
	vec.w = w / d;
	return vec;
}

float vec4::quadrance() {
	return (x*x) + (y*y) + (z*z) + (w*w);
}

float vec4::norm() {
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

void vec4::normalize() {
	float norm = this->norm();
	x = x / norm;
	y = y / norm;
	z = z / norm;
	w = w / norm;
}

float vec4::dot(const vec4& v) {
	return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
}

//vec4 vec4::cross(const vec4& v) {}
