#include "GeoMapGrid.h"

#include "Terrain.h"
#include "Util.h"

GeoMapGrid::GeoMapGrid()
{
}

GeoMapGrid::~GeoMapGrid()
{
	Destroy();
}

void GeoMapGrid::CreateGeomapGrid(int width, int depth, int patchSize, const BaseTerrain* pTerrain)
{
	if ((width - 1) % (patchSize - 1) != 0)
	{
		int RecommendedWidth = ((width - 1 + patchSize - 1) / (patchSize - 1)) * (patchSize - 1) + 1;
		printf("Width minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", width, patchSize);
		printf("Try using Width = %d\n", RecommendedWidth);
		exit(0);
	}

	if ((depth - 1) % (patchSize - 1) != 0)
	{
		int RecommendedDepth = ((depth - 1 + patchSize - 1) / (patchSize - 1)) * (patchSize - 1) + 1;
		printf("Depth minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", depth, patchSize);
		printf("Try using Width = %d\n", RecommendedDepth);
		exit(0);
	}

	if (patchSize < 3)
	{
		printf("The minimum patch size is 3 (%d)\n", patchSize);
		exit(0);
	}

	if (patchSize % 2 == 0) {
		printf("Patch size must be an odd number (%d)\n", patchSize);
		exit(0);
	}

	m_width = width;
	m_depth = depth;
	m_patchSize = patchSize;

	m_numPatchesX = (width - 1) / (patchSize - 1);
	m_numPatchesZ = (depth - 1) / (patchSize - 1);

	float worldScale = pTerrain->GetWorldScale();
	m_maxLOD = m_lodManager.InitLODManager(patchSize, m_numPatchesX, m_numPatchesZ, worldScale);
	m_LODInfo.resize(m_maxLOD + 1);
	
	CreateGLState();
	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GeoMapGrid::Destroy()
{
	if (m_vao > 0)
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	if (m_vb > 0)
	{
		glDeleteBuffers(1, &m_vb);
	}

	if (m_ib > 0)
	{
		glDeleteBuffers(1, &m_ib);
	}
}

void GeoMapGrid::Render(glm::vec3& cameraPos)
{
	m_lodManager.Update(cameraPos);

	glBindVertexArray(m_vao);

	//glDrawElementsBaseVertex(GL_POINTS, m_LODInfo[0].info[0][0][0][0].count, GL_UNSIGNED_INT, (void*)0, 0);

	for (int patchZ = 0; patchZ < m_numPatchesZ; patchZ++)
	{
		for (int patchX = 0; patchX < m_numPatchesX; patchX++)
		{
			LODManager::PatchLOD pLOD = m_lodManager.GetPatchLod(patchX, patchZ);

			int c = pLOD.core;
			int l = pLOD.left;
			int r = pLOD.right;
			int t = pLOD.top;
			int b = pLOD.bottom;

			size_t baseIndex = sizeof(unsigned int) * m_LODInfo[c].info[l][r][t][b].start;

			int z = patchZ * (m_patchSize - 1);
			int x = patchX * (m_patchSize - 1);
			int baseVertex = z * m_width + x;

			glDrawElementsBaseVertex(GL_TRIANGLES, m_LODInfo[c].info[l][r][t][b].count, GL_UNSIGNED_INT, (void*)baseIndex, baseVertex);
		}
	}

	/*for (int z = 0; z < m_depth - 1; z += (m_patchSize - 1))
	{
		for (int x = 0; x < m_width - 1; x += (m_patchSize - 1))
		{
			int baseVertex = z * m_width + x;
			glDrawElementsBaseVertex(GL_TRIANGLES, (m_patchSize - 1) * (m_patchSize - 1) * 6, GL_UNSIGNED_INT, nullptr, baseVertex);
		}
	}*/

	glBindVertexArray(0);
}

void GeoMapGrid::RenderSettings()
{
	m_lodManager.RenderSettings();
}

void GeoMapGrid::CreateGLState()
{
	//glGenVertexArrays(1, &m_vao1);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);

	glGenBuffers(1, &m_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);

	int POS_LOC = 0;
	int UV_LOC = 1;
	int NORMAL_LOC = 2;

	size_t NumFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glEnableVertexAttribArray(UV_LOC);
	glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;

	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
}

void GeoMapGrid::PopulateBuffers(const BaseTerrain* pTerrain)
{
	std::vector<Vertex> vertices;
	vertices.resize(m_width * m_depth);

	printf("Preparing space for %zu vertices\n", vertices.size());

	InitVertices(pTerrain, vertices);

	int numIndices = CalcNumIndices();
	std::vector<unsigned int> indices;
	indices.resize(numIndices);
	
	numIndices = InitIndices(indices);

	printf("Final number of indices %d\n", numIndices);

	CalcNormals(vertices, indices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

int GeoMapGrid::InitIndices(std::vector<unsigned int>& indices)
{
	int index = 0;

	for (int lod = 0; lod <= m_maxLOD; lod++)
	{
		printf("*** Init indices lod %d ***\n", lod);
		index = InitIndicesLOD(index, indices, lod);
		printf("\n");
	}

	return index;
	
	/*int Index = 0;

	for (int z = 0; z < m_patchSize - 1; z += 2) {
		for (int x = 0; x < m_patchSize - 1; x += 2) {
		

			unsigned int indexCenter = (z + 1) * m_width + x + 1;

			unsigned int indexTemp1 = z * m_width + x;
			unsigned int indexTemp2 = (z + 1) * m_width + x;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2 += m_width;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2++;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2++;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2 -= m_width;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2 -= m_width;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2--;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);

			indexTemp1 = indexTemp2;
			indexTemp2--;

			Index = AddTriangle(Index, indices, indexCenter, indexTemp1, indexTemp2);
		}
	}

	assert(Index == indices.size());*/
}

int GeoMapGrid::InitIndicesLOD(int index, std::vector<unsigned int>& indices, int lod)
{
	int totalIndicesForLOD = 0;

	for (int l = 0; l < LEFT; l++)
	{
		for (int r = 0; r < RIGHT; r++)
		{
			for (int t = 0; t < TOP; t++)
			{
				for (int b = 0; b < BOTTOM; b++)
				{
					m_LODInfo[lod].info[l][r][t][b].start = index;
					index = InitIndicesLODSingle(index, indices, lod, lod + l, lod + r, lod + t, lod + b);

					m_LODInfo[lod].info[l][r][t][b].count = index - m_LODInfo[lod].info[l][r][t][b].start;
					totalIndicesForLOD += m_LODInfo[lod].info[l][r][t][b].count;
				}
			}
		}
	}
	printf("Total indices for LOD: %d\n", totalIndicesForLOD);
	return index;
}

int GeoMapGrid::InitIndicesLODSingle(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom)
{
	int FanStep = powi(2, lodCore + 1);
	int endPos = m_patchSize - 1 - FanStep;

	for (int z = 0; z <= endPos; z += FanStep)
	{
		for (int x = 0; x <= endPos; x += FanStep)
		{
			int lLeft = x == 0 ? lodLeft : lodCore;
			int lRight = x == endPos ? lodRight : lodCore;
			int lBottom = z == 0 ? lodBottom : lodCore;
			int lTop = z == endPos ? lodTop : lodCore;

			index = CreateTriangleFan(index, indices, lodCore, lLeft, lRight, lTop, lBottom, x, z);
		}
	}

	return index;
}

void GeoMapGrid::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices)
{
	int Index = 0;

	for (int x = 0; x < m_width; x++) {
		for (int z = 0; z < m_depth; z++) {

			assert(Index < vertices.size());
			vertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}

	assert(Index == vertices.size());
}

void GeoMapGrid::CalcNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	unsigned int index = 0;

	for (int z = 0; z < m_depth - 1; z += (m_patchSize - 1))
	{
		for (int x = 0; x < m_width - 1; x += (m_patchSize - 1))
		{
			int baseVertex = z * m_width + x;

			int numIndices = m_LODInfo[0].info[0][0][0][0].count;
			for (unsigned int i = 0; i < numIndices; i += 3)
			{
				unsigned int Index0 = baseVertex + indices[i];
				unsigned int Index1 = baseVertex + indices[i + 1];
				unsigned int Index2 = baseVertex + indices[i + 2];
				glm::vec3 v1 = vertices[Index1].pos - vertices[Index0].pos;
				glm::vec3 v2 = vertices[Index2].pos - vertices[Index0].pos;
				glm::vec3 Normal = glm::cross(v2, v1);
				Normal = glm::normalize(Normal);

				vertices[Index0].normal += Normal;
				vertices[Index1].normal += Normal;
				vertices[Index2].normal += Normal;
			}
		}
	}

	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}
}

unsigned int GeoMapGrid::AddTriangle(unsigned int index, std::vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3)
{
	assert(index < indices.size());
	indices[index++] = v1;
	assert(index < indices.size());
	indices[index++] = v2;
	assert(index < indices.size());
	indices[index++] = v3;

	return index;
}

unsigned int GeoMapGrid::CreateTriangleFan(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom, int x, int z)
{
	unsigned int stepLeft	= powi(2, lodLeft);
	unsigned int stepRight	= powi(2, lodRight);
	unsigned int stepTop	= powi(2, lodTop);
	unsigned int stepBottom = powi(2, lodBottom);
	unsigned int stepCenter = powi(2, lodCore);

	unsigned int indexCenter = (z + stepCenter) * m_width + x + stepCenter;

	// first up
	unsigned int indexTemp1 = z * m_width + x;
	unsigned int indexTemp2 = (z + stepLeft) * m_width + x;

	index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	
	// second up

	if (lodLeft == lodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 += stepLeft * m_width;

		index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	

	// first right
	indexTemp1 = indexTemp2;
	indexTemp2 += stepTop;

	index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	// second right
	if (lodTop == lodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 += stepTop;

		index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	// first down
	indexTemp1 = indexTemp2;
	indexTemp2 -= stepRight * m_width;

	index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second down
	if (lodRight == lodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepRight * m_width;

		index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	// first left
	indexTemp1 = indexTemp2;
	indexTemp2 -= stepBottom;

	index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second left
	if (lodBottom == lodCore)
	{
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepBottom;

		index = AddTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}
	
	

	return index;
}

int GeoMapGrid::CalcNumIndices()
{
	int numQuads = (m_patchSize - 1) * (m_patchSize - 1);
	int numIndices = 0;
	int maxPermutationsPerLevel = 16;
	const int indicesPerQuad = 6;
	for (int lod = 0; lod <= m_maxLOD; lod++)
	{
		printf("LOD %d: num quads %d\n", lod, numQuads);
		//numIndices += indicesPerQuad * numIndices;
		numIndices += numQuads * indicesPerQuad * maxPermutationsPerLevel;
		numQuads /= 4;
	}

	printf("Initial number of indices %d\n", numIndices);
	return numIndices;
}

void GeoMapGrid::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float worldScale = pTerrain->GetWorldScale();
	float y = pTerrain->GetHeight(x, z);
	//y = 0;
	//y = 0.1f * y;
	pos = glm::vec3(x * worldScale, y, z * worldScale);

	float size = pTerrain->GetTerrainSize();

	uv = glm::vec2((float)x / size, (float)z / size);
}
