#include "Shapes.h"
#include <glad/glad.h>
#include <iostream>
#include "Libs/stb_image.h"
void generateTexture(unsigned int& texture, const char* path)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Could not load texture: " << path << "\n";
        return;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Adjust texture format based on the number of channels
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void retangle(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, unsigned int& texture1, unsigned int& texture2)
{

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    generateTexture(texture1, "TextureImages/container.jpg");
    generateTexture(texture2, "TextureImages/awesomeface.png");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void triangle(unsigned int& VAO, unsigned int& VBO)
{
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    float texCoords[] = {
        0.0f,0.0f, // bottom left
        1.0f, 0.0f, //bottom right
        0.5, 1.0f//top
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
void CreateCircle(float r, int tri, int render, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
    if (tri <= 3)
    {
        return;
    }
    tri--;

    float x, y;

    int size = (tri * 3) + 6;
    int sizeIndice = 3 * (tri + 1);
    float* pos = (float*)malloc(size * (sizeof(float)));
    int* indices = (int*)malloc(sizeIndice * (sizeof(int)));

    if (!pos || !indices)
    {
        printf("ERROR MEMORY ALOCATION FAILED FOR CIRCLE.\n");
        if (pos)
            free(pos);
        if (indices)
            free(indices);
        return;
    }

    pos[0] = 0.0f; pos[1] = 0.0f; pos[2] = 0.0f;//ponto central do circulo
    pos[3] = r * cos(2 * PI * 0 / tri); pos[4] = r * sin(2 * PI * 0 / tri); pos[5] = 0.0f;//ponto inicial do circulo

    int currentPosIndex = 6;
    int currentIndicesIndex = 0;
    int i = 0;
    bool stopRender = render != -1 && i >= render;
    while (i < tri && !stopRender)
    {
        i++;
        x = r * cos(2 * PI * i / tri);
        y = r * sin(2 * PI * i / tri);

        //definindo vertices
        pos[currentPosIndex] = x; currentPosIndex++; pos[currentPosIndex] = y; currentPosIndex++; pos[currentPosIndex] = 0.0f; currentPosIndex++;
        indices[currentIndicesIndex] = 0; currentIndicesIndex++; indices[currentIndicesIndex] = i - 1; currentIndicesIndex++; indices[currentIndicesIndex] = i; currentIndicesIndex++;
        if (i == tri)
        {
            indices[currentIndicesIndex] = 0; currentIndicesIndex++; indices[currentIndicesIndex] = i; currentIndicesIndex++; indices[currentIndicesIndex] = 1; currentIndicesIndex++;
            break;
        }
        stopRender = render != -1 && i >= render;
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), pos, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndice * sizeof(int), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(pos);
    free(indices);

}
void triangles(unsigned int& VAO, unsigned int& VBO, const int& vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9,(void*) vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}
void house(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -0.5f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -1.0f, -0.5f, -0.0f,
        -0.75f, -0.25f, 0.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        0, 2, 3,   // second triangle
        2, 3 ,4
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void box(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, unsigned int& texture1, unsigned int& texture2)
{
    float vertices[] = {
        // Posições         // Coordenadas de textura
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posição dos vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Gerar texturas
    generateTexture(texture1, "TextureImages/container.jpg");
    generateTexture(texture2, "TextureImages/awesomeface.png");

    // Ativar unidades de textura e associá-las com os uniformes nos shaders
    glActiveTexture(GL_TEXTURE0);  // Primeira unidade de textura
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);  // Segunda unidade de textura
    glBindTexture(GL_TEXTURE_2D, texture2);
}
