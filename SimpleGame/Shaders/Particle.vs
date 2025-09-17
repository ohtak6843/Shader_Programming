#version 330

in vec3 a_Position;
in float a_Value;

in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592f;

void main()
{
	
	vec2 dir = normalize(a_Position.xy);
	dir = normalize(dir);

	float radius = a_Value;
	float value = 0.5 * fract(u_Time); // [0, 0.5]
	float x = dir.x * value;
	float y = dir.y * value;
	vec4 newPosition = vec4(a_Position, 1.f);
	newPosition.xy += vec2(x, y);
	gl_Position = newPosition;

	v_Color = a_Color;
}
