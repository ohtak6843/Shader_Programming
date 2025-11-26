#version 330

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 FragColor1;

in vec4 v_Color;
in vec2 v_UV;

uniform sampler2D u_Texture;

void main()
{
	FragColor = v_Color * texture(u_Texture, v_UV);
	FragColor1 = vec4(1);
}