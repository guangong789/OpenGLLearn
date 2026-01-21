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
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    int type; // 0 = directional, 1 = point, 2 = spot
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool hasSpecularMap;

void main()
{
    vec3 albedo = texture(material.diffuse, TexCoord).rgb;
    vec3 norm   = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // 光照方向
    vec3 lightDir;
    if (light.type == 0) lightDir = normalize(-light.direction);
    else lightDir = normalize(light.position - FragPos);
    // Ambient
    vec3 ambient = light.ambient * albedo * 0.3;
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo * 0.8;
    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specularColor = hasSpecularMap ? texture(material.specular, TexCoord).rgb : vec3(0.5);
    vec3 specular = light.specular * spec * specularColor;
    // Attenuation
    float attenuation = 1.0;
    if (light.type != 0) {
        float dist = length(light.position - FragPos);
        attenuation = 1.0 / (
            light.constant +
            light.linear * dist +
            light.quadratic * dist * dist
        );
    }

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}