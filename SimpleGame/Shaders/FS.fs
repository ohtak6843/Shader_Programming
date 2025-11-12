#version 330

layout(location = 0) out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;

void main()
{
    FragColor = vec4(v_UV, 0, 1);
}
