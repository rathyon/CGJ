#include <iostream>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"

#define PRINT(s) std::cout << s << std::endl

int test() {

	//-----------------------------[ 2x2 VECTOR TESTING ]-----------------------------//

	vec2 tv = vec2(1, 1);
	vec2 res;

	//assignment
	tv = vec2(3, 2);
	if (tv.x != 3 || tv.y != 2)
		PRINT("test failed");

	//comparison
	if(tv != vec2(3,2))
		PRINT("test failed");
	else if(tv == vec2(0))
		PRINT("test failed");

	//input/output
	PRINT(tv);
	// how to test input?

	tv = vec2(1);

	//basic math operations
	res = vec2(3, 2) + vec2(1, 2);
	if(res != vec2(4))
		PRINT("test failed");

	res = vec2(5) - vec2(1);
	if(res != vec2(4))
		PRINT("test failed");

	res = vec2(2) * 2;
	if(res != vec2(4))
		PRINT("test failed");

	res = vec2(8) / 2;
	if(res != vec2(4))
		PRINT("test failed");

	try {
		res = vec2(8) / 0;
		PRINT("test failed");
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		PRINT("Exception caught: test success!");
	}

	//aux math operations
	if(tv.quadrance() != 2)
		PRINT("test failed");

	if(vec2(3,4).norm() != 5)
		PRINT("test failed");

	tv.normalize();

	if(tv != vec2(1/sqrt(2), 1/sqrt(2)))
		PRINT("test failed");

	// dot product
	tv = vec2(2);
	vec2 tv2 = vec2(4);

	if(tv.dot(tv2) != 16)
		PRINT("test failed");

	//-----------------------------[ 3x3 VECTOR TESTING ]-----------------------------//

	vec3 v3 = vec3(1);
	vec3 v3result;

	//assignment
	v3 = vec3(2, 2, 6);
	if (v3.x != 2 || v3.y != 2 || v3.z != 6)
		PRINT("test failed");

	//comparison
	if(v3 == vec3(1))
		PRINT("test failed");

	if(v3 != vec3(2,2,6))
		PRINT("test failed");

	//input/output
	PRINT(v3);
	// how to test input?

	//basic math operations
	if(v3 + vec3(4,4,0) != vec3(6))
		PRINT("v3 add failed");

	if(v3 - vec3(0,0,4) != vec3(2))
		PRINT("v3 sub failed");

	if(v3 * 2 != vec3(4,4,12))
		PRINT("v3 mul scalar failed");

	if(v3 / 2 != vec3(1,1,3))
		PRINT("v3 div failed");

	try {
		v3result = v3 / 0;
		PRINT("test failed");
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		PRINT("Exception caught: test success!");
	}

	//aux math operations

	v3 = vec3(2, 2, 3);

	if(v3.quadrance() != 17)
		PRINT("quadrance failed");

	v3 = vec3(0, 3, 4);
	if(v3.norm() != 5)
		PRINT("norm failed");

	v3 = vec3(1, 1, 1);
	v3.normalize();
	if(v3 != vec3( 1/sqrt(3), 1/sqrt(3), 1/sqrt(3)) )
		PRINT("normalize failed");

	// dot and cross products

	v3 = vec3(3);

	if(v3.dot(vec3(2)) != 18)
		PRINT("dot failed");

	if(vec3(0,1,0).cross(vec3(0,0,1)) != vec3(1,0,0))
		PRINT("cross failed");

	//-------------------------------[ 2x2 MATRIX TESTING ]-------------------------------//

	/*mat2 m2t = mat2_identity();
	PRINT(m2t);

	m2t = mat2(1,2,3,4);
	PRINT(m2t);

	m2t = mat2_scale(2 ,3);
	PRINT(m2t);

	m2t = mat2_scale(vec2(2,3));
	PRINT(m2t);

	m2t = mat2_rotation(90);
	PRINT(m2t);

	m2t = mat2_identity();

	if (m2t != mat2_identity())
		PRINT("mat2 comparison fail");

	if( m2t == mat2_scale(3,9))
		PRINT("mat2 comparison fail");

	PRINT(m2t + m2t);
	PRINT(m2t - m2t);
	PRINT(m2t * 6);
	PRINT(m2t * vec2(3,3));
	PRINT(m2t * m2t);
	m2t = mat2_rotation(90);
	transpose(m2t);
	PRINT(m2t);
	PRINT(mat2_identity().determinant());
	PRINT(inverse(mat2_identity()));*/


	//-------------------------------[ 3x3 MATRIX TESTING ]-------------------------------//

	mat3 m3t = mat3_identity();
	PRINT(m3t);
	/*
	m3t = mat3(1,2,3,4,5,6,7,8,9);
	PRINT(m3t);

	m3t = mat3(mat2(1,2,3,4));
	PRINT(m3t);

	m3t = mat2(1, 2, 3, 4);
	PRINT(m3t);

	m3t = mat3_scale(2, 3, 2);
	PRINT(m3t);

	m3t = mat3_scale(vec3(2, 3, 2));
	PRINT(m3t);

	m3t = mat3_rotation(90, vec3(1,0,0));
	PRINT(m3t);

	m3t = mat3_identity();

	if (m3t != mat3_identity())
		PRINT("mat3 comparison fail");

	if (m3t == mat3_scale(3, 9, 3))
		PRINT("mat3 comparison fail");


	PRINT(m3t + m3t);
	PRINT(m3t - m3t);
	PRINT(m3t * 6);
	PRINT(m3t * vec3(3, 3, 3));
	PRINT(m3t * m3t);
	m3t = mat3_rotation(90, vec3(1,0,0));
	transpose(m3t);
	PRINT(m3t);
	PRINT(mat3_identity().determinant());
	PRINT(inverse(mat3(1,2,3,0,1,4,5,6,0)));*/

	//-------------------------------[ 4x4 MATRIX TESTING ]-------------------------------//

	/*mat4 m4t = mat4_identity();
	PRINT(m4t);
	
	m4t = mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	PRINT(m4t);

	m4t = mat4(mat3(1,2,3,4,5,6,7,8,9));
	PRINT(m4t);

	m4t = mat3(1, 2, 3, 4,5,6,7,8,9);
	PRINT(m4t);

	m4t = mat3(mat2(1, 2, 3, 4));
	PRINT(m4t);

	m4t = mat4_scale(2, 3, 2);
	PRINT(m4t);

	m4t = mat4_scale(vec3(2, 3, 2));
	PRINT(m4t);

	m4t = mat4_rotation(90, vec3(1,0,0));
	PRINT(m4t);

	m4t = mat4_identity();

	if (m4t != mat4_identity())
	PRINT("mat4 comparison fail");

	if (m4t == mat4_scale(3, 9, 3))
	PRINT("mat4 comparison fail");


	PRINT(m4t + m4t);
	PRINT(m4t - m4t);
	PRINT(m4t * 6);
	PRINT(m4t * vec4(3, 3, 3, 3));
	PRINT(m4t * m4t);
	m4t = mat4_rotation(90, vec3(1,0,0));
	transpose(m4t);
	PRINT(m4t);
	PRINT(mat4_identity().determinant());*/

	mat4 T = mat4_translation(1, 0, 0);
	mat4 R = mat4_rotation(90, vec3(0,1,0));
	mat4 S = mat4_scale(2, 2, 2);

	PRINT(T);
	PRINT(R);
	PRINT(S);

	mat4 ModelMatrix = T*R*S;

	mat3 NormalMatrix = inverse(toMat3(ModelMatrix));
	transpose(NormalMatrix);

	PRINT(ModelMatrix);
	PRINT(NormalMatrix);

	//-------------------------------[ END OF TESTING ]-------------------------------//

	std::cin.ignore();

	return 0;

}