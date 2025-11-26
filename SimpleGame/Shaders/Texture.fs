#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexID;
uniform float u_Time;

in vec2 v_Tex;

void Pixelized()
{
	vec2 newUV = v_Tex;
	float resol = 200 * (sin(u_Time * 10) + 1);
	newUV.x = floor(newUV.x * resol) / resol;
	newUV.y = floor(newUV.y * resol) / resol;

	FragColor = texture(u_TexID, vec2(newUV.x, 1 - newUV.y));
}

void main()
{
	//FragColor = vec4(v_Tex, 0, 1);
	//FragColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y));
	Pixelized();
}
