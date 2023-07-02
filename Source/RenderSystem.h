#pragma once

#include <list>
#include <GL/glew.h>
#include <string>

class RenderSystem
{
public:
	RenderSystem();
	
	virtual ~RenderSystem();

	virtual bool Init();

	void Enable();

	GLuint GetProgram() const;

	void SetInt(const std::string& name, int value) const;

protected:

	bool AddShader(GLenum ShaderType, const char* pFileName);

	bool Finalize();

	GLuint GetUniformLocation(const char* pUniformName) const;

	GLuint m_shaderProg = 0;

private:
	
	typedef std::list<GLuint> ShaderObjectList;
	ShaderObjectList m_shaderObjList;
};