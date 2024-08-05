#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
//Callback function for when window is resized
//Função de callback para quando a janela é resized(redimensionada)
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void triangle();
void retangle();
void triangles(unsigned int* VAOt, unsigned int* VBOt, float* vertices);
void house();
void CreateCircle(float r, int tri, int render);
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
bool wireFrame = false;
unsigned int VBO, VAO, EBO, VBO1, VAO1;

unsigned int compileShaders()
{
    /*********************Vertex Shader**************************/
    const char* vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec3 pos;\n"
        "out vec3 aPos;"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
        "    aPos = pos;\n"
        "}\n";
    /*********************Shader Compilation*******************************/
    unsigned int vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderId);
    /*********************Error Checking**************************/
    int sucess;
    char infoLog[512];
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        printf("ERROR:: VERTEX SHADER COMPILATION FAILED\n %s \n", infoLog);
    }
    /*********************Vertex Shader**************************/

    /*********************Fragment Shader***************************/
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 aPos;\n"
        "uniform vec4 ourColor;\n"
        "uniform float ourTime;\n"
        "void main()\n"
        "{\n"
        "float red   =(sin(ourTime)+sin(aPos.y+aPos.x))/4 +0.5f;\n"
        "float green =(sin(ourTime)+sin(aPos.y+aPos.x))/4 +0.5f;\n"
        "float blue  = (sin(ourTime)+sin(aPos.y+aPos.x))/4 +0.5f;\n"
        "green = blue = sqrt(pow(aPos.x,2)+pow(aPos.y,2)) < ((mod(ourTime, 10.0))/10) && sqrt(pow(aPos.x,2)+pow(aPos.y,2)) > ((mod(ourTime, 10.0))/10)-0.05f ? 1 : red ;\n"
        "FragColor = ourColor+vec4(red,green,blue,1.0f);\n"
        "}\n";
    //0.5*aPos.y+0.5;
    // //aPos.x - 0.5*aPos.y+0.5;
    //-1 e -1 max e 1 1 none     blue 1 e -1 max green//-0.5*aPos.y+0.5
    /*********************Shader Compilation*******************************/
    unsigned int fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderId);
    /*********************Error Checking**************************/
    //using the same variables for error checking as the vertexShader
    //Usando a mesma variavel do vertexShader para checagem de erro 
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        printf("ERROR:: FRAGMENT SHADER COMPILATION FAILED\n %s\n", infoLog);
    }
    /*********************Fragment Shader***************************/

    /******************Creating Shader Program***********************/
    unsigned int shaderProgramId;
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);
    /*********************Error Checking**************************/
   //using the same variables for error checking as the vertexShader
   //Usando a mesma variavel do vertexShader para checagem de erro 
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &sucess);
    if (!sucess)
    {
        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
        printf("ERROR:: LINKIN THE PROGRAM FAILED\n %s\n", infoLog);

    }
    shaderProgramId;
    /******************Creating Shader Program***********************/
    //delete the shader objects once we've linked them into the program object; we no longer need them anymore
    //apagando os shader objects após linkar com o objecto do programa; nao vamos precisar mais deles
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    return shaderProgramId;
}
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
    /*Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need
    Mac OS X you need to add glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); to your initialization code for it to work*///Foda-se
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LeanOpengl", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //Avisando ao GLFW que queremos que essa funcao seja chamada toda vez que a janela é redimensionada
    //telling GLFW we want to call this function on every window resize by registering it
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    /*******************Set GLFW********************/
    //O Glad gerencia ponteiros para funcoes do OpenGL, entao precisamos inicializar o GLAD antes the usarmos funcoes do OpenGL
    //Glad manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))/* GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.*/
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    //Os primeiros parametros definem a localizacao do canto esquerdo da janela
    //The first two parameters of glViewport set the location of the lower left corner of the window.
    glViewport(0, 0, WIDTH, HEIGHT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//WireframeMode
    float vertices1[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f,0.0f,
        0.0f, 0.5f,0.0f,
    };
    // triangles(&VAO, &VBO, vertices1);
    float vertices2[] = {
        0.5f, 0.0f, 0.0f,
        1.0f,0.0f,0.0f,
        0.75, 0.5f,0.0f

    };
    //triangles(&VAO1, &VBO1, vertices2);
    //
    /*****************************Circle teste***************************************************************
    bool diminuindo = false;
    int desenhar = 3;
    int circlesSize = 500;
    CreateCircle(0.5, circlesSize, 3);
    long long int currentFrame = 0;
    long long int lastFrame = 0;
    *****************************Circle teste****************************************************************/
    triangle();
    unsigned int shaderProgramId = compileShaders();

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /************Color**********/
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue / 2.0f) + 0.5f);
        int vertexColorLocation = glGetUniformLocation(shaderProgramId, "ourColor");
        int vertexTimeLocation = glGetUniformLocation(shaderProgramId, "ourTime");
        glUseProgram(shaderProgramId);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUniform1f(vertexTimeLocation, (GLfloat)timeValue);
        if (vertexColorLocation == -1) {
            fprintf(stderr, "Could not find uniform location for 'ourColor'\n");
        }
        if (vertexTimeLocation == -1) {
            fprintf(stderr, "Could not find uniform location for 'ourTime'\n");
        }
        /************Color**********/
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 2*3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();

        /*****************************Circle teste***************************************************************
        currentFrame++;
        if (currentFrame - lastFrame >50)
        {
            lastFrame = currentFrame;
            if (desenhar <= 3)
            {
                diminuindo = false;
            }
            else if (desenhar >= circlesSize)
            {
                diminuindo = true;
            }

            if (diminuindo)
            {
                CreateCircle(0.5, circlesSize, --desenhar);
            }
            else
            {
                CreateCircle(0.5, circlesSize, ++desenhar);
            }
        }
        *****************************Circle teste****************************************************************/

    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgramId);

    glfwTerminate();
    return 0;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool toggletWireFrameMode = false;
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

    }//fazendo com que somente quando o botao é solto o wireFrmae sofre toggle// Only when the button is realease the wireframe is toggle
    if (toggletWireFrameMode && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        toggletWireFrameMode = false;
        wireFrame = !wireFrame;
        wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    /***************WireFrame*************************/
}
void triangle()
{
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f,0.0f,
        0.0f, 1.0f,0.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glEnableVertexAttribArray(0);
}
constexpr float PI = 3.14159265358979323846f;
void CreateCircle(float r, int tri, int render)
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
void triangles(unsigned int* VAOt, unsigned int* VBOt, float* vertices)
{
    glGenVertexArrays(1, VAOt);
    glGenBuffers(1, VBOt);
    glBindVertexArray(*VAOt);
    glBindBuffer(GL_ARRAY_BUFFER, *VBOt);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}
void house()
{
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,   // top left 
        0.0f, 1.0f, 0.0f//top
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,   // second triangle
        0, 3 ,4
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

void retangle()
{

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
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


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}