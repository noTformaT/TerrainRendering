#include "SkyBox.h"
#include <stb_image.h>
#include <iostream>

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        GLuint texture = m_textures[i].textureID;
        glDeleteTextures(1, &texture);
    }
}

void SkyBox::Init()
{
	if (m_isInit == true)
	{
		return;
	}

	m_isInit = true;

	float vertices[] = {
        -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        0.5, -0.5,  0.5,
        0.5,  0.5,  0.5,

        -0.5,  0.5, 0.5,
        0.5,   0.5, 0.5,
        -0.5,  0.5,-0.5,
        -0.5,  0.5,-0.5,
        0.5,   0.5, 0.5,
        0.5,   0.5, -0.5,

        -0.5, 0.5, -0.5,
        0.5,  0.5, -0.5,
        -0.5, -0.5,-0.5,
        -0.5, -0.5,-0.5,
        0.5,  0.5, -0.5,
        0.5, -0.5, -0.5,

        -0.5, -0.5,-0.5,
        0.5, -0.5, -0.5,
        -0.5, -0.5, 0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, -0.5,
        0.5, -0.5,  0.5,

        0.5, -0.5,  0.5,
        0.5, -0.5, -0.5,
        0.5,  0.5,  0.5,
        0.5,  0.5,  0.5,
        0.5, -0.5, -0.5,
        0.5,  0.5, -0.5,

        -0.5, -0.5, -0.5,
        -0.5, -0.5,  0.5,
        -0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
        -0.5, -0.5,  0.5,
        -0.5,  0.5,  0.5,
	};
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_renderSystem.Init();

    std::vector<std::string> cubemap1
    {
        "Textures/Yokohama3/posx.jpg", // +x
        "Textures/Yokohama3/negx.jpg", // -x
        "Textures/Yokohama3/posy.jpg", 
        "Textures/Yokohama3/negy.jpg",
        "Textures/Yokohama3/posz.jpg",
        "Textures/Yokohama3/negz.jpg"
    };

    std::vector<std::string> cubemap2
    {
        "Textures/skybox/right.jpg", // +x
        "Textures/skybox/left.jpg", // -x
        "Textures/skybox/top.jpg",
        "Textures/skybox/bottom.jpg",
        "Textures/skybox/front.jpg",
        "Textures/skybox/back.jpg"
    };

    std::vector<std::string> cubemap3
    {
        "Textures/CoitTower2/posx.jpg", // +x
        "Textures/CoitTower2/negx.jpg", // -x
        "Textures/CoitTower2/posy.jpg",
        "Textures/CoitTower2/negy.jpg",
        "Textures/CoitTower2/posz.jpg",
        "Textures/CoitTower2/negz.jpg"
    };

    std::vector<std::string> cubemap4
    {
        "Textures/GamlaStan/posx.jpg", // +x
        "Textures/GamlaStan/negx.jpg", // -x
        "Textures/GamlaStan/posy.jpg",
        "Textures/GamlaStan/negy.jpg",
        "Textures/GamlaStan/posz.jpg",
        "Textures/GamlaStan/negz.jpg"
    };

    std::vector<std::string> cubemap5
    {
        "Textures/IceRiver/posx.jpg", // +x
        "Textures/IceRiver/negx.jpg", // -x
        "Textures/IceRiver/posy.jpg",
        "Textures/IceRiver/negy.jpg",
        "Textures/IceRiver/posz.jpg",
        "Textures/IceRiver/negz.jpg"
    };

    std::vector<std::string> cubemap6
    {
        "Textures/LancellottiChapel/posx.jpg", // +x
        "Textures/LancellottiChapel/negx.jpg", // -x
        "Textures/LancellottiChapel/posy.jpg",
        "Textures/LancellottiChapel/negy.jpg",
        "Textures/LancellottiChapel/posz.jpg",
        "Textures/LancellottiChapel/negz.jpg"
    };

    std::vector<std::string> cubemap7
    {
        "Textures/Lycksele3/posx.jpg", // +x
        "Textures/Lycksele3/negx.jpg", // -x
        "Textures/Lycksele3/posy.jpg",
        "Textures/Lycksele3/negy.jpg",
        "Textures/Lycksele3/posz.jpg",
        "Textures/Lycksele3/negz.jpg"
    };

    std::vector<std::string> cubemap8
    {
        "Textures/NiagaraFalls3/posx.jpg", // +x
        "Textures/NiagaraFalls3/negx.jpg", // -x
        "Textures/NiagaraFalls3/posy.jpg",
        "Textures/NiagaraFalls3/negy.jpg",
        "Textures/NiagaraFalls3/posz.jpg",
        "Textures/NiagaraFalls3/negz.jpg"
    };

    std::vector<std::string> cubemap9
    {
        "Textures/Plants/posx.jpg", // +x
        "Textures/Plants/negx.jpg", // -x
        "Textures/Plants/posy.jpg",
        "Textures/Plants/negy.jpg",
        "Textures/Plants/posz.jpg",
        "Textures/Plants/negz.jpg"
    };

    std::vector<std::string> cubemap10
    {
        "Textures/SanFrancisco4/posx.jpg", // +x
        "Textures/SanFrancisco4/negx.jpg", // -x
        "Textures/SanFrancisco4/posy.jpg",
        "Textures/SanFrancisco4/negy.jpg",
        "Textures/SanFrancisco4/posz.jpg",
        "Textures/SanFrancisco4/negz.jpg"
    };

    std::vector<std::string> cubemap11
    {
        "Textures/Yokohama/posx.jpg", // +x
        "Textures/Yokohama/negx.jpg", // -x
        "Textures/Yokohama/posy.jpg",
        "Textures/Yokohama/negy.jpg",
        "Textures/Yokohama/posz.jpg",
        "Textures/Yokohama/negz.jpg"
    };

    /*m_textures.push_back(LoadCubemap(cubemap1));
    m_textures.push_back(LoadCubemap(cubemap2));
    m_textures.push_back(LoadCubemap(cubemap3));
    m_textures.push_back(LoadCubemap(cubemap4));
    m_textures.push_back(LoadCubemap(cubemap5));
    m_textures.push_back(LoadCubemap(cubemap6));
    m_textures.push_back(LoadCubemap(cubemap7));
    m_textures.push_back(LoadCubemap(cubemap8));
    m_textures.push_back(LoadCubemap(cubemap9));
    m_textures.push_back(LoadCubemap(cubemap10));
    m_textures.push_back(LoadCubemap(cubemap11));*/

    LoadCubemap("Yokohama");
    
    m_renderSystem.Enable();
    m_renderSystem.SetInt("skybox", 0);
}

void SkyBox::Render(Camera& camera, GLuint width, GLuint height)
{
	//glFrontFace(GL_CW);
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);

    glViewport(0, 0, width, height);

	if (!m_isInit)
	{
		return;
	}

	m_renderSystem.Enable();

    glm::mat4 view = camera.GetView();
    glm::mat4 projection = camera.GetProjection();

    m_renderSystem.UpdateViewProjection(view, projection);


	glDepthMask(GL_FALSE);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[GetCubemapIndex()].textureID);
    glDrawArrays(GL_TRIANGLES, 0, 108);

	glDepthMask(GL_TRUE);
}

size_t SkyBox::GetCubemapIndex()
{
    return m_cubemapIndex;
}

size_t SkyBox::GetCubemapCount()
{
    return m_textures.size();
}

std::string SkyBox::GetCubemapName(size_t index)
{
    return m_textures[index].name;
}

void SkyBox::SetCubemapIndex(size_t index)
{
    m_cubemapIndex = index;
}

void SkyBox::LoadCubemap(std::string name)
{
    std::vector<std::string> names
    {
        std::string("Textures/") + name + "/posx.jpg",
        std::string("Textures/") + name + "/negx.jpg",
        std::string("Textures/") + name + "/posy.jpg",
        std::string("Textures/") + name + "/negy.jpg",
        std::string("Textures/") + name + "/posz.jpg",
        std::string("Textures/") + name + "/negz.jpg"
        //"Textures/Plants/posx.jpg", // +x
        //"Textures/Plants/negx.jpg", // -x
        //"Textures/Plants/posy.jpg",
        //"Textures/Plants/negy.jpg",
        //"Textures/Plants/posz.jpg",
        //"Textures/Plants/negz.jpg"
    };

    GLuint textureID = LoadCubemaps(names);

    TexturesData data;
    data.textureID = textureID;
    data.name = name;

    m_textures.push_back(data);
}

GLuint SkyBox::LoadCubemaps(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
