#version 330

layout(location = 0) out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;
uniform sampler2D u_RGBTexture;

const float c_PI = 3.141592;

void Test()
{
    float s = 0.01;
    vec2 newPos = v_UV;
    newPos += vec2(0, 0.2 * sin(v_UV.x * c_PI * 2 + u_Time));
    vec4 newColor = texture(u_RGBTexture, newPos);
    FragColor = newColor;
}

void Circles()
{
    vec2 newUV = v_UV;
    vec2 center = vec2(0.5, 0.5);
    float d = distance(newUV, center);
    vec4 newColor = vec4(0);
    
    float value = sin(d * 4 * c_PI * 8 - u_Time * 5);

    newColor = vec4(value);
    FragColor = newColor;
}

void Flag()
{
    vec2 newUV = vec2(v_UV.x, (1 - v_UV.y) - 0.5);
    float sinValue = v_UV.x * 0.2 * sin(v_UV.x * 2 * c_PI - u_Time * 5);
    vec4 newColor = vec4(0);
    float width = 0.2 * abs(sin((1 - v_UV.x) * 1 * c_PI));

    if(sinValue + width > newUV.y && sinValue - width < newUV.y)
    {
        newColor = vec4(1);
    }
    else
    {
        discard;
    }

    FragColor = newColor;
}

void Q1()
{
    float newX = v_UV.x;
    float newY = 1 - abs((v_UV.y * 2) - 1);
    FragColor = texture(u_RGBTexture, vec2(newX, newY));
}

void Q2()
{
    float newX = fract(v_UV.x * 3);
    float newY = (2 - floor(v_UV.x * 3)) / 3 + v_UV.y / 3;
	FragColor = texture(u_RGBTexture, vec2(newX, newY));
}

void main()
{
    //Test();
    //Circles();
    //Flag();
    //Q1();
    Q2();
}
