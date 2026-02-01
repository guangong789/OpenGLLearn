#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

#define MAX_POINT_LIGHTS 8

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float diffuseStrength;
};

struct DirLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform vec3 viewPos;

uniform DirLight dirlight;
uniform PointLight pointlights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform SpotLight spotlight;

// --------------------------------------------------

void CalcBlinnPhong(vec3 lightDir, vec3 normal, vec3 viewDir, float shininess, out float diff, out float spec) {
    diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, vec3 specColor) {
    vec3 lightDir = normalize(-light.direction);

    float diff, spec;
    CalcBlinnPhong(lightDir, normal, viewDir, material.shininess, diff, spec);
    vec3 diffuse = light.diffuse * diff * albedo * material.diffuseStrength;
    vec3 specular = light.specular * spec * specColor;

    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, vec3 specColor) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff, spec;
    CalcBlinnPhong(lightDir, normal, viewDir, material.shininess, diff, spec);
    vec3 diffuse = light.diffuse * diff * albedo * material.diffuseStrength;
    vec3 specular = light.specular * spec * specColor;

    vec3 delta = light.position - fragPos;
    float dist2 = dot(delta, delta);
    float dist  = sqrt(dist2);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist2);

    return (diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, vec3 specColor) {
    float epsilon = max(light.cutoff - light.outercutoff, 0.0001);
    if (epsilon <= 0.0) return vec3(0.0);

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);

    float diff, spec;
    CalcBlinnPhong(lightDir, normal, viewDir, material.shininess, diff, spec);

    vec3 diffuse = light.diffuse * diff * albedo * material.diffuseStrength;
    vec3 specular = light.specular * spec * specColor;

    vec3 delta = light.position - fragPos;
    float dist2 = dot(delta, delta);
    float dist = sqrt(dist2);
    float attenuation = 1.0 / max(
        light.constant + light.linear * dist + light.quadratic * dist2,
        0.0001
    );

    return (diffuse + specular) * attenuation * intensity;
}

// --------------------------------------------------

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 albedo = texture(material.diffuse, TexCoord).rgb;
    vec3 specColor = texture(material.specular, TexCoord).rgb;

    vec3 result = vec3(0.0);
    result += CalcDirLight(dirlight, normal, viewDir, albedo, specColor);
    for (int i = 0; i < numPointLights; ++i) {
        result += CalcPointLight(pointlights[i], normal, FragPos, viewDir, albedo, specColor);
    }
    result += CalcSpotLight(spotlight, normal, FragPos, viewDir, albedo, specColor);

    FragColor = vec4(result, 1.0);
}