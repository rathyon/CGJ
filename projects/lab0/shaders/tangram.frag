#version 330 core

uniform vec4 color;

in vec4 ex_color;
in vec4 ex_normal;

out vec4 out_color;

void main(void) {
	
	normalize(ex_normal);

	out_color = color;

	if(ex_normal.x < 0)
		out_color = out_color * 0.75f;
		if(ex_normal.y < 0)
			out_color = out_color * 0.75f;
			if(ex_normal.z < 0)
				out_color = out_color * 0.75f;

	if(ex_normal.y < 0)
		out_color = out_color * 0.75f;
		if(ex_normal.z < 0)
			out_color = out_color * 0.75f;

	if(ex_normal.z < 0)
			out_color = out_color * 0.75f;
}