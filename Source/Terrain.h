#pragma once

#include "Array2D.h"
#include "TerrainRenderSystem.h"
#include "TriangleList.h"
#include "TestRenderSystem.h"

class Camera;

class BaseTerrain
{
public:
	BaseTerrain();
	~BaseTerrain();

	//void CreateTriangle();

	void InitTerrain();
	void Render(Camera& camera);

	void LoadFromFile(const char* pFileName);

protected:
	void LoadHeighMapFile(const char* pFileName);

	int m_terrainSize = 0;
	Array2D<float> m_heightMap;
	TriangleList m_triangleList;
	TerrainRenderSystem m_terrainRender;
	TestRenderSystem m_testRender;
};