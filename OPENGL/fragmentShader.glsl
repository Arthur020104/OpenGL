//FRAGMENT SHADER
#version 330 core
uniform float timeValue;
in vec3 position;
in vec3 colorFrag;
void main()
{
    float red = (cos(timeValue) + position.x + position.y + colorFrag.x)/4.0f + 0.5f; 
    float green = (sin(timeValue) + position.x + position.y + colorFrag.y)/4.0f + 0.5f;
    float blue = (sin(timeValue) + position.x + position.y + colorFrag.z)/4.0f + 0.5f;
    gl_FragColor = vec4(red, green, blue, 1.0f);
}

/*
-Exercicio de cor baseado no posicao
in vec3 position;

void main()
{
  
    gl_FragColor = vec4(position.x, position.y, position.z, 1.0f);
}
*/