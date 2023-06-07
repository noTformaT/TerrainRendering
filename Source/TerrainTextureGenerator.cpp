#include "TerrainTextureGenerator.h"
#include "Terrain.h"
#include <glm/vec3.hpp>



TextureGenerator::TextureGenerator()
{

}

TextureGenerator::~TextureGenerator()
{
}

void TextureGenerator::LoadTile(const char* fileName)
{
    if (m_numTextureTiles == MAX_TEXTURE_TILES) {
        printf("%s:%d: exceeded the maximum of texture tiles with '%s'\n", __FILE__, __LINE__, fileName);
        exit(0);
    }
    
    m_textureTiles[m_numTextureTiles].image.Load(fileName);
}

Texture* TextureGenerator::GenerateTexture(int textureSize, BaseTerrain* pTerrain, float minHeight, float maxHeight)
{
    if (m_numTextureTiles == 0) {
        printf("%s:%d: no texture tiles loaded\n", __FILE__, __LINE__);
        exit(0);
    }

    CalculateTextureRegios(minHeight, maxHeight);

    int BPP = 3;
    int TextureBytes = textureSize * textureSize * BPP;
    unsigned char* pTextureData = (unsigned char*)malloc(TextureBytes);
    unsigned char* p = pTextureData;

    float HeightMapToTextureRatio = (float)pTerrain->GetTerrainSize() / (float)textureSize;

    printf("Height map to texture ratio: %f\n", HeightMapToTextureRatio);

    for (int y = 0; y < textureSize; y++) {
        for (int x = 0; x < textureSize; x++) {

            float InterpolatedHeight = 0.0f;// pTerrain->GetHeightInterpolated((float)x * HeightMapToTextureRatio,
                //(float)y * HeightMapToTextureRatio);

            float Red = 0.0f;
            float Green = 0.0f;
            float Blue = 0.0f;

            for (int Tile = 0; Tile < m_numTextureTiles; Tile++) {
                glm::vec3 Color = m_textureTiles[Tile].image.GetColor(x, y);

                float BlendFactor = RegionPercent(Tile, InterpolatedHeight);

                Red += BlendFactor * Color.r;
                Green += BlendFactor * Color.g;
                Blue += BlendFactor * Color.b;
            }

            if (Red > 255.0f || Green > 255.0f || Blue > 255.0f) {
                printf("%d:%d: %f %f %f\n", y, x, Red, Green, Blue);
                exit(0);
            }

            p[0] = (unsigned char)Red;
            p[1] = (unsigned char)Green;
            p[2] = (unsigned char)Blue;

            p += 3;
        }
    }

    Texture* pTexture = new Texture(GL_TEXTURE_2D);

    //stbi_write_png("texture.png", TextureSize, TextureSize, BPP, pTextureData, TextureSize * BPP);

    pTexture->LoadRaw(textureSize, textureSize, BPP, pTextureData);

    free(pTextureData);

    return pTexture;
}

void TextureGenerator::CalculateTextureRegios(float minHeight, float maxHeight)
{
}

float TextureGenerator::RegionPercent(int tile, float height)
{
    return 0.0f;
}
