#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 Tex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3  p_osition;
out vec2 tex_coord;
out vec4 fragPos;

void main()
{
	tex_coord = Tex;
	
	p_osition =  vec3(inverse(transpose(model)) * vec4(position, 1.0f));
	fragPos = model * vec4(position, 1.0f);
	gl_Position =  projection * view *  model * vec4(position, 1.0f);
}