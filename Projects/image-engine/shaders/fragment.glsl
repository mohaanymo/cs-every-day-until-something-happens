#version 330 core

out vec4 FragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0);

    float hue = uv.x;

    vec3 color = vec3(
        abs(sin(hue * 6.28318)),
        abs(sin(hue * 6.28318 + 2.094)),
        abs(sin(hue * 6.28318 + 4.188))
    );

    FragColor = vec4(color, 1.0);
}
