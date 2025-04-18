#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D Albedo;

void main() {
    vec4 color = texture(Albedo, texCoord);
    if(color.a < 0.1)
        discard;
    FragColor = color;
}