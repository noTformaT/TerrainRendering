#version 330
layout (location = 0) in vec3 pos;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
    gl_Position = model * vec4(pos, 1.0f);
    vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}