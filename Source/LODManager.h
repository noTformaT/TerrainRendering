#pragma once

#include <glm/glm.hpp>
#include "Array2D.h"
#include <vector>

class LODManager
{
public:
	LODManager();
	~LODManager();

	int InitLODManager(int patchSize, int numPatchesX, int numPatchesZ, float worldScale);

	void Update(const glm::vec3& cameraPos);

	struct PatchLOD
	{
		int core = 0;
		int left = 0;
		int right = 0;
		int top = 0;
		int bottom = 0;
	};

	const PatchLOD& GetPatchLod(int PatchX, int PatchZ) const;

	void PrintLODMap();

	void RenderSettings();

private:
	void CalcLODRegions();
	void CalcMaxLOD();
	void UpdateLODMapPass1(const glm::vec3& cameraPos);
	void UpdateLODMapPass2(const glm::vec3& cameraPos);

	int DistanceToLOD(float Distance);

	int m_maxLOD = 0;
	int m_patchSize = 0;
	int m_numPatchesX = 0;
	int m_numPatchesZ = 0;
	float m_worldScale = 0.0f;

	Array2D<PatchLOD> m_map;
	std::vector<float> m_regions;
};