#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 position;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
	position = aPos;
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