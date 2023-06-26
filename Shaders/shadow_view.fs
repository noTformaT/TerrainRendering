#version 330
out vec4 color;

in vec2 uv;

uniform sampler2D depthTexture;

void main()
{
    float red = texture(depthTexture, uv).r;

    color =  vec4(red, red, red, 1.0f);
}