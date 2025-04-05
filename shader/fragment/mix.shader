#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D face;

void main() {
    FragColor = mix(texture(tex, texCoord), texture(face, texCoord), 0.2);
}