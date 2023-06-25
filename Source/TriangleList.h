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

	void Destroy();

	~TriangleList();

private:
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
		
		void InitVertex(const BaseTerrain* pTerrain, int x, int z);
	};


	void CreateGLState();
	void PopulateBuffers(const BaseTerrain* pTerrain);
	void InitIndices(std::vector<unsigned int>& indices);
	void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices);
	void CalcNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

private:
	int m_width = 0;
	int m_depth = 0;
	GLuint m_vao = 0;
	GLuint m_vb = 0;
	GLuint m_ib = 0;
};