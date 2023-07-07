#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "LODManager.h"

class BaseTerrain;

class GeoMapGrid
{
public:
	GeoMapGrid();
	~GeoMapGrid();

	void CreateGeomapGrid(int width, int depth, int patchSize, const BaseTerrain* pTerrain);

	void Destroy();
	
	void Render(glm::vec3& cameraPos);
	void RenderSettings();

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
	int InitIndices(std::vector<unsigned int>& indices);
	int InitIndicesLOD(int index, std::vector<unsigned int>& indices, int lod);
	int InitIndicesLODSingle(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom);
	//unsigned int AddTriangle(unsigned int index, std::vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3);
	void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices);
	void CalcNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	unsigned int AddTriangle(unsigned int index, std::vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3);
	unsigned int CreateTriangleFan(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom, int x, int z);

	int m_width = 0;
	int m_depth = 0;
	int m_patchSize = 0;
	int m_maxLOD = 0;
	GLuint m_vao = 0;
	GLuint m_vb = 0;
	GLuint m_ib = 0;

	struct SingleLODInfo
	{
		int start = 0;
		int count = 0;
	};

#define LEFT 2
#define RIGHT 2
#define TOP 2
#define BOTTOM 2

	struct LODInfo
	{
		SingleLODInfo info[LEFT][RIGHT][TOP][BOTTOM];
	};

	std::vector<LODInfo> m_LODInfo;
	int m_numPatchesX = 0;
	int m_numPatchesZ = 0;
	LODManager m_lodManager;

	int CalcNumIndices();
};