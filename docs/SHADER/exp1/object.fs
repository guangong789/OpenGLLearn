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

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionlLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform PointLight pointlight;
uniform DirectionlLight directionlight;
uniform vec3 viewPos;
uniform bool hasSpecularMap;
uniform bool useDirectionLight;

void main() {
    vec3 albedo = texture(material.diffuse, TexCoord).rgb;

    // 1. 环境光
    vec3 ambient = (useDirectionLight ? directionlight.ambient : pointlight.ambient) * albedo * 0.3;

    // 2. 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = useDirectionLight ? normalize(-directionlight.direction) : normalize(pointlight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (useDirectionLight ? directionlight.diffuse : pointlight.diffuse) * (diff * albedo);

    // 3. 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    vec3 specularColor = hasSpecularMap ? texture(material.specular, TexCoord).rgb : vec3(0.5);
    vec3 specular = (useDirectionLight ? directionlight.specular : pointlight.specular) * (spec * specularColor);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}