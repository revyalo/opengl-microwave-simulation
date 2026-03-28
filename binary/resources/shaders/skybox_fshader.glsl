#version 330 core

in vec3 TexCoords;

out vec4 outColor;

uniform samplerCube uSkybox;

void main() {
    outColor = texture(uSkybox, TexCoords);
}