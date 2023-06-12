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

	m_terrainRender.Init();
}

void BaseTerrain::Render(Camera& camera)
{
	glm::mat4 vp = camera.CalculateVewMatrix();

	m_terrainRender.Enable();
	m_terrainRender.SetVP(vp);

	for (size_t i = 0; i < TEXTURE_COUNT; i++)
	{
		if (m_pTextures[i])
		{
			m_pTextures[i]->Bind(GL_TEXTURE0 + i);
		}
	}

	m_triangleList.Render();
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
	return m_terrainSize;
}

void BaseTerrain::Destroy()
{
	m_heightMap.Destroy();
	m_triangleList.Destroy();
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
