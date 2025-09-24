#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_STime;
in vec3 a_Vel;
in float a_LifeTime;
in float a_Mass;

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Force;

const float c_PI = 3.141592f;
const vec2 c_G = vec2(0.0, -9.8);

void main()
{
	float lifeTime = a_LifeTime;
	float newAlpha = 1;
	float newTime = u_Time - a_STime;
	vec4 newPosition = vec4(a_Position, 1.f);
	if(newTime > 0)
	{
		float fX = c_G.x * a_Mass + u_Force.x;
		float fY = c_G.y * a_Mass + u_Force.y;
		float aX = fX / a_Mass;
		float aY = fY / a_Mass;

		float t = lifeTime * fract(newTime / lifeTime);
		float tt = t * t;
		float x = a_Vel.x * t + 0.5 * aX * tt; // x = v0xt + 0.5at^2
		float y = a_Vel.y * t + 0.5 * aY * tt;
		newPosition.xy += vec2(x, y);
		newAlpha = 1 - t / lifeTime;
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}
