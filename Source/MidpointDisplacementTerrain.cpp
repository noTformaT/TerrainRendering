#include "MidpointDisplacementTerrain.h"

#include "Util.h"

MidpointDisplacementTerrain::MidpointDisplacementTerrain()
{
}

MidpointDisplacementTerrain::~MidpointDisplacementTerrain()
{
}

void MidpointDisplacementTerrain::CreateMidpoinDisplacement(int size, float roughness, float minHeight, float maxHeight)
{
	if (roughness < 0.0f)
	{
		printf("%s: roughness must be positive - %f \n", __FUNCTION__, roughness);
		exit(0);
	}

	m_terrainSize = size;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;

	m_terrainRender.Enable();
	m_terrainRender.SetMinMaxHeight(minHeight, maxHeight);

	m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, 0.0f);

	CreateMidpoinDisplacementF32(roughness);

	m_heightMap.Normalize(minHeight, maxHeight);

	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

void MidpointDisplacementTerrain::CreateMidpoinDisplacementF32(float roughness)
{
	int rectSize = CalcNextPowerOfTwo(m_terrainSize);
	float curHeight = (float)rectSize / 2.0f;
	float heightReduce = pow(2.0f, -roughness);

	while (rectSize > 0)
	{
		DiamondStep(rectSize, curHeight);

		SquareStep(rectSize, curHeight);

		rectSize /= 2;

		curHeight *= heightReduce;
	}
}

void MidpointDisplacementTerrain::DiamondStep(int rectSize, float curHeight)
{
	int halfRectSize = rectSize / 2;

	for (size_t y = 0; y < m_terrainSize; y += rectSize)
	{
		for (size_t x = 0; x < m_terrainSize; x += rectSize)
		{
			int next_x = (x + rectSize) % m_terrainSize;
			int next_y = (y + rectSize) % m_terrainSize;

			if (next_x < x)
			{
				next_x = m_terrainSize - 1;
			}

			if (next_y < y)
			{
				next_y = m_terrainSize - 1;
			}

			float topLeft = m_heightMap.Get(x, y);
			float topRight = m_heightMap.Get(next_x, y);
			float bottomLeft = m_heightMap.Get(x, next_y);
			float bottomRight = m_heightMap.Get(next_x, next_y);

			int mid_x = (x + halfRectSize) % m_terrainSize;
			int mid_y = (y + halfRectSize) % m_terrainSize;

			float randomValue = RandomFloatRange(curHeight, - curHeight);

			float midPoint = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;

			m_heightMap.Set(mid_x, mid_y, midPoint + randomValue);
		}
	}
}

void MidpointDisplacementTerrain::SquareStep(int rectSize, float curHeight)
{
	int halfRectSize = rectSize / 2;

	for (size_t y = 0; y < m_terrainSize; y += rectSize)
	{
		for (size_t x = 0; x < m_terrainSize; x += rectSize)
		{

			int next_x = (x + rectSize) % m_terrainSize;
			int next_y = (y + rectSize) % m_terrainSize;

			if (next_x < x) {
				next_x = m_terrainSize - 1;
			}

			if (next_y < y) {
				next_y = m_terrainSize - 1;
			}

			int mid_x = (x + halfRectSize) % m_terrainSize;
			int mid_y = (y + halfRectSize) % m_terrainSize;

			int prev_mid_x = (x - halfRectSize + m_terrainSize) % m_terrainSize;
			int prev_mid_y = (y - halfRectSize + m_terrainSize) % m_terrainSize;

			float CurTopLeft = m_heightMap.Get(x, y);
			float CurTopRight = m_heightMap.Get(next_x, y);
			float CurCenter = m_heightMap.Get(mid_x, mid_y);
			float PrevYCenter = m_heightMap.Get(mid_x, prev_mid_y);
			float CurBotLeft = m_heightMap.Get(x, next_y);
			float PrevXCenter = m_heightMap.Get(prev_mid_x, mid_y);

			float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + RandomFloatRange(-curHeight, curHeight);
			float CurTopMid = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + RandomFloatRange(-curHeight, curHeight);

			m_heightMap.Set(mid_x, y, CurTopMid);
			m_heightMap.Set(x, mid_y, CurLeftMid);
		}
	}
}
