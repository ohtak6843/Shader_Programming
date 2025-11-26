#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragColor1;

in vec2 v_UV;

uniform float u_Time;
uniform sampler2D u_RGBTexture;
uniform sampler2D u_NumTexture;
uniform sampler2D u_TotalNumTexture;
uniform int u_Number;

const float c_PI = 3.141592;

vec4 Test()
{
    float s = 0.01;
    vec2 newPos = v_UV;
    newPos += vec2(0, 0.2 * sin(v_UV.x * c_PI * 2 + u_Time));
    vec4 newColor = texture(u_RGBTexture, newPos);
    return newColor;
}

vec4 Circles()
{
    vec2 newUV = v_UV;
    vec2 center = vec2(0.5, 0.5);
    float d = distance(newUV, center);
    vec4 newColor = vec4(0);
    
    float value = sin(d * 4 * c_PI * 8 - u_Time * 5);

    newColor = vec4(value);
    return newColor;
}

vec4 Flag()
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

    return newColor;
}

vec4 Q1()
{
    float newX = v_UV.x;
    float newY = 1 - abs((v_UV.y * 2) - 1);
    return texture(u_RGBTexture, vec2(newX, newY));
}

vec4 Q2()
{
    float newX = fract(v_UV.x * 3);
    float newY = (2 - floor(v_UV.x * 3)) / 3 + v_UV.y / 3;
	return texture(u_RGBTexture, vec2(newX, newY));
}

vec4 Q3()
{
    float newX = fract(v_UV.x * 3);
    float newY = floor(v_UV.x * 3) / 3 + v_UV.y / 3;
    return texture(u_RGBTexture, vec2(newX, newY));
}

vec4 Q4()
{
    // uniform 변수로 외부에서 받을 수 있음
    float count = 8;
    float shift = 0.1 * u_Time;

    float newX = fract(fract(v_UV.x * count) + (floor(v_UV.y * count) + 1) * shift);
    float newY = fract(v_UV.y * count);
    return texture(u_RGBTexture, vec2(newX, newY));
}

vec4 Q5()
{
    float count = 2;
	float shift = 0.5;

	float newX = fract(v_UV.x * count);
	float newY = fract(fract(v_UV.y * count) + floor(v_UV.x * count) * shift);
	return texture(u_RGBTexture, vec2(newX, newY));
}

vec4 Number()
{
    return texture(u_NumTexture, v_UV);
}

vec4 TotalNumber()
{
    int n = u_Number % 10;
    if(n < 0) n += 10;

    int tileIndex = (n + 9) % 10;

    int tileX = tileIndex % 5;
    int tileY = tileIndex / 5;

    vec2 tileSize = vec2(1.0 / 5.0, 1.0 / 2.0);
    vec2 tileOrigin = vec2(float(tileX), float(tileY)) * tileSize;

    vec2 sampleUV = tileOrigin + v_UV * tileSize;

    return texture(u_TotalNumTexture, sampleUV);
}

vec4 DigitNumber()
{
    // 출력 가능한 최대 자릿수
    const int MAX_DIGITS = 5;
    // 자릿수 폭
    float digitWidth = 1.0 / float(MAX_DIGITS);

    // 자릿수별로 숫자를 분해
    int num = u_Number;
    int digits[MAX_DIGITS];
    for(int i = 0; i < MAX_DIGITS; i++)
    {
        digits[i] = num % 10;
        num /= 10;
    }
    // 이제 digits[0] = 일의 자리, digits[1] = 십의 자리, … digits[4] = 만의 자리

    // 현재 fragment가 어느 자리 (x축) 범위에 속하는지 판단
    int digitIndex = int(v_UV.x / digitWidth);
    if(digitIndex < 0 || digitIndex >= MAX_DIGITS)
    {
        return vec4(0.0);
    }

    // 각 자리 영역 내에서의 로컬 UV
    vec2 localUV = vec2(
        (v_UV.x - float(digitIndex) * digitWidth) / digitWidth,
        v_UV.y
    );

    // 출력할 숫자 (왼쪽부터 큰 자리수) → digits[MAX-1-digitIndex]
    int digit = digits[MAX_DIGITS - 1 - digitIndex];

    // 타일맵 계산 (5col × 2row)
    int n = digit % 10;
    if(n < 0) n += 10;
    int tileIndex = (n + 9) % 10;

    int tileX = tileIndex % 5;
    int tileY = tileIndex / 5;

    vec2 tileSize = vec2(1.0 / 5.0, 1.0 / 2.0);
    vec2 tileOrigin = vec2(float(tileX), float(tileY)) * tileSize;

    vec2 sampleUV = tileOrigin + localUV * tileSize;

    return texture(u_TotalNumTexture, sampleUV);
}

vec4 Pinwheel()
{
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = normalize(v_UV - center);

    float angle = atan(dir.y, dir.x); // -PI ~ PI
    angle += u_Time * 2.0; // 회전

    float stripes = sin(angle * 6.0);
    stripes = (stripes + 1.0) * 0.5;

    return vec4(vec3(stripes), 1.0);
}

vec4 Moire()
{
    vec2 center = vec2(0.5, 0.5);
    float d = distance(v_UV, center);

    float v1 = sin(d * 30.0 - u_Time * 5.0);
    float v2 = cos(d * 35.0 + u_Time * 3.0);

    float result = (v1 + v2) * 0.5;

    result = (result + 1.0) * 0.5;

    return vec4(vec3(result), 1.0);
}

vec4 EnergyBurst()
{
    vec2 center = vec2(0.5, 0.5);
    float d = distance(v_UV, center);

    float shock = sin(d * 50.0 - u_Time * 10.0);
    shock = max(shock, 0.0);

    float ring = smoothstep(0.05, 0.0, abs(d - fract(u_Time * 0.2)));

    float color = shock * ring;

    return vec4(color, color * 0.5, 1.0, 1.0);
}

vec4 Dispersion()
{
    vec2 center = vec2(0.5, 0.5);
    float d = distance(v_UV, center);

    float base = sin(d * 25.0 - u_Time * 5.0);

    float r = sin(base + 0.0);
    float g = sin(base + 1.0);
    float b = sin(base + 2.0);

    return vec4(r, g, b, 1.0);
}

vec4 CyberpunkCircles()
{
    vec2 newUV = v_UV;
    vec2 center = vec2(0.5, 0.5);
    float d = distance(newUV, center);

    float value = sin(d * 4 * c_PI * 8 - u_Time * 5);

    // Pulse wave
    float pulse = smoothstep(0.2, 0.0, abs(sin(d * 10.0 - u_Time * 4.0)));
    value += pulse * 0.3;

    // Glow
    float glow = pow(1.0 - d, 3.0);
    value += glow * 0.2;

    // Noise
    float noise = fract(sin(dot(v_UV * 20.0 + u_Time, vec2(12.989,78.233))) * 43758.5453123);
    noise = (noise - 0.5) * 0.2;
    value += noise;

    // Color shift
    vec3 col = vec3(
        sin(value + u_Time),
        sin(value + u_Time * 1.5),
        sin(value + u_Time * 2.0)
    );

    return vec4(col * 0.5 + value * 0.5, 1.0);
}

void main()
{
    //Test();
    //Circles();
    //Flag();
    //Q1();
    //Q2();
    //Q3();
    //Q4();
    //Q5();
    //Number();
    //TotalNumber();
    //DigitNumber();
    FragColor = Pinwheel();
    FragColor1 = CyberpunkCircles();
}
