#version 330 core
in vec3 ourColor;
in vec2 ourTexture;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int activeTexture;

void main()
{
    if (activeTexture == 0)
    {
        FragColor = texture(texture1, ourTexture);
    }
    else
    {
        FragColor = texture(texture2, ourTexture);
    }
}
