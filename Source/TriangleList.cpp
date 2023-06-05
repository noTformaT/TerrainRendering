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

	glDrawArrays(GL_POINTS, 0,m_depth * m_width);

	glBindVertexArray(0);
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

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);

	int POS_LOC = 0;

	glEnableVertexAttribArray(POS_LOC);

	size_t NumFloats = 0;
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices)
{
	int index = 0;

	for (int z = 0; z < m_depth; z++)
	{
		for (int x = 0; x < m_width; x++)
		{
			assert(index < vertices.size());
			vertices[index].InitVertex(pTerrain, x, z);
			index++;
		}
	}
}

void TriangleList::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	pos = glm::vec3(x, 0.0f, z);
}
