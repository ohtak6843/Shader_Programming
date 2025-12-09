#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexID;
uniform sampler2D u_TexID1;

uniform float u_Time;
uniform int u_Method; // 0: Normal, 1: blurH, 2: blurV, 3: merge

in vec2 v_Tex;

const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void Pixelized()
{
	vec2 newUV = v_Tex;
	float resol = 200 * (sin(u_Time * 10) + 1);
	newUV.x = floor(newUV.x * resol) / resol;
	newUV.y = floor(newUV.y * resol) / resol;

	FragColor = texture(u_TexID, vec2(newUV.x, 1 - newUV.y));
}

vec4 BlurH()
{             
    vec2 tex_offset = 1.0 / textureSize(u_TexID, 0); // gets size of single texel
    vec3 result = texture(u_TexID, v_Tex).rgb * weight[0]; // current fragment's contribution

    for(int i = 1; i < 5; ++i)
    {
        result += texture(u_TexID, v_Tex + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(u_TexID, v_Tex - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }

    return vec4(result, 1.0);
}

vec4 BlurV()
{             
    vec2 tex_offset = 1.0 / textureSize(u_TexID, 0); // gets size of single texel
    vec3 result = texture(u_TexID, v_Tex).rgb * weight[0]; // current fragment's contribution

    for(int i = 1; i < 5; ++i)
    {
        result += texture(u_TexID, v_Tex + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        result += texture(u_TexID, v_Tex - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    }

    return vec4(result, 1.0);
}

vec4 Merge()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y)).rgb;      
    vec3 bloomColor = texture(u_TexID1, v_Tex).rgb;
    hdrColor += bloomColor; 

    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * 2.2);

    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    return vec4(result, 1.0);
} 

void main()
{
	FragColor = vec4(0);
	if(u_Method == 0)
	{
		FragColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y));
	}
	else if(u_Method == 1)
	{
		FragColor = BlurH();
	}
	else if(u_Method == 2)
	{
		FragColor = BlurV();
	}
	else if(u_Method == 3)
	{
        FragColor = Merge();
	}
}
