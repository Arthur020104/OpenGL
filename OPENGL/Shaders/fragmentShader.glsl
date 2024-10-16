#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
in vec3 position;
void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2,TexCoord), mixValue);
    return;
}

/*//FRAGMENT SHADER
#version 330 core
//uniform float timeValue;
in vec3 position;
in vec3 colorFrag;
in vec2 texture;
void main()
{
   // float red = (cos(timeValue) + position.x + position.y + colorFrag.x)/4.0f + 0.5f; 
    //float green = (sin(timeValue) + position.x + position.y + colorFrag.y)/4.0f + 0.5f;
    //float blue = (sin(timeValue) + position.x + position.y + colorFrag.z)/4.0f + 0.5f;
    gl_FragColor = vec4(texture, 1.0f);
}*/


/*TESTE SHADER
//FRAGMENT SHADER
#version 330 core
uniform float timeValue;
in vec3 position;
in vec3 colorFrag;
uniform float circleTickness;
void main()
{
    float aux = pow(position.x, 2) + pow(position.y, 2);
    float timeIntervalZeroAndOne = mod(timeValue, 2);
    bool isInCircle = ((aux) <= pow(timeIntervalZeroAndOne/10, 2) && (aux) >= pow(timeIntervalZeroAndOne/10 - circleTickness, 2)) || (aux) <= pow((mod(timeValue, 3)/10), 2) && (aux) >= pow((mod(timeValue, 3)/10) - circleTickness, 2) || (aux) <= pow((mod(timeValue, 4)/10), 2) && (aux) >= pow((mod(timeValue, 4)/10) - circleTickness, 2);
    if(isInCircle)
    {
        gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }
    float red = (cos(timeValue) + position.x + position.y + colorFrag.x)/4.0f + 0.5f; 
    float green = (sin(timeValue) + position.x + position.y + colorFrag.y)/4.0f + 0.5f;
    float blue = (sin(timeValue) + position.x + position.y + colorFrag.z)/4.0f + 0.5f;
    gl_FragColor = vec4(red, green, blue, 1.0f);
}

*/


/*
-Exercicio de cor baseado no posicao
in vec3 position;

void main()
{
  
    gl_FragColor = vec4(position.x, position.y, position.z, 1.0f);
}
*/