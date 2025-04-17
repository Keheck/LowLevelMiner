#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Albedo;

void main() {
    vec3 baseColor = texture(Albedo, texCoord).rgb;

    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;
    
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specularLight = specularStrength * spec * lightColor;

    FragColor = vec4((diffuseLight + ambientLight + specularLight) * baseColor, 1.0);
}