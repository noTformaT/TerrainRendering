#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class BaseTerrain;

class GeoMapGrid
{
public:
	GeoMapGrid();
	~GeoMapGrid();

	void CreateGeomapGrid(int width, int depth, int patchSize, const BaseTerrain* pTerrain);

	void Destroy();
	
	void Render();

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

	unsigned int AddTriangle(unsigned int index, std::vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3);

	int m_width = 0;
	int m_depth = 0;
	int m_patchSize = 0;
	GLuint m_vao = 0;
	GLuint m_vb = 0;
	GLuint m_ib = 0;
};