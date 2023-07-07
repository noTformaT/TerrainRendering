#include "LODManager.h"

#include "Util.h"


#include <imgui.h>

LODManager::LODManager()
{
}

LODManager::~LODManager()
{
}

int LODManager::InitLODManager(int patchSize, int numPatchesX, int numPatchesZ, float worldScale)
{
	m_patchSize = patchSize;
	m_numPatchesX = numPatchesX;
	m_numPatchesZ = numPatchesZ;
	m_worldScale = worldScale;

	CalcMaxLOD();

	PatchLOD zero;
	m_map.InitArray2D(numPatchesX, numPatchesZ, zero);

	m_regions.resize(m_maxLOD + 1);

	CalcLODRegions();

	return m_maxLOD;
}

void LODManager::Update(const glm::vec3& cameraPos)
{
	UpdateLODMapPass1(cameraPos);
	UpdateLODMapPass2(cameraPos);
}

const LODManager::PatchLOD& LODManager::GetPatchLod(int PatchX, int PatchZ) const
{
	return m_map.Get(PatchX, PatchZ);
}

void LODManager::PrintLODMap()
{
}

void LODManager::RenderSettings()
{
	if (ImGui::CollapsingHeader("LOD settings"))
	{
		std::vector<float> regions;
		regions.resize(m_regions.size());


		for (size_t i = 0; i < m_regions.size(); i++)
		{
			regions[i] = m_regions[i];
			float& region = regions[i];
			std::string caption = std::string("LOD ") + std::to_string(i);
			ImGui::SliderFloat(caption.c_str(), &region, 0, FAR_PLANE);
		}

		for (size_t i = 0; i < regions.size() - 1; i++)
		{
			if (regions[i] > regions[i + 1])
			{
				if (regions[i] != m_regions[i])
				{
					regions[i + 1] = regions[i];
				}
				else
				{
					regions[i] = regions[i + 1];
				}
			}
		}

		for (size_t i = 0; i < regions.size(); i++)
		{
			m_regions[i] = regions[i];
		}
	}
}

void LODManager::CalcLODRegions()
{
	int sum = 0;

	for (int i = 0; i <= m_maxLOD; i++)
	{
		sum += (i + 1);
	}

	printf("Sum %d\n", sum);

	float x = FAR_PLANE / (float)sum;

	int temp = 0;

	for (int i = 0; i <= m_maxLOD; i++)
	{
		int curRange = (int)(x * (i + 1));
		m_regions[i] = temp + curRange;
		temp += curRange;

		printf("%d %f\n", i, m_regions[i]);
	}
}

void LODManager::CalcMaxLOD()
{
	int numSegments = m_patchSize - 1;

	if (ceilf(log2f((float)numSegments)) != floorf(log2f((float)numSegments))) {
		printf("The number of vertices in the patch minus one must be a power of two\n");
		printf("%f %f\n", ceilf(log2f((float)numSegments)), floorf(log2f((float)numSegments)));
		exit(0);
	}

	int patchSizeLog2 = (int)log2f((float)numSegments);
	printf("log2 of patch size %d is %d\n", m_patchSize, patchSizeLog2);
	m_maxLOD = patchSizeLog2 - 1;
}

void LODManager::UpdateLODMapPass1(const glm::vec3& cameraPos)
{
	int centerStep = m_patchSize / 2;

	for (int loadMapZ = 0; loadMapZ < m_numPatchesZ; loadMapZ++)
	{
		for (int loadMapX = 0; loadMapX < m_numPatchesX; loadMapX++)
		{
			int x = loadMapX * (m_patchSize - 1) + centerStep;
			int z = loadMapZ * (m_patchSize - 1) + centerStep;

			glm::vec3 patchCenter = glm::vec3(z * (float)m_worldScale, 0.0f, x * (float)m_worldScale);

			float distanceToCamera = glm::distance(cameraPos, patchCenter);

			int coreLOD = DistanceToLOD(distanceToCamera);
			//coreLOD = m_maxLOD;

			PatchLOD* pPatchLOD = m_map.GetAddr(loadMapX, loadMapZ);
			pPatchLOD->core = coreLOD;
		}
	}
}

void LODManager::UpdateLODMapPass2(const glm::vec3& cameraPos)
{
	int step = m_patchSize / 2;

	for (int LODmapZ = 0; LODmapZ < m_numPatchesZ; LODmapZ++)
	{
		for (int LODmapX = 0; LODmapX < m_numPatchesX; LODmapX++)
		{
			int coreLod = m_map.Get(LODmapX, LODmapZ).core;

			int indexLeft = LODmapX;
			int indexRight = LODmapX;
			int indexTop = LODmapZ;
			int indexBottom = LODmapZ;

			if (LODmapX > 0)
			{
				indexLeft--;

				if (m_map.Get(indexLeft, LODmapZ).core > coreLod)
				{
					m_map.At(LODmapX, LODmapZ).left = 1;
				}
				else
				{
					m_map.At(LODmapX, LODmapZ).left = 0;
				}
			}

			if (LODmapX < m_numPatchesX - 1)
			{
				indexRight++;

				if (m_map.Get(indexRight, LODmapZ).core > coreLod)
				{
					m_map.At(LODmapX, LODmapZ).right = 1;
				}
				else
				{
					m_map.At(LODmapX, LODmapZ).right = 0;
				}
			}

			if (LODmapZ > 0)
			{
				indexBottom--;

				if (m_map.Get(LODmapX, indexBottom).core > coreLod)
				{
					m_map.At(LODmapX, LODmapZ).bottom = 1;
				}
				else
				{
					m_map.At(LODmapX, LODmapZ).bottom = 0;
				}
			}

			if (LODmapZ < m_numPatchesZ - 1)
			{
				indexTop++;

				if (m_map.Get(LODmapX, indexTop).core > coreLod)
				{
					m_map.At(LODmapX, LODmapZ).top = 1;
				}
				else
				{
					m_map.At(LODmapX, LODmapZ).top = 0;
				}
			}
		}
	}
}

int LODManager::DistanceToLOD(float Distance)
{
	int lod = m_maxLOD;

	for (int i = 0; i <= m_maxLOD ; i++)
	{
		if (Distance < m_regions[i])
		{
			lod = i;
			break;
		}
	}

	return lod;
}
