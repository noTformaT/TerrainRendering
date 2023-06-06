#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class BaseTerrain;

class TriangleList
{
public:
	TriangleList();

	void CreateTriangleList(int width, int depth, const BaseTerrain* pTerrain);

	void Render();

	~TriangleList();

private:
	struct Vertex
	{
		glm::vec3 pos;
		
		void InitVertex(const BaseTerrain* pTerrain, int x, int z);
	};


	void CreateGLState();
	void PopulateBuffers(const BaseTerrain* pTerrain);
	void InitIndices(std::vector<unsigned int>& indices);
	void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices);

private:
	int m_width = 0;
	int m_depth = 0;
	GLuint m_vao = 0;
	GLuint m_vb = 0;
	GLuint m_ib = 0;
};