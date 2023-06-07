#pragma once

#include "Texture.h"
#include "Image.h"

#define MAX_TEXTURE_TILES 4

struct TextureHeightDesc
{
	float low = 0.0f;
	float optimal = 0.0f;
	float height = 0.0f;
};

struct TextureTile
{
	Image image;
	TextureHeightDesc heightDesc;
};

class BaseTerrain;

class TextureGenerator
{
public:
	TextureGenerator();
	~TextureGenerator();

	void LoadTile(const char* fileName);

	Texture* GenerateTexture(int textureSize, BaseTerrain* pTerrain, float minHeight, float maxHeight);

private:
	void CalculateTextureRegios(float minHeight, float maxHeight);

	float RegionPercent(int tile, float height);

	TextureTile m_textureTiles[MAX_TEXTURE_TILES] = {};
	int m_numTextureTiles = 0;
};