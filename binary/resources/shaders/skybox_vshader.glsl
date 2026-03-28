#version 330 core

layout (location = 0) in vec3 inPosition;

out vec3 TexCoords;

uniform mat4 uP;
uniform mat4 uV;

void main() {
    TexCoords = inPosition;

    vec4 pos = uP * uV * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}