#pragma once

#include "Array2D.h"
#include "TerrainRenderSystem.h"
#include "TriangleList.h"
#include "GeoMapGrid.h"
#include "TestRenderSystem.h"
#include "Texture.h"
#include "LightingData.h"
#include "DirectionalShadowMapRenderSystem.h"
#include "ShadowMapViewRenderSystem.h"

#define TEXTURE_COUNT 5

class Camera;

class BaseTerrain
{
public:
	BaseTerrain();
	~BaseTerrain();

	//void CreateTriangle();

	void InitTerrain(float worldScale, float textureScale, std::vector<std::string>& textureFileNames);

	void RenderShadowPass(Camera& camera, LightingData& lightingData, GLint width, GLint height);
	void RenderBasePass(bool isGeoMappingRender, Camera& camera, LightingData& lightingData, GLint width, GLint height);

	void Render(bool isGeoMappingRender, Camera& camera, LightingData& lightingData, GLint width, GLint height);

	void RenderLODSettings();

	void RenderShadowMapPreview(GLint x, GLint y, GLint width, GLint height, LightingData& lightingData);

	void LoadFromFile(const char* pFileName);

	float GetHeight(int x, int z) const;
	float GetWorldScale() const;
	float GetTerrainSize() const;

	void Destroy();

	void UpdateLayers(float l0, float l1, float l2, float l3, float l4, float l5);

protected:
	void LoadHeighMapFile(const char* pFileName);

	int m_patchSize = 33;
	int m_terrainSize = 0;
	Array2D<float> m_heightMap;
	TriangleList m_triangleList;
	GeoMapGrid m_geoMapGrid;
	DirectionalShadowMapRenderSystem m_shadowMapRender;
	TerrainRenderSystem m_terrainRender;
	TestRenderSystem m_testRender;
	ShadowMapViewRenderSystem m_shadowMapViewRender;
	float m_worldScale = 1.0f;
	float m_minHeight = 0.0f;
	float m_maxHeight = 0.0f;
	float m_textureScale = 1.0f;
	Texture* m_pTextures[TEXTURE_COUNT] = { nullptr };

	float h0 = 0.218f;
	float h1 = 0.249f;
	float h2 = 0.284f;
	float h3 = 0.386f;
	float h4 = 0.560f;
	float h5 = 0.684;
};