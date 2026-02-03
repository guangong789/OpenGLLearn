#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D uColor;
uniform sampler2D uDepth;
uniform vec2 uTexelSize;
uniform vec3 uOutlineColor;
uniform float uThreshold;
uniform float uThickness;
uniform float uNear;
uniform float uFar;
uniform int uEnableOutline;

// Convert depth value to linear depth
float linearizeDepth(float z) {
    float ndc = z * 2.0 - 1.0;
    return (2.0 * uNear * uFar) / (uFar + uNear - ndc * (uFar - uNear));
}

void main() {
    vec3 baseColor = texture(uColor, TexCoord).rgb;

    vec2 off = uTexelSize * max(uThickness, 1.0);
    float dC = linearizeDepth(texture(uDepth, TexCoord).r) / uFar;
    float dL = linearizeDepth(texture(uDepth, TexCoord + vec2(-off.x, 0.0)).r) / uFar;
    float dR = linearizeDepth(texture(uDepth, TexCoord + vec2( off.x, 0.0)).r) / uFar;
    float dU = linearizeDepth(texture(uDepth, TexCoord + vec2(0.0,  off.y)).r) / uFar;
    float dD = linearizeDepth(texture(uDepth, TexCoord + vec2(0.0, -off.y)).r) / uFar;

    if (uEnableOutline == 0) {
        FragColor = vec4(baseColor, 1.0);
        return;
    }

    float edge = 0.0;
    edge = max(edge, abs(dC - dL));
    edge = max(edge, abs(dC - dR));
    edge = max(edge, abs(dC - dU));
    edge = max(edge, abs(dC - dD));

    float mask = edge > uThreshold ? 1.0 : 0.0;
    vec3 finalColor = mix(baseColor, uOutlineColor, mask);
    FragColor = vec4(finalColor, 1.0);
}
