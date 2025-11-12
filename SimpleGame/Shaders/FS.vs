#version 330

in vec3 a_Position;
in vec2 a_TexPos;

// a_Position [-1, 1]
// --> 0, 0 left top | 1, 1 right bottom
// u : (a_Position.x + 1) / 2
// v : (1 - a_Position.y) / 2

out vec2 v_UV;

void main()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;

	v_UV = a_TexPos;

	//v_UV.x = (a_Position.x + 1) / 2;
	//v_UV.y = (1 - a_Position.y) / 2;
}
