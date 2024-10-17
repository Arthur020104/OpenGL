#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern double DeltaTime;  
extern const unsigned int WIDTH;  
extern const unsigned int HEIGHT;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
