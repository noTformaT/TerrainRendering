#pragma once

#include "Terrain.h"

class FaultFormationTerrain : public BaseTerrain
{
public:
	FaultFormationTerrain();
	~FaultFormationTerrain();

	void CreateFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float filter);

	int size = 257;
	int iterration = 500;
	float minHeight = 0.0f;
	float maxHeight = 300.0f;
	float filter = 0.4f;

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

	void CreateFaultFormationInternal(int iterations, float minHeight, float maxHeight, float filter);
	void GetRandomTerrainPoint(TerrainPoint& p1, TerrainPoint& p2);

	float FIRFilterSinglePoint(int x, int y, float prevVal, float Filter);
	void ApplyFIRFilter(float filter);
};