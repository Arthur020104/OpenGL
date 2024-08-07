//VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 colorFrag;
out vec3 position;
//out bool isOnBorder;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	position = pos;
	colorFrag = color;
}
/*
-Exercicio de Inverter triangulo
void main()
{
	gl_Position = vec4(pos.x*-1, pos.y*-1, pos.z*-1, 1.0f);
	colorFrag = color;
}

-Exercicio de offset para direita
uniform float horizontalOffset;

void main()
{
	gl_Position = vec4(pos.x+horizontalOffset, pos.y, pos.z, 1.0f);
	colorFrag = color;
}
-Exercicio de cor baseado no posicao
out vec3 position;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	position = pos;
}
*/