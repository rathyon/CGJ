#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::init() {
	for (int i = 0; i < Shader::SHADER_TYPE_COUNT; ++i) {
		shaders[i] = 0;
	}
	programID = glCreateProgram();
}

void Shader::loadShader(Shader::ShaderType st, std::string filename) {

	std::ifstream in(filename);
	std::string contents;

	if (in) {

		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}

	const char *shader = contents.c_str();

	if (shader != NULL) {
		shaders[st] = glCreateShader(shaderTypes[st]);
		glShaderSource(shaders[st], 1, &shader, NULL);
		glCompileShader(shaders[st]);
		glAttachShader(programID, shaders[st]);
	}

}

void Shader::setAttributeName(AttributeType at, std::string name) {
	glBindAttribLocation(programID, at, name.c_str());
}

void Shader::linkProgram() {
	glLinkProgram(programID);
}
