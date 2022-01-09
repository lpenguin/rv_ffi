#include "Shader.h"

#include <iostream>
#include <GLES3/gl32.h>

Shader::Shader(const char* vs_shader, const char* fs_shader)
	:_program(0)
{
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	{
		glShaderSource(vertex_shader, 1, &vs_shader, nullptr);
		glCompileShader(vertex_shader);
		GLint success;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if(!success){
			char infoLog[1024];
			glGetShaderInfoLog(vertex_shader, 1024, NULL, infoLog);
			std::cout << "Vertext shader compilation error" << "\n" << infoLog << "\n" << std::endl;
			return;
		}
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		glShaderSource(fragment_shader, 1, &fs_shader, nullptr);
		glCompileShader(fragment_shader);
		GLint success;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success){
			char infoLog[1024];
			glGetShaderInfoLog(fragment_shader, 1024, NULL, infoLog);
			std::cout << "Fragment shader compilation error" << "\n" << infoLog << "\n" << std::endl;
			return;
		}
	}

	_program = glCreateProgram();
	{
		glAttachShader(_program, vertex_shader);
		glAttachShader(_program, fragment_shader);
		glLinkProgram(_program);
		GLint success;
		glGetProgramiv(_program, GL_LINK_STATUS, &success);
		if(!success){
			char infoLog[1024];
			glGetProgramInfoLog(fragment_shader, 1024, NULL, infoLog);
			std::cout << "Program link error" << "\n" << infoLog << "\n" << std::endl;
			return;
		}
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

}

void Shader::use()
{
	if(_program == 0){
		std::cout << "error: shader is not initialized" << std::endl;
		return;
	}
	glUseProgram(_program);
}

void Shader::unuse()
{
	glUseProgram(0);
}

void Shader::set_uniform(const char* name, int value)
{
	if(_program == 0){
		std::cout << "error: shader is not initialized" << std::endl;
		return;
	}

	glUniform1i(glGetUniformLocation(_program, name), value);
}
