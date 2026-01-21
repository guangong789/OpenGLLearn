#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool hasSpecularMap;

void main() {
    vec3 albedo = texture(material.diffuse, TexCoord).rgb;
    // 环境光
    vec3 ambient = light.ambient * albedo * 0.3f;
    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * albedo);
    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specularColor = hasSpecularMap ? texture(material.specular, TexCoord).rgb : vec3(0.5f);
    vec3 specular = light.specular * (spec * specularColor);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}