#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_UV;

const float c_PI = 3.141592;

void main()
{
	float vx = 2 * c_PI * v_UV.x * 8;
	float vy = 2 * c_PI * v_UV.y * 8;
	//float greyScale = 1 - pow(abs(sin(vx) + sin(vy)), 0.5);
	float greyScale = 1 - pow(abs(sin(vx)), 0.5);

	vec4 newColor = vec4(greyScale);
	FragColor = newColor;
}
