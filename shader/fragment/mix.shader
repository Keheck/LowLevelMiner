#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D Albedo;
uniform sampler2D Secondary;

void main() {
    FragColor = mix(texture(Albedo, texCoord), texture(Secondary, texCoord), 0.2);
}