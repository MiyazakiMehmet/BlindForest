#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 texCoords;

void main() {
    fragPos = vec3(model * vec4(aPos, 1.0));

    // Hardcoded up-facing normal for now; ideal is to pass real normals
    fragNormal = mat3(transpose(inverse(model))) * aNormal; 

    texCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}