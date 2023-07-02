#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    TexCoords = aPos * 2.0;

    mat4 fix = mat4(mat3(view));
    gl_Position = projection * fix * vec4(aPos * 2.0, 1.0);
}