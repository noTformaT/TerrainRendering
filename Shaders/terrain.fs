#version 330

in vec4 vertexColor;
in vec2 fragmentUV;
uniform sampler2D myTexture;

layout(location = 0) out vec4 FragColor;

void main()
{
    //FragColor = vec4(fragmentUV.x, fragmentUV.y, 1.0f, 1.0f);
    FragColor = texture(myTexture, fragmentUV * 5.0f);
}