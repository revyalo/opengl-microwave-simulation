#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexture;
layout (location = 3) in vec3 inTangent;

uniform mat4 uN;
uniform mat4 uM;
uniform mat4 uPVM;
uniform mat4 uLightSpaceMatrix;

out vec3 vnor;
out vec3 vpos;
out vec2 vtex;
out vec4 vposLightSpace;

out vec3 vtan;
out vec3 vbit;

void main() {

    vec4 inPositionH = vec4(inPosition, 1.0);

    vec4 worldPos = uM * inPositionH;

    gl_Position = uPVM * inPositionH;

    vec3 N = normalize((uN * vec4(inNormal, 0.0)).xyz);

    // La tangente se transforma con la matriz de modelo.
    vec3 T = normalize((uM * vec4(inTangent, 0.0)).xyz);

    // Re-ortogonalización para evitar errores si hay escalados.
    T = normalize(T - dot(T, N) * N);

    vec3 B = normalize(cross(N, T));

    vnor = N;
    vtan = T;
    vbit = B;

    vpos = worldPos.xyz;
    vtex = inTexture;
    vposLightSpace = uLightSpaceMatrix * worldPos;
}