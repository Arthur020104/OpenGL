#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//pegando o vertex e fragment shader do filepath(caminho do arquivo)
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
		}
	}
	void use();
	void SetBool(const std::string &name, bool value)const;
	void SetInt(const std::string& name, int value)const;
	void SetFloat(const std::string& name, float value)const;
};
#endif