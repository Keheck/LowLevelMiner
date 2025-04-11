#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform vec3 lightPos;

void main() {
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;
    
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight = diff * lightColor;

    FragColor = vec4((diffuseLight + ambientLight) * objectColor, 1.0f);
}