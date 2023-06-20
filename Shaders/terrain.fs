#version 330

in vec4 vertexColor;
in vec2 fragmentUV;
in vec3 worldPos;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform sampler2D myTexture3;
uniform sampler2D myTexture4;

uniform float gHeight0 = 64.0;
uniform float gHeight1 = 128.0;
uniform float gHeight2 = 193.0;
uniform float gHeight3 = 256.0;

layout(location = 0) out vec4 FragColor;

vec4 CalcTexColor()
{
    vec4 TexColor;

    float Height = worldPos.y;

    if (Height < gHeight0) {
       TexColor = texture(myTexture1, fragmentUV);
    } else if (Height < gHeight1) {
       vec4 Color0 = texture(myTexture1, fragmentUV);
       vec4 Color1 = texture(myTexture2, fragmentUV);
       float Delta = gHeight1 - gHeight0;
       float Factor = (Height - gHeight0) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight2) {
       vec4 Color0 = texture(myTexture2, fragmentUV);
       vec4 Color1 = texture(myTexture3, fragmentUV);
       float Delta = gHeight2 - gHeight1;
       float Factor = (Height - gHeight1) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight3) {
       vec4 Color0 = texture(myTexture3, fragmentUV);
       vec4 Color1 = texture(myTexture4, fragmentUV);
       float Delta = gHeight3 - gHeight2;
       float Factor = (Height - gHeight2) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else {
       TexColor = texture(myTexture4, fragmentUV);
    }

    return TexColor;
}

void main()
{
    FragColor = CalcTexColor();
    //FragColor = vec4(fragmentUV.x / 5.0f, fragmentUV.y / 5.0f, 1.0f, 1.0f);
    FragColor = vec4(fragmentUV.x / 5.0f, fragmentUV.y / 5.0f, vertexColor.x, 1.0f);
    /*if(fragmentUV.y > 0.5f)
    {
        if(fragmentUV.x > 0.5f)
        {
            FragColor = texture(myTexture1, fragmentUV * 5.0f);
        }
        else
        {
            FragColor = texture(myTexture2, fragmentUV * 5.0f);
        }
    }
    else
        {
        if(fragmentUV.x > 0.5f)
        {
            FragColor = texture(myTexture3, fragmentUV * 5.0f);
        }
        else
        {
            FragColor = texture(myTexture4, fragmentUV * 5.0f);
        }
    }*/
}