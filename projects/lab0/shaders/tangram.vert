#version 330 core

in vec4 in_Position;
in vec4 in_Color;
out vec4 ex_color;

uniform mat4 Matrix;

void main(void){
	gl_Position = Matrix * in_Position;
	ex_color = in_Color;
}