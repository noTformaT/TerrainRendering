#pragma once

#include "Terrain.h"

class FaultFormationTerrain : public BaseTerrain
{
public:
	FaultFormationTerrain();
	~FaultFormationTerrain();

	void CreateFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight);

private:
	struct TerrainPoint
	{
		int x = 0;
		int z = 0;

		void Print()
		{
			printf("[%d, %d]", x, z);
		}

		bool isEqual(TerrainPoint& p) const;
	};

	void CreateFaultFormationInternal(int iterations, float minHeight, float maxHeight);
	void GetRandomTerrainPoint(TerrainPoint& p1, TerrainPoint& p2);
};