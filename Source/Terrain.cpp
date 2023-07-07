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

void BaseTerrain::InitTerrain(float worldScale, float textureScale, std::vector<std::string>& textureFileNames)
{
	if (textureFileNames.size() != TEXTURE_COUNT)
	{
		printf("%s:%d - number of provided textures (%lud) is not equal to the size of the texture array (%lud)\n",
			__FILE__, __LINE__, textureFileNames.size(), 4);
		exit(0);
	}

	m_textureScale = textureScale;

	m_worldScale = worldScale;
	
	for (size_t i = 0; i < TEXTURE_COUNT; i++)
	{
		m_pTextures[i] = new Texture(GL_TEXTURE_2D);
		m_pTextures[i]->Load(textureFileNames[i]);
	}

	m_shadowMapRender.Init();
	m_terrainRender.Init();
	m_shadowMapViewRender.Init();
}

void BaseTerrain::RenderShadowPass(Camera& camera, LightingData& lightingData, GLint width, GLint height)
{
	m_shadowMapRender.Enable();

	glViewport(0, 0, lightingData.shadowMap.GetShadowHeight(), lightingData.shadowMap.GetShadowHeight());
	lightingData.shadowMap.Write();
	//glClear(GL_DEPTH_BUFFER_BIT);

	lightingData.shadowBox.Init(lightingData.lightViewMatrix, &camera);
	
	
	lightingData.shadowBox.update();


	glm::vec3 center = camera.GetPosition();

	glm::mat4 lightTransform = lightingData.CalculateLightTransform(center);
	glm::mat4 model(1.0f);
	//model = glm::translate(model, camera.GetPosition());
	m_shadowMapRender.SetModelTransform(&model);
	m_shadowMapRender.SetDirectionalLightTransform(&lightTransform);

	m_triangleList.Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BaseTerrain::RenderBasePass(bool isGeoMappingRender, Camera& camera, LightingData& lightingData, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 vp = camera.CalculateVewMatrix();

	m_terrainRender.Enable();

	/*glm::mat4 projection = camera.GetProjection();
	glm::mat4 view = camera.GetView();

	glm::vec3 center = camera.GetPosition();
	glm::mat4 model(1.0f);
	glm::mat4 lightTransform = lightingData.CalculateLightTransform(center);

	m_terrainRender.SetModelViewProjection(model, view, projection);
	m_terrainRender.SetLightSpaceMatrix(lightTransform);
	lightingData.shadowMap.Read(GL_TEXTURE0);*/

	m_terrainRender.SetVP(vp);
	m_terrainRender.SetLevels(h0, h1, h2, h3, h4, h5);
	m_terrainRender.SetLightingData(lightingData);

	for (size_t i = 0; i < TEXTURE_COUNT; i++)
	{
		if (m_pTextures[i])
		{
			m_pTextures[i]->Bind(GL_TEXTURE0 + i);
		}
	}

	if (isGeoMappingRender)
	{
		glm::vec3 camPos = camera.GetPosition();
		m_geoMapGrid.Render(camPos);
	}
	else
	{
		m_triangleList.Render();
	}
}

void BaseTerrain::Render(bool isGeoMappingRender, Camera& camera, LightingData& lightingData, GLint width, GLint height)
{
	/*glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);*/

	//RenderShadowPass(camera, lightingData, width, height);

	RenderBasePass(isGeoMappingRender, camera, lightingData, width, height);

	//

	//RenderShadowMapPreview(width - 256, height - 256, 256, 256, lightingData);
}

void BaseTerrain::RenderLODSettings()
{
	m_geoMapGrid.RenderSettings();
}

void BaseTerrain::RenderShadowMapPreview(GLint x, GLint y, GLint width, GLint height, LightingData& lightingData)
{

	glViewport(x, y, width, height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, width, height);

	//glClearColor(0.1f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_SCISSOR_TEST);

	m_shadowMapViewRender.Enable();

	glBindVertexArray(m_triangleList.m_vao1);

	lightingData.shadowMap.Read(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void BaseTerrain::LoadFromFile(const char* pFileName)
{
	LoadHeighMapFile(pFileName);

	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

float BaseTerrain::GetHeight(int x, int z) const
{
	return m_heightMap.Get(x, z);
}

float BaseTerrain::GetWorldScale() const
{
	return m_worldScale;
}

float BaseTerrain::GetTerrainSize() const
{
	return (float)m_terrainSize;
}

void BaseTerrain::Destroy()
{
	m_heightMap.Destroy();
	m_triangleList.Destroy();
	m_geoMapGrid.Destroy();
}

void BaseTerrain::UpdateLayers(float l0, float l1, float l2, float l3, float l4, float l5)
{
	h0 = l0;
	h1 = l1;
	h2 = l2;
	h3 = l3;
	h4 = l4;
	h5 = l5;
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
