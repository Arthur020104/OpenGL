#include <glad/glad.h>
#include <cmath>
#include <vector>
#include "shader.hpp"
#include "Input.hpp"
#include "Shapes.hpp"
//Callback function for when window is resized
//Funcao de callback para quando a janela eh resized(redimensionada)
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
unsigned int VBO, VAO, EBO, VBO1, VAO1;
unsigned int texture1, texture2;
int main()
{
    /*******************Set GLFW********************/
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

    Shader myShaders("C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/vertexShader.glsl", "C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/fragmentShader.glsl");

    myShaders.use();

    glUniform1i(glGetUniformLocation(myShaders.ID, "texture1"), 0);
    myShaders.setInt("texture2", 1);

    retangle(VAO, VBO, EBO, texture1, texture2);

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myShaders.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        

        float timeValue = glfwGetTime();

        myShaders.setFloat("timeValue", timeValue);
        myShaders.setFloat("mixValue", 0.2f);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


