#version 330

layout(location = 0) out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;

const int DROP_COUNT = 8;           // 빗방울 개수
const float waveSpeed = 0.1;        // 파동 퍼지는 속도 낮춤
const float waveFreq  = 300.0;      // 파동 세부 형태
const float damping   = 30.0;       // 거리 감쇠 → 범위 좁게
const float amplitude = 0.18;       // 파동 세기
const float dropInterval = 2.8;     // 빗방울 간격

// 단순 난수 생성
vec2 random2(float seed)
{
    return vec2(
        fract(sin(seed * 12.9898) * 43758.5453),
        fract(sin(seed * 78.233) * 12345.6789)
    );
}

// 빗방울 위치: 시간 기반 랜덤
vec2 getDropPos(int i, float t)
{
    float seed = float(i) * 10.123 + floor(t / dropInterval);
    return random2(seed);
}

void main()
{
    float totalIntensity = 0.0;
    float totalDuration = dropInterval * float(DROP_COUNT);
    float t = mod(u_Time, totalDuration);

    for (int i = 0; i < DROP_COUNT; ++i)
    {
        float startTime = float(i) * dropInterval;
        float elapsed = t - startTime;
        if (elapsed < 0.0 || elapsed > dropInterval) continue;

        vec2 dropPos = getDropPos(i, u_Time);
        float dist = distance(v_UV, dropPos);

        // 원형 파동
        float wave = sin(dist * waveFreq - elapsed * waveSpeed);

        // 거리 감쇠 강화, 시간 감쇠 완화
        wave *= exp(-dist * damping);
        wave *= exp(-elapsed * 0.1); // 시간 감쇠 느리게 → 오래 남음

        totalIntensity += wave;
    }

    float grey = 0.5 + amplitude * totalIntensity;

    vec3 baseColor = vec3(0.15, 0.3, 0.6);
    vec3 color = baseColor + vec3(grey * 0.8, grey * 0.9, grey);

    FragColor = vec4(color, 1.0);
}
