#include "GeoMapGrid.h"

#include "Terrain.h"

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

void GeoMapGrid::Render()
{
	glBindVertexArray(m_vao);

	for (int z = 0; z < m_depth	- 1; z += (m_patchSize - 1))
	{
		for (int x = 0; x < m_width - 1; x += (m_patchSize - 1))
		{
			int baseVertex = z * m_width + x;
			glDrawElementsBaseVertex(GL_TRIANGLES, (m_patchSize - 1) * (m_patchSize - 1) * 6, GL_UNSIGNED_INT, nullptr, baseVertex);
		}
	}

	glBindVertexArray(0);
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

	InitVertices(pTerrain, vertices);

	std::vector<unsigned int> indices;
	int numQuads = (m_patchSize - 1) * (m_patchSize - 1);
	indices.resize(numQuads * 6);
	InitIndices(indices);

	CalcNormals(vertices, indices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void GeoMapGrid::InitIndices(std::vector<unsigned int>& indices)
{
	int Index = 0;

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

	assert(Index == indices.size());
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

			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				unsigned int Index0 = baseVertex + indices[i];
				unsigned int Index1 = baseVertex + indices[i + 1];
				unsigned int Index2 = baseVertex + indices[i + 2];
				glm::vec3 v1 = vertices[Index1].pos - vertices[Index0].pos;
				glm::vec3 v2 = vertices[Index2].pos - vertices[Index0].pos;
				glm::vec3 Normal = glm::cross(v1, v2);
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
