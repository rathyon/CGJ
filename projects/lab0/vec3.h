#pragma once
#include <iostream>

#define THRESHOLD 0.00001f

class vec3
{
public:
	float x;
	float y;
	float z;

	vec3();
	vec3(const float d);
	vec3(const float d1, const float d2, const float d3);
	vec3(const vec3& v);
	~vec3();

	//assignment
	void operator= (const vec3 &v);

	//comparison
	bool operator ==(const vec3 &v);
	bool operator !=(const vec3 &v);

	//input/output
	friend std::ostream& operator <<(std::ostream &output, const vec3 &v);
	friend std::istream& operator >>(std::istream &input, const vec3 &v);

	//math operations
	vec3 operator +(const vec3& v);
	vec3 operator -(const vec3& v);
	vec3 operator *(const float d); //scalar
	vec3 operator /(const float d); //scalar

	float quadrance();
	float norm();
	void normalize();

	float dot(const vec3& v); //dot product
	vec3 cross(const vec3&v); //cross product
};

