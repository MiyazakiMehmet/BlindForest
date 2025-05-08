#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 texCoords;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vec3 pos = aPos;

	fragPos = vec3(model * vec4(pos, 1.0));
    vec3 normal = normalize(vec3(0.0, 1.0, 0.0));
    fragNormal = normalize(mat3(model) * normal);

	texCoords = pos.xz * 0.4;
	gl_Position = projection * view * model * vec4(pos, 1.0);
}