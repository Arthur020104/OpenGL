#include "OPENGL.h"
#include "shader.h"
#include "Input.h"
#include "Shapes.h"
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


//Callback function for when window is resized
//Funcao de callback para quando a janela eh resized(redimensionada)

double DeltaTime = 0;
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

GLFWwindow* Window;

unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
unsigned int Texture1, Texture2;

double LastTime = 0;

float Speed = 8.0f;
float DefaultSpeed = 8.0f;
float VerticalSens = 50.0f;
float HorizontalSens = 50.0f;

int const AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 30;

glm::vec3 Camera = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 LookPos = Camera + glm::vec3(0, 0, 100.0f);

glm::mat4 View;

const glm::mat4 PROJECTION = glm::perspective(glm::radians(103.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);

double LastXPos = -100000, LastYPos = -100000;
glm::mat4 myRotate(const float& theta, const glm::vec3& a)
{
    glm::mat4 I4 = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); I4 = glm::transpose(I4);
    glm::mat4 mat = glm::mat4(a.x * a.x, a.x * a.y, a.x * a.z, 0.0f, a.y * a.x, a.y * a.y, a.y * a.z, 0.0f, a.z * a.x, a.z * a.y, a.z * a.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); mat = glm::transpose(mat);
    glm::mat4 mat2 = glm::mat4(0.0f, -a.z, a.y, 0.0f, a.z, 0.0f, -a.x, 0.0f, -a.y, a.x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); mat2 = glm::transpose(mat2);
    glm::mat4 rot = glm::cos(theta) * I4 + (1 - glm::cos(theta)) * mat + glm::sin(theta) * mat2;
    rot[3][3] = 1;
    return rot;
}
void keyInput()
{
        glm::vec3 forward = glm::normalize(LookPos - Camera);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
       // std::cout << right.x << "--" << right.y << "--" << right.z << "\n";
        if (isKeyPressed(Window, GLFW_KEY_S))
        {
            Camera +=  glm::fvec1((double)-Speed * DeltaTime) * forward;
        }
        if (isKeyPressed(Window, GLFW_KEY_W))
        {
            Camera += glm::fvec1((double)Speed * DeltaTime) * forward;
        }
        if (isKeyPressed(Window, GLFW_KEY_D))
        {

            Camera += glm::fvec1((double)Speed * DeltaTime) * right;
            //LookPos.x += -28 * DeltaTime;
        }
        if (isKeyPressed(Window, GLFW_KEY_A))
        {
            Camera += glm::fvec1((double)Speed * DeltaTime) * -right;
            //LookPos.x += 28 * DeltaTime;

        }
        if (isKeyPressed(Window, GLFW_KEY_E))
        {
            Camera.y += Speed * DeltaTime;
        }
        if (isKeyPressed(Window, GLFW_KEY_Q))
        {
            Camera.y += -Speed * DeltaTime;
        }
        if (Speed <= DefaultSpeed && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
        {
            Speed *= 4;
        }
        else
        {
            Speed = DefaultSpeed;
        }

}
void cameraMovement()
{
    glm::vec3 forward = glm::normalize(LookPos - Camera);
    double xpos, ypos;
    glfwGetCursorPos(Window, &xpos, &ypos);

    if (LastXPos == -100000 || LastYPos == -100000)
    {
        LastXPos = xpos; LastYPos = ypos;
        return;
    }



    //Horizontal Rotatation Camera
    // Rotação horizontal em torno do eixo Y
    glm::vec4 aux = glm::vec4(LookPos.x, LookPos.y, LookPos.z, 1.0f);

    aux = myRotate(glm::radians(HorizontalSens) * DeltaTime * (LastXPos - xpos), glm::vec3(0, 1.0f, 0)) * aux;

    LookPos.x = aux.x;
    LookPos.y = aux.y;
    LookPos.z = aux.z;

    // Rotação vertical em torno do foward
    aux = glm::vec4(LookPos.x, LookPos.y, LookPos.z, 1.0f);

    aux = myRotate(glm::radians(VerticalSens) * DeltaTime * (LastYPos - ypos), glm::vec3(forward.z * -1, 0, forward.x)) * aux;
    
    LookPos.x = aux.x;
    LookPos.y = aux.y;
    LookPos.z = aux.z;



    LastXPos = xpos; LastYPos = ypos;
}
int init(GLFWwindow** window)
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
    *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpengl", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(*window);
    //Avisando ao GLFW que queremos que essa funcao seja chamada toda vez que a janela eh redimensionada
    glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        

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
    return 0;
}
/*******************/
int main()
{

    if (init(&Window) == -1)
    {
        return -1;
    }
    Shader defaultShader("C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/vertexShaderDefault.glsl", "C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/fragmentShaderDefault.glsl");
    Shader boxShaders("C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/vertexShader.glsl", "C:/Users/arthu/OneDrive/Desktop/OpenGl/OPENGLCOM/OPENGL/OPENGL/Shaders/fragmentShader.glsl");
    box(VAO, VBO, EBO, Texture1, Texture2);
    boxShaders.use();

    boxShaders.setInt("texture1", 0);
    boxShaders.setInt("texture2", 1);

    //glm::mat4 model = glm::mat4(1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    
   // CreateCircle(0.1, 50, -1, VAO1, VBO1, EBO1);
    ////house(VAO1, VBO1, EBO1);

    int FpsSampleCounter = 0;
    double DeltatimeMean = 0;
    // Mean for every 2 frames with weights 0.8 and 0.2, with the newer frame being weighted higher.
    while (!glfwWindowShouldClose(Window))
    {
        processInput(Window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture2);
        
        

        double timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;

        boxShaders.use();
        boxShaders.setFloat("timeValue",(float) timeValue);
        boxShaders.setFloat("mixValue", 0.2f);

        DeltatimeMean = (DeltatimeMean*0.2 + DeltaTime*0.8)/2;
        FpsSampleCounter++;
        if (FpsSampleCounter >= AMOUNT_OF_SAMPLES_TO_COUNT_FPS)
        {
            FpsSampleCounter = 0;
            std::cout << "Fps: " << int(1 / DeltaTime) << "\n";
        }
        
        keyInput();
        cameraMovement();
          
        View = glm::lookAt(Camera, LookPos, glm::vec3(0, 1, 0));// glm::translate(View, glm::vec3(0.0f, 0.0f, -2.0f));

        unsigned int viewLoc = glGetUniformLocation(boxShaders.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
        

        boxShaders.setMat4("projection", PROJECTION);
      

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i * (timeValue*2);
            if (i % 3 == 0)
            {
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            boxShaders.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glfwSwapBuffers(Window);
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




/*

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
glm::mat4 glm::translate(View, const glm::vec3& amount)
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
*/