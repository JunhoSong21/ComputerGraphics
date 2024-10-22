#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 0) in vec3 vPos;

out vec4 vertexColor;
uniform mat4 transform;
uniform mat4 modelTransform;

void main() {
	gl_Position = vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);
	vertexColor = in_Color;

	gl_Position = transform *vec4 (vPos, 1.0f);
	gl_Position = modelTransform *vec4(vPos, 1.0);
}