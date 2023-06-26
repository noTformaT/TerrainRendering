#include "RenderSystem.h"

#include <string.h>

#include "Util.h"

RenderSystem::RenderSystem()
{
	m_shaderProg = 0;
}

RenderSystem::~RenderSystem()
{
	for (ShaderObjectList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool RenderSystem::Init()
{
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}

	return true;
}

void RenderSystem::Enable()
{
	glUseProgram(m_shaderProg);
}

GLuint RenderSystem::GetProgram() const
{
	return m_shaderProg;
}

bool RenderSystem::AddShader(GLenum ShaderType, const char* pFileName)
{
	std::string s;

	if (!ReadFile(pFileName, s))
	{
		return false;
	}

	GLuint shaderObj = glCreateShader(ShaderType);

	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		return false;
	}

	m_shaderObjList.push_back(shaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint length[1] = { (GLint)s.size() };

	glShaderSource(shaderObj, 1, p, length);

	glCompileShader(shaderObj);

	GLint succes;

	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &succes);

	if (!succes)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling '%s': '%s'\n", pFileName, InfoLog);
		return false;
	}

	glAttachShader(m_shaderProg, shaderObj);

	return true;
}

bool RenderSystem::Finalize()
{
	GLint success;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (success == 0) 
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		return false;
	}

	glValidateProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), nullptr, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		//   return false;
	}

	for (ShaderObjectList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return true;
}

GLuint RenderSystem::GetUniformLocation(const char* pUniformName)
{
	GLint location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (location == INVALID_UNIFORM_LOCATION)
	{
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return location;
}
