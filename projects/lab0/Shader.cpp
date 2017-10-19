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

void Shader::shaderInfoLog(Shader::ShaderType st) {
	int logLen = 0;
	int charsWritten = 0;
	char *infoLog;

	std::string log;

	if (shaders[st]) {
		glGetShaderiv(shaders[st], GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			infoLog = (char *)malloc(logLen);
			glGetShaderInfoLog(shaders[st], logLen, &charsWritten, infoLog);
			if (charsWritten)
				log = infoLog;
			free(infoLog);
		}
		else {
			log = "Shader loaded, no info log.";
		}
	}
	else
		log = "Shader not loaded.";

	std::cout << log << std::endl;
}

void Shader::programInfoLog() {

	int logLen = 0;
	int charsWritten = 0;
	char *infoLog;

	std::string log = "Program created, no info log.";;

	if (programID) {
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			infoLog = (char *)malloc(logLen);
			glGetProgramInfoLog(programID, logLen, &charsWritten, infoLog);
			if (charsWritten)
				log = infoLog;
			free(infoLog);
		}
	}

	std::cout << log << std::endl;

}

bool Shader::isShaderCompiled(Shader::ShaderType st) {

	GLint b = GL_FALSE;

	if (shaders[st]) {

		glGetShaderiv(shaders[st], GL_COMPILE_STATUS, &b);
	}

	return (b != GL_FALSE);
}

bool Shader::isLinked() {

	GLint b = GL_FALSE;

	if (programID) {

		glGetProgramiv(programID, GL_LINK_STATUS, &b);
	}
	return (b != GL_FALSE);
}

bool Shader::isValid() {
	GLint b = GL_FALSE;

	if (programID) {

		glValidateProgram(programID);
		glGetProgramiv(programID, GL_VALIDATE_STATUS, &b);
	}

	return (b != GL_FALSE);
}