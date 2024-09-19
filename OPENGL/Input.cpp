#include "Input.h"
bool toggletWireFrameMode = false;
bool wireFrame = false;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// Se nao estiver pressionado, glfwGetKey retorna GLFW_RELEASE//if it's not pressed, glfwGetKey returns GLFW_RELEASE
    {
        glfwSetWindowShouldClose(window, true);
    }
    /***************WireFrame*************************/
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        toggletWireFrameMode = true;

    }//fazendo com que somente quando o botao eh solto o wireFrmae sofre toggle// Only when the button is realease the wireframe is toggle
    if (toggletWireFrameMode && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        toggletWireFrameMode = false;
        wireFrame = !wireFrame;
        wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    /***************WireFrame*************************/
}