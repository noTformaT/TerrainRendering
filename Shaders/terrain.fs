#version 330

in vec4 vertexColor;
in vec2 fragmentUV;
in vec3 worldPos;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform sampler2D myTexture3;
uniform sampler2D myTexture4;
uniform sampler2D myTexture5;

uniform float gHeight0 = 0.0;
uniform float gHeight1 = 0.0;
uniform float gHeight2 = 0.0;
uniform float gHeight3 = 0.0;
uniform float gHeight4 = 0.0;
uniform float gHeight5 = 0.0;

uniform float minHeight;
uniform float maxHeight;

layout(location = 0) out vec4 FragColor;

vec4 calcLayers()
{
    vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);
    float height = worldPos.y / maxHeight;
    float delta = 0.0f;
    float factor = 0.0f;
    vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 color2 = vec4(0.0, 0.0, 0.0, 1.0);

    if (height <= gHeight0)
    {
        outColor = texture(myTexture1, fragmentUV);
    }
    
    if (height > gHeight0 && height <= gHeight1)
    {
        delta = gHeight1 - gHeight0;
        factor = (height - gHeight0) / delta;

        color1 = texture(myTexture1, fragmentUV);
        color2 = texture(myTexture2, fragmentUV);

        outColor = mix(color1, color2, factor);
    }

    if (height > gHeight1 && height <= gHeight2)
    {
        outColor = texture(myTexture2, fragmentUV);
    }

    if (height > gHeight2 && height <= gHeight3)
    {
        delta = gHeight3 - gHeight2;
        factor = (height - gHeight2) / delta;

        color1 = texture(myTexture2, fragmentUV);
        color2 = texture(myTexture3, fragmentUV);

        outColor = mix(color1, color2, factor);
    }

    if (height > gHeight3 && height <= gHeight4)
    {
        outColor = texture(myTexture3, fragmentUV);
    }

    if (height > gHeight4 && height <= gHeight5)
    {
        delta = gHeight5 - gHeight4;
        factor = (height - gHeight4) / delta;

        color1 = texture(myTexture3, fragmentUV);
        color2 = texture(myTexture4, fragmentUV);

        outColor = mix(color1, color2, factor);
    }

    if (height > gHeight5)
    {
        outColor = texture(myTexture4, fragmentUV);
    }

    vec4 checker = texture(myTexture5, fragmentUV);

    //outColor = checker * 2 * outColor;

    return outColor;
}

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
    //FragColor = vec4(fragmentUV.x / 5.0f, fragmentUV.y / 5.0f, vertexColor.x, 1.0f);
    FragColor = calcLayers();
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