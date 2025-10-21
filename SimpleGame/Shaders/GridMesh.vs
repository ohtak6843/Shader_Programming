#version 330
#define MAX_POINTS 500

in vec3 a_Position;

out vec4 v_Color;

uniform float u_Time;
uniform int u_DropCount;
uniform vec4 u_Points[MAX_POINTS];

const float c_PI = 3.141592;
/*const vec4 c_Points[3] = vec4[](vec4(0, 0, 2, 2),
								vec4(0.5, 0, 3, 3),
								vec4(-0.5, 0, 4, 4));*/

void Flag()
{
	vec4 newPosition = vec4(a_Position, 1.f);

	float value = (a_Position.x + 0.5) * 2.f * c_PI; // [0, 2PI]
	float value1 = a_Position.x + 0.5; // [0, 1]
	vec2 deltaPos = 0.3 * value1 * vec2(0.f, sin(value + u_Time * 10));
	deltaPos.y *= 0.5f;
	
	newPosition.y *= (1 - value1);
	newPosition.xy += deltaPos;

	gl_Position = newPosition;

	float shading = (sin(value - u_Time * 10) + 1) / 2 + 0.2f; // [0.2f, 1.2f]
	v_Color = vec4(shading);
}

void Wave()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;

	float d = distance(a_Position.xy, vec2(0.f, 0.f));
	
	if(d < 0.5)
	{
		v_Color = vec4(1.f);
	}
	else
	{
		v_Color = vec4(0.f);
	}

	/*
	float value = clamp(0.5f - d, 0, 1);
	value = ceil(value);
	value = (0.5 - d) * 100; // Anti-aliasing Effect
	*/

	float value = sin(d * 4 * c_PI * 10 - u_Time * 10);
	float p = 1 - clamp(d * 2, 0, 1);

	v_Color = vec4(value * p);
}

void RainDrop()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;

	vec2 pos = a_Position.xy;
	float newColor = 0;
	for(int i = 0; i < u_DropCount; i++)
	{
		vec2 cen = u_Points[i].xy;
		float range = 0.5;
		float sTime = u_Points[i].z;
		float lTime = u_Points[i].w;

		float newTime = u_Time - sTime;
		if(newTime > 0)
		{
			float baseTime = fract(newTime / lTime);
			float oneMinus = 1 - baseTime;
			float t = baseTime * lTime;
			float range = t / 10;
			float d = distance(pos, cen);
			float value = sin(d * 4 * c_PI * 10 - t * 10);
			float p = 30 * clamp(range - d, 0, 1);

			newColor += value * p * oneMinus;
		}
	}

	v_Color = vec4(newColor);
}

void main()
{
	//Flag();
	//Wave();
	RainDrop();
}
