#version 330
layout (location = 0) in vec3 pos;

out vec4 vertexColor;

uniform mat4 model;


void main()
{
    gl_Position = model * vec4(pos, 1.0f);
    vertexColor = vec4(pos.x / 256.0f, pos.z / 256.0f, 1.0f, 1.0f);
}