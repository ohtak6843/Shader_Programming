#version 330

layout(location = 0) out vec4 FragColor;

uniform float u_Time;
uniform sampler2D u_RGBTexture;

const float c_PI = 3.141592;

in vec2 v_UV;

void main()
{
    vec2 uv = v_UV;
    uv += vec2(0, 0.2 * sin(v_UV.x * c_PI * 2 + u_Time * 0.01));
    vec4 newColor = texture(u_RGBTexture, uv);
    FragColor = newColor;

    //FragColor = vec4(u_UV, 0, 1);
}
