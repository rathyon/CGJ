#include "qtrn.h"



qtrn::qtrn()
{
}

qtrn::qtrn(float theta, vec3& axis)
{

	float axis_length = axis.norm();
	vec3 norm_axis = vec3(axis.x / axis_length, axis.y / axis_length, axis.z / axis_length);

	float a = theta * (float)DEGREES_TO_RADIANS;
	t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	x = norm_axis.x * s;
	y = norm_axis.y * s;
	z = norm_axis.z * s;
	 
	//this->clean();
	//normalize(*this);

	//normalize
	s = 1.0f / (sqrt(t*t + x*x + y*y + z*z));

	t *= s;
	x *= s;
	y *= s;
	z *= s;
}

qtrn::qtrn(float t1, float x1, float y1, float z1) {
	t = t1;
	x = x1;
	y = y1;
	z = z1;
}

qtrn::~qtrn()
{
}

void normalize(qtrn& q) {

	float s = 1.0f / q.norm();
	q.t *= s;
	q.x *= s;
	q.y *= s;
	q.z *= s;

}

void qtrn::clean() {
	if (fabs(t) < QTHRESHOLD) t = 0.0f;
	if (fabs(x) < QTHRESHOLD) x = 0.0f;
	if (fabs(y) < QTHRESHOLD) y = 0.0f;
	if (fabs(z) < QTHRESHOLD) z = 0.0f;
}

float qtrn::quadrance() {
	return t*t + x*x + y*y + z*z;
}

float qtrn::norm() {
	return sqrt(t*t + x*x + y*y + z*z);
}

qtrn qtrn::operator *(float f) {

	qtrn sq;
	sq.t = f * t;
	sq.x = f * x;
	sq.y = f * y;
	sq.z = f * z;
	return sq;

}

qtrn qtrn::operator *(qtrn& q) {

	qtrn res;
	res.t = t * q.t - x * q.x - y * q.y - z * q.z;
	res.x = t * q.x + x * q.t + y * q.z - z * q.y;
	res.y = t * q.y + y * q.t + z * q.x - x * q.z;
	res.z = t * q.z + z * q.t + x * q.y - y * q.x;
	return res;

}

qtrn qtrn::operator +(qtrn& q) {
	qtrn res;

	res.t = t + q.t;
	res.x = x + q.x;
	res.y = y + q.y;
	res.z = z + q.z;

	return res;
}

bool qtrn::operator ==(qtrn& q) {

	return (fabs(t - q.t) < QTHRESHOLD && fabs(x - q.x) < QTHRESHOLD &&
		fabs(y - q.y) < QTHRESHOLD && fabs(z - q.z) < QTHRESHOLD);
}

void toAngleAxis(qtrn& q, float& angle, vec3& axis) {

	normalize(q);
	angle = 2.0f * acos(q.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - q.t*q.t);
	if (s < QTHRESHOLD) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
	}
	else {
		axis.x = q.x / s;
		axis.y = q.y / s;
		axis.z = q.z / s;
	}

}

qtrn conjugate(qtrn& q) {
	qtrn res = qtrn(q.t, -q.x, -q.y, -q.z);
	return res;
}

qtrn inverse(qtrn& q) {

	return conjugate(q) * (1.0f / q.quadrance());

}

mat4 toMat4(qtrn& q) {

	normalize(q);

	mat4 res;

	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xt = q.x * q.t;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yt = q.y * q.t;
	float zz = q.z * q.z;
	float zt = q.z * q.t;

	res[0][0] = 1.0f - 2.0f * (yy + zz);
	res[0][1] = 2.0f * (xy + zt);
	res[0][2] = 2.0f * (xz - yt);
	res[0][3] = 0.0f;

	res[1][0] = 2.0f * (xy - zt);
	res[1][1] = 1.0f - 2.0f * (xx + zz);
	res[1][2] = 2.0f * (yz + xt);
	res[1][3] = 0.0f;

	res[2][0] = 2.0f * (xz + yt);
	res[2][1] = 2.0f * (yz - xt);
	res[2][2] = 1.0f - 2.0f * (xx + yy);
	res[2][3] = 0.0f;

	res[3][0] = 0.0f;
	res[3][1] = 0.0f;
	res[3][2] = 0.0f;
	res[3][3] = 1.0f;

	transpose(res); // to compensate for teacher's implementation xD

	return res;

}

qtrn LERP(qtrn q1, qtrn q2, float f) {

	float cos_angle = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.t*q2.t;
	float k0 = 1.0f - f;
	float k1 = (cos_angle > 0) ? f : -f;
	qtrn qi = (q1*k0) + (q2*k1);
	normalize(qi);
	return qi;
}

qtrn SLERP(qtrn q1, qtrn q2, float f) {

	float angle = acos(q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.t*q2.t);
	float k0 = sin((1 - f)*angle) / sin(angle);
	float k1 = sin(f*angle) / sin(angle);
	qtrn qi = (q1*k0) + (q2*k1);
	normalize(qi);
	return qi;

}

std::ostream& operator <<(std::ostream &output, qtrn& q) {

	output << "qtrn: (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;

	float thetaf;
	vec3 axis_f;
	toAngleAxis(q, thetaf, axis_f);

	output << "Angle = " << thetaf << std::endl;
	output << "Axis = " << axis_f << std::endl;

	return output;

}
