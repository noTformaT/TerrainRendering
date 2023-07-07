#pragma once

#include "Terrain.h"

class MidpointDisplacementTerrain : public BaseTerrain
{
public:
	MidpointDisplacementTerrain();
	~MidpointDisplacementTerrain();

	void CreateMidpoinDisplacement(int size, float roughness, float minHeight, float maxHeight);

	int size = 257;
	float roughness = 1.0f;
	float minHeight = 0.0f;
	float maxHeight = 250.0f;

private:
	void CreateMidpoinDisplacementF32(float roughness);
	void DiamondStep(int rectSize, float curHeight);
	void SquareStep(int rectSize, float curHeight);
};