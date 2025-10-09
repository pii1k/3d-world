#version 330 core
in vec3 ourColor;
in vec2 ourTexture;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, ourTexture);
    FragColor = texColor * vec4(ourColor, 1.0);
}
