#pragma once
#include "GL/glew.h"
#include <string>

class Shader
{
public:

	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		SHADER_TYPE_COUNT //always last! handy trick
	};

	enum AttributeType {
		VERTICES_ATTRIB,
		NORMALS_ATTRIB,
		TEXCOORDS_ATTRIB,
		COLORS_ATTRIB
	};

	Shader();
	~Shader();

	void init();
	void loadShader(ShaderType st, std::string filename);
	void setAttributeName(AttributeType at, std::string name);
	void linkProgram();

	bool isLinked();
	bool isValid();
	void shaderInfoLog(ShaderType st);
	void programInfoLog();
	bool isShaderCompiled(ShaderType st);

	GLuint programID;
	GLuint shaders[SHADER_TYPE_COUNT];
	GLuint shaderTypes[SHADER_TYPE_COUNT] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
};

