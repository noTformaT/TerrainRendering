#include "TriangleList.h"
#include "Terrain.h"

TriangleList::TriangleList()
{
}

void TriangleList::CreateTriangleList(int width, int depth, const BaseTerrain* pTerrain)
{
	m_width = width;
	m_depth = depth;

	CreateGLState();
	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangleList::Render()
{
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, (m_depth - 1) * (m_width - 1) * 6, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void TriangleList::Destroy()
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

TriangleList::~TriangleList()
{
}

void TriangleList::CreateGLState()
{
	/*GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};*/

	glGenVertexArrays(1, &m_vao1);

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

	/*glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
}

void TriangleList::PopulateBuffers(const BaseTerrain* pTerrain)
{
	std::vector<Vertex> vertices;
	vertices.resize(m_width * m_depth);

	InitVertices(pTerrain, vertices);

	std::vector<unsigned int> indices;
	int numQuads = (m_width - 1) * (m_depth - 1);
	indices.resize(numQuads * 6);
	InitIndices(indices);

	CalcNormals(vertices, indices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void TriangleList::InitIndices(std::vector<unsigned int>& indices)
{
	int Index = 0;

	for (int x = 0; x < m_width - 1; x++) {
		for (int z = 0; z < m_depth - 1; z++) {
		
			unsigned int IndexBottomLeft = z * m_width + x;
			unsigned int IndexTopLeft = (z + 1) * m_width + x;
			unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
			unsigned int IndexBottomRight = z * m_width + x + 1;

			// Add top left triangle
			assert(Index < indices.size());
			indices[Index++] = IndexBottomLeft;
			assert(Index < indices.size());
			indices[Index++] = IndexTopLeft;
			assert(Index < indices.size());
			indices[Index++] = IndexTopRight;

			// Add bottom right triangle
			assert(Index < indices.size());
			indices[Index++] = IndexBottomLeft;
			assert(Index < indices.size());
			indices[Index++] = IndexTopRight;
			assert(Index < indices.size());
			indices[Index++] = IndexBottomRight;
		}
	}

	assert(Index == indices.size());
}

void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices)
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

void TriangleList::CalcNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	//unsigned int index = 0;

	//return;

	for (unsigned int i = 0;  i < indices.size(); i += 3)
	{
		unsigned int index0 = indices[i];
		unsigned int index1 = indices[i + 1];
		unsigned int index2 = indices[i + 2];

		glm::vec3 v1 = vertices[index1].pos - vertices[index0].pos;
		glm::vec3 v2 = vertices[index2].pos - vertices[index0].pos;

		glm::vec3 normal = glm::normalize(glm::cross(v2, v1));

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}
}

void TriangleList::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float worldScale = pTerrain->GetWorldScale();
	float y = pTerrain->GetHeight(x, z);
	//y = 0;
	y = 0.1f * y;
	pos = glm::vec3(-128 + x * worldScale, y, -128 + z * worldScale);
	
	float size = pTerrain->GetTerrainSize();

	uv = glm::vec2((float)x / size, (float)z / size);
}
