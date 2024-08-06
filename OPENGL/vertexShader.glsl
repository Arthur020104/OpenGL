//VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 colorFrag;
out vec3 position;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	colorFrag = color;
}