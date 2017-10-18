#include "vec2.h"
#include <math.h>  

vec2::vec2(){
	x = 0.0f;
	y = 0.0f;
}

vec2::vec2(const float d) {
	x = d;
	y = d;
}

vec2::vec2(const float d1, const float d2) {
	x = d1;
	y = d2;
}

vec2::vec2(const vec2& v) {
	x = v.x;
	y = v.y;
}

vec2::~vec2()
{
}

void vec2::operator=(const vec2 &v) {
	x = v.x;
	y = v.y;
}

bool vec2::operator==(const vec2 &v) {
	if (abs(x - v.x) <= THRESHOLD && abs(y - v.y) <= THRESHOLD)
		return true;
	else
		return false;
}

bool vec2::operator!=(const vec2 &v) {
	if (abs(x - v.x) > THRESHOLD || abs(y - v.y) > THRESHOLD)
		return true;
	else
		return false;
}

std::ostream& operator <<(std::ostream &output, const vec2 &v) {
	output << "(" << v.x << "," << v.y << ")";
	return output;
}

std::istream& operator >>(std::istream &input, const vec2 &v) {
	input >> v.x;
	input >> v.y;
	return input;
}

vec2 vec2::operator +(const vec2& v) {
	vec2 vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	return vec;
}

vec2 vec2::operator -(const vec2& v) {
	vec2 vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	return vec;
}

vec2 vec2::operator *(const float d) {
	vec2 vec;
	vec.x = x * d;
	vec.y = y * d;
	return vec;
}

vec2 vec2::operator /(const float d) {

	if (d == 0)
		throw "Error: cannot divide by zero.";

	vec2 vec;
	vec.x = x / d;
	vec.y = y / d;
	return vec;
}

float vec2::quadrance() {
	return (x*x) + (y*y);
}

float vec2::norm() {
	return sqrt((x*x) + (y*y));
}

void vec2::normalize() {
	float norm = this->norm();
	x = x / norm;
	y = y / norm;
}

float vec2::dot(const vec2& v) {
	return (x * v.x) + (y * v.y);
}