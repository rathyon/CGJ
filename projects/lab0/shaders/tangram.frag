#version 330 core

uniform int blabla;

in vec4 ex_color;
out vec4 out_color;

void main(void) {
	out_color = ex_color;
}