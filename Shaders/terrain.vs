#version 330
layout (location = 0) in vec3 pos;

out vec4 vertexColor;

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

    vertexColor = vec4(c, c, 1.0 - c, 1.0);
}