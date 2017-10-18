#pragma once
#include <iostream>

#define THRESHOLD 0.00001f

class vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	vec4();
	vec4(const float d);
	vec4(const float d1, const float d2, const float d3, const float d4);
	vec4(const vec4& v);
	~vec4();

	//assignment
	void operator= (const vec4 &v);

	//comparison
	bool operator ==(const vec4 &v);
	bool operator !=(const vec4 &v);

	//input/output
	friend std::ostream& operator <<(std::ostream &output, const vec4 &v);
	friend std::istream& operator >>(std::istream &input, const vec4 &v);

	//math operations
	vec4 operator +(const vec4& v);
	vec4 operator -(const vec4& v);
	vec4 operator *(const float d); //scalar
	vec4 operator /(const float d); //scalar

	float quadrance();
	float norm();
	void normalize();

	float dot(const vec4& v); //dot product
	//vec4 cross(const vec4&v); //cross product
};

