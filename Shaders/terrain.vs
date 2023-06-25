#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec4 vertexColor;
out vec2 fragmentUV;
out vec3 worldPos;
out vec3 fragmentNormal;

uniform mat4 model;
uniform float minHeight;
uniform float maxHeight;


void main()
{
    gl_Position = model * vec4(pos, 1.0f);
    //vertexColor = vec4(pos.x / (256.0f * 4.0f), pos.z / (256.0f * 4.0f), 1.0f, 1.0f);
    //vertexColor = vec4(pos.y / 200.0);

    float deltaHeight = maxHeight - minHeight;
    float heightRatio = (pos.y - minHeight) / deltaHeight;

    float c = heightRatio * 0.8 + 0.2;

    vertexColor = vec4(c, c, c, 1.0);

    fragmentUV = uv * 10.0;
    worldPos = pos;
    fragmentNormal = normal;

}