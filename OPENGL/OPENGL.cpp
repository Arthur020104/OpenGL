#include "shader.h"
#include "Input.h"
#include "Shapes.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


//Callback function for when window is resized
//Funcao de callback para quando a janela eh resized(redimensionada)
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
unsigned int texture1, texture2;
void printMatrix(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
glm::mat4 myRotate(const float &theta, const glm::vec3 &a)
{
    glm::mat4 I4 = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); I4 = glm::transpose(I4);
    glm::mat4 mat = glm::mat4(a.x * a.x, a.x * a.y, a.x * a.z, 0.0f, a.y * a.x, a.y * a.y, a.y * a.z, 0.0f, a.z * a.x, a.z * a.y, a.z * a.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); mat = glm::transpose(mat);
    glm::mat4 mat2 = glm::mat4(0.0f, -a.z, a.y, 0.0f, a.z, 0.0f, -a.x, 0.0f, -a.y, a.x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); mat2 = glm::transpose(mat2);
    glm::mat4 rot = glm::cos(theta) * I4 +(1-glm::cos(theta)) * mat + glm::sin(theta) * mat2;
    rot[3][3] = 1;
    return rot;
}
glm::mat3 myRotate2d(const float& theta)
{
    glm::mat3 rot = glm::mat3(glm::cos(theta), -glm::sinh(theta), 0.0f, glm::sin(theta), glm::cos(theta), 0.0f, 0.0f, 0.0f, 1.0f); rot = glm::transpose(rot);
    return rot;
}
glm::mat3 myTranslate2d(const glm::vec2& amount)
{
    glm::mat3 transMat = glm::mat3(1.0f, 0.0f, amount.x, 0.0f, 1.0f, amount.y, 0.0f, 0.0f, 1.0f); transMat = glm::transpose(transMat);
    return transMat;
}
glm::mat4 myScale(const glm::vec3& amount)
{
    glm::mat4 scaleMat = glm::mat4(amount.x, 0.0f, 0.0f, 0.0f, 0.0f, amount.y, 0.0f, 0.0f, 0.0f, 0.0f, amount.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); scaleMat = glm::transpose(scaleMat);
    return scaleMat;
}
glm::mat4 myTranslate(const glm::vec3& amount)
{
    glm::mat4 transMat = glm::mat4(1.0f, 0.0f, 0.0f,amount.x, 0.0f, 1.0f, 0.0f, amount.y, 0.0f, 0.0f, 1.0f, amount.z, 0.0f, 0.0f, 0.0f, 1.0f); transMat = glm::transpose(transMat);
    return transMat;
}
void teste()
{
    glm::vec3 inicialVec = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 result = myRotate2d(glm::radians(282.0f)) * myTranslate2d(glm::vec2(1.0f, 0.0f)) * inicialVec;
   // std::cout << "X: " << result.x << " Y: " << result.y << "\n";

    glm::vec3 result1 = myTranslate2d(glm::vec2(1.0f, 0.0f)) * myRotate2d(glm::radians(282.0f))  * inicialVec;
    //std::cout << "X1: " << result1.x << " Y1: " << result1.y << "\n";
    
    
}
/*******************/
int main()
    {
    if (!glfwInit())
    {
        printf("Failed to init glfw\n");
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need    Mac OS X you need to add glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); to your initialization code for it to work*/
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LeanOpengl", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //Avisando ao GLFW que queremos que essa funcao seja chamada toda vez que a janela eh redimensionada
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    /*******************Set GLFW********************/
    /*******************Set GLAD********************/

    //O Glad gerencia ponteiros para funcoes do OpenGL, entao precisamos inicializar o GLAD antes the usarmos funcoes do OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))/*GLFW nos da glfwGetProcAddress que define a funcao correta baseado em qual sistema operacional estamos compilando para .*/
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    /*******************Set GLAD********************/

    glViewport(0, 0, WIDTH, HEIGHT);
    Shader defaultShader("C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/vertexShaderDefault.glsl", "C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/fragmentShaderDefault.glsl");
    Shader boxShaders("C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/vertexShader.glsl", "C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/fragmentShader.glsl");

    boxShaders.use();

    boxShaders.setInt("texture1", 0);
    boxShaders.setInt("texture2", 1);


    float k = 0;

    CreateCircle(0.1, 50, -1, VAO1, VBO1, EBO1);
    ////house(VAO1, VBO1, EBO1);
    retangle(VAO, VBO, EBO, texture1, texture2);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        teste();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        

        

        
        glBindVertexArray(VAO);
        float timeValue = glfwGetTime();
        boxShaders.use();
      //  boxShaders.
        boxShaders.setFloat("timeValue", timeValue);
        boxShaders.setFloat("mixValue", 0.2f);
        glm::mat4 trans = myTranslate(glm::vec3(0.5f, -0.5f, 0.0f));
        trans *= myRotate((float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
      /*  glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/
        boxShaders.setMat4("transform", trans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// glDrawArrays(GL_TRIANGLES, 0, 3);
        trans = myTranslate(glm::vec3(-0.5f, 0.5f, 0.0f));
        trans *= myScale(glm::vec3(1,1,1)*glm::sin((float)glfwGetTime()));
        boxShaders.setMat4("transform", trans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        k += glm::sin((float)glfwGetTime())/10;
        std::cout << k / 200 * 12 << "\n";
        for (int i = 0; i < 11; i++)
        {
            for (int j = 0; j < 11-i; j++)
            {
                trans = myRotate((float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
                trans *= myTranslate(glm::vec3(-0.9f+j/10.0f+ i / 20.f, -0.9f + i / 10.0f, 0.0f));
                trans *= myScale(glm::vec3(0.1f, 0.1f, 0.1f));
                if (k / 200 * 12 >= j && k / 200 * 12 < j + 1)
                {
                    trans *= myRotate((float)glfwGetTime()*50, glm::vec3(0.0f, 0.6f, 0.4f));
                }
                // trans *= myScale(glm::vec3(1, 1, 1) * glm::sin((float)glfwGetTime()));
                boxShaders.setMat4("transform", trans);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            
        }
        glBindVertexArray(VAO1);
        defaultShader.use();
        glDrawElements(GL_TRIANGLES, 150, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


