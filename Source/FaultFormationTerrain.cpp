#include "FaultFormationTerrain.h"

bool FaultFormationTerrain::TerrainPoint::isEqual(TerrainPoint& p) const
{
	return ((x == p.x) && (z == p.z));
}

FaultFormationTerrain::FaultFormationTerrain()
{
}

FaultFormationTerrain::~FaultFormationTerrain()
{
}

void FaultFormationTerrain::CreateFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float filter)
{
	m_terrainSize = terrainSize;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;

	m_terrainRender.Enable();
	m_terrainRender.SetMinMaxHeight(minHeight, maxHeight);

	m_heightMap.InitArray2D(terrainSize, terrainSize, 0.0f);
	CreateFaultFormationInternal(iterations, minHeight, maxHeight, filter);

	m_heightMap.Normalize(minHeight, maxHeight);

	m_triangleList.CreateTriangleList(terrainSize, terrainSize, this);
}

void FaultFormationTerrain::CreateFaultFormationInternal(int iterations, float minHeight, float maxHeight, float filter)
{
	float deltaHeight = maxHeight - minHeight;

	for (int curItter = 0; curItter < iterations; curItter++)
	{
		float iterationRatio = ((float)curItter) / ((float)iterations);
		float height = maxHeight - iterationRatio * deltaHeight;

		TerrainPoint p1, p2;

		GetRandomTerrainPoint(p1, p2);

		int dirX = p2.x - p1.x;
		int dirZ = p2.z - p1.z;

		for (int z = 0; z < m_terrainSize; z++)
		{
			for (int x = 0; x < m_terrainSize; x++)
			{
				int dirX_in = x - p1.x;
				int dirZ_in = z - p1.z;

				int crossProduct = dirX_in * dirZ - dirX * dirZ_in;

				if (crossProduct > 0)
				{
					float curHeight = m_heightMap.Get(x, z);
					m_heightMap.Set(x, z, curHeight + height);
				}
			}
		}
	}

	ApplyFIRFilter(filter);
}

void FaultFormationTerrain::GetRandomTerrainPoint(TerrainPoint& p1, TerrainPoint& p2)
{
	p1.x = rand() % m_terrainSize;
	p1.z = rand() % m_terrainSize;

	int counter = 0;

	do
	{
		p2.x = rand() % m_terrainSize;
		p2.z = rand() % m_terrainSize;

		if (counter++ == 1000)
		{
			printf("Endless loop detection in %s:%d\n", __FILE__, __LINE__);
			assert(0);
		}
	} while (p1.isEqual(p2));
}

float FaultFormationTerrain::FIRFilterSinglePoint(int x, int z, float prevVal, float filter)
{
	float curVal = m_heightMap.Get(x, z);
	float newVal = filter * prevVal + (1.0f - filter) * curVal;

	m_heightMap.Set(x, z, newVal);

	return newVal;
}

void FaultFormationTerrain::ApplyFIRFilter(float filter)
{
	for (int z = 0; z < m_terrainSize; z++)
	{
		float prevVal = m_heightMap.Get(0, z);
		for (int x = 1; x < m_terrainSize; x++)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int z = 0; z < m_terrainSize; z++)
	{
		float prevVal = m_heightMap.Get(m_terrainSize - 1, z);
		for (int x = m_terrainSize - 2; x >= 0; x--)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int x = 0; x < m_terrainSize; x++)
	{
		float prevVal = m_heightMap.Get(x, 0);
		for (int z = 1; z < m_terrainSize; z++)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int x = 0; x < m_terrainSize; x++)
	{
		float prevVal = m_heightMap.Get(x, m_terrainSize - 1);
		for (int z = m_terrainSize - 2; z >= 0; z--)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}
}
