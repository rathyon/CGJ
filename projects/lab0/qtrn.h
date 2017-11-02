#pragma once
#include "vec3.h"
#include "mat4.h"

#define QTHRESHOLD 0.00001
#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

class qtrn
{
public:

	float t, x, y, z;

	qtrn();
	qtrn(float angle, vec3& axis);
	qtrn(float t1, float x1, float y1, float z1);
	~qtrn();

	friend void toAngleAxis(qtrn& q, float& angle, vec3& axis);
	friend void normalize(qtrn& q);
	friend qtrn conjugate(qtrn& q);
	friend qtrn inverse(qtrn& q);
	friend mat4 toMat4(qtrn& q);
	friend qtrn LERP(qtrn q1, qtrn q2, float f);
	friend qtrn SLERP(qtrn q1, qtrn q2, float f);

	qtrn operator +(qtrn& q);
	qtrn operator *(float f);
	qtrn operator *(qtrn& q);
	bool operator ==(qtrn& q);

	friend std::ostream& operator <<(std::ostream &output, qtrn& q);

	void clean();
	float quadrance();
	float norm();

};

