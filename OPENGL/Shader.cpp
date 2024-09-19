#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
void Shader::checkCompileErrors(unsigned int Id, std::string type)
{
	int sucess;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(Id, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetShaderInfoLog(Id, 512, NULL, infoLog);
			std::cout << "ERROR::" << type << "SHADER COMPILATION FAILED"<<infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(Id, GL_LINK_STATUS, &sucess);
		if (!sucess)
		{
			glGetProgramInfoLog(Id, 512, NULL, infoLog);
			std::cout << "ERROR::" << type << " LINKIN THE PROGRAM FAILED" << infoLog << std::endl;
		}
	}
}
Shader::~Shader()
{
	glDeleteProgram(ID);
}
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	/********** pegando o vertex e fragment shader do filepath(caminho do arquivo)**********/
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//garantindo que os objectos ifstream podem dar throw em exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//abrindo arquivos
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		//lendo o conteudo no buffers dos aquivos nas streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//fechando arquivos
		vShaderFile.close();
		fShaderFile.close();
		//convertendo streamstrings pra string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCESFULLY_READ\n");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	/********** pegando o vertex e fragment shader do filepath(caminho do arquivo)**********/

	/***********Compilando shaders**********************************************************/

	/*********************Vertex Shader**************************/

	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
	glCompileShader(vertexShaderId);
	checkCompileErrors(vertexShaderId, "VERTEX");

	/*********************Vertex Shader**************************/

	/*********************Fragment Shader**************************/

	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShaderId);
	checkCompileErrors(fragmentShaderId, "FRAGMENT");

	/*********************Fragment Shader**************************/

	/*********************Shader Program**************************/

	unsigned int programId;
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	checkCompileErrors(programId, "PROGRAM");

	/*********************Shader Program**************************/

	/***********Compilando shaders**********************************************************/

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	ID = programId;
}
void Shader::use()
{
	glUseProgram(ID);
}
void Shader::setBool(const std::string &name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, const glm::mat4& value)
{
	unsigned int transformLoc = glGetUniformLocation(ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}