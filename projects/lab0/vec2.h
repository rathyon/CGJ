#pragma once
#include <iostream>

#define THRESHOLD 0.00001f

class vec2
{
public:

	float x;
	float y;

	vec2();
	vec2(const float d);
	vec2(const float d1, const float d2);
	vec2(const vec2& v);
	~vec2();

	//assignment
	void operator= (const vec2 &v);

	//comparison
	bool operator ==(const vec2 &v);
	bool operator !=(const vec2 &v);

	//input/output
	friend std::ostream& operator <<(std::ostream &output, const vec2 &v);
	friend std::istream& operator >>(std::istream &input, const vec2 &v);

	//math operations
	vec2 operator +(const vec2& v);
	vec2 operator -(const vec2& v);
	vec2 operator *(const float d); //scalar
	vec2 operator /(const float d); //scalar

	float quadrance();
	float norm();
	void normalize(); // destructive

	float dot(const vec2& v); //dot product
};

