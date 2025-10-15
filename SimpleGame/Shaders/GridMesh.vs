#version 330

in vec3 a_Position;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592;

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
	value = ceil(value); // 올림 함수
	value = (0.5 - d) * 100; // Anti-aliasing Effect
	*/

	float value = sin(d * 4 * c_PI * 10 - u_Time * 10);
	float strength = 1 - clamp(d * 2, 0, 1);
	value *= strength;

	v_Color = vec4(value);
}

void main()
{
	//Flag();
	Wave();
}
