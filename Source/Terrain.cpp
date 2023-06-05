#include "Terrain.h"

#include <assert.h>
#include <math.h>

#include "Util.h"
#include "Camera.h"

BaseTerrain::BaseTerrain()
{
}

BaseTerrain::~BaseTerrain()
{
}

void BaseTerrain::InitTerrain()
{
	m_terrainRender.Init();
}

void BaseTerrain::Render(Camera& camera)
{
	glm::mat4 vp = camera.CalculateVewMatrix();

	m_terrainRender.Enable();
	m_terrainRender.SetVP(vp);

	m_triangleList.Render();
}

void BaseTerrain::LoadFromFile(const char* pFileName)
{
	LoadHeighMapFile(pFileName);

	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

void BaseTerrain::LoadHeighMapFile(const char* pFileName)
{
	int fileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(pFileName, fileSize);

	assert(fileSize % sizeof(fileSize) == 0);

	m_terrainSize = sqrt(fileSize / sizeof(float));

	m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, p);
	//m_heightMap.PrintFloat();
}
