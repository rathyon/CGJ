#version 330 core

in vec4 in_Position;
in vec4 in_Normal;

out vec4 ex_color;
out vec4 ex_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_color = vec4(0.0);
	ex_normal = in_Normal;
}