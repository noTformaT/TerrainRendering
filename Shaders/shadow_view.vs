#version 330

vec3 vertices[6] = vec3[](
    vec3(-1.0f, -1.0f, 0.0f),
    vec3(-1.0f,  1.0f, 0.0f),
    vec3( 1.0f,  1.0f, 0.0f),

    vec3( 1.0f,  1.0f, 0.0f),
    vec3( 1.0f,  -1.0f, 0.0f),
    vec3(-1.0f, -1.0f, 0.0f)
);

vec2 uvs[6] = vec2[](
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),

    vec2(1.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0)
);

out vec2 uv;

void main()
{
    gl_Position = vec4(vertices[gl_VertexID], 1.0);
    uv = uvs[gl_VertexID];
}