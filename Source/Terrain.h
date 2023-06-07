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

	void InitTerrain(float worldScale);
	void Render(Camera& camera);

	void LoadFromFile(const char* pFileName);

	float GetHeight(int x, int z) const;
	float GetWorldScale() const;
	float GetTerrainSize() const;

	void Destroy();
protected:
	void LoadHeighMapFile(const char* pFileName);

	int m_terrainSize = 0;
	Array2D<float> m_heightMap;
	TriangleList m_triangleList;
	TerrainRenderSystem m_terrainRender;
	TestRenderSystem m_testRender;
	float m_worldScale = 1.0f;
	float m_minHeight = 0.0f;
	float m_maxHeight = 0.0f;
};