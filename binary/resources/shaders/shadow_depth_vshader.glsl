#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uM;

void main() {
    gl_Position = uLightSpaceMatrix * uM * vec4(inPosition, 1.0);
}