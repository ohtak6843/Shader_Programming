#version 330

in vec3 a_Position;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592f;

void main()
{
	float radius = 0.5;
	float value = 2 * fract(u_Time) - 1; // [-1, 1]
	float rad = (value + 1) * c_PI; // [0, 2PI]
	float x = cos(rad);
	float y = sin(rad);
	vec4 newPosition = vec4(a_Position, 1.f);
	newPosition.xy += radius * vec2(x, y);
	gl_Position = newPosition;

	v_Color = a_Color;
}
