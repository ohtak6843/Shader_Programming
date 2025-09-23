#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_STime;
in vec3 a_Vel;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592f;
const vec2 c_G = vec2(0.0, -9.8);

void main()
{
	float newTime = u_Time - a_STime;
	vec4 newPosition = vec4(a_Position, 1.f);
	vec3 velocity = normalize(a_Vel);
	if(newTime > 0)
	{
		float t = 2 * fract(newTime / 2.f);
		float tt = t * t;
		float x = 0;
		float y = 0.5 * c_G.y * tt;
		newPosition.xy += vec2(x, y);
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	gl_Position = newPosition;
	v_Color = a_Color;
}
