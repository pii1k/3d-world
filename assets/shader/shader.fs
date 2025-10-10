#version 330 core
in vec3 ourColor;
in vec2 ourTexture;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix_value;

void main()
{
    vec4 texColor = texture(texture1, ourTexture);
    FragColor = mix(texture(texture1, ourTexture), texture(texture2, ourTexture), mix_value);
}