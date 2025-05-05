#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 fragPos;

void main(){
	vec3 pos = aPos;

	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	fragPos = pos;
}