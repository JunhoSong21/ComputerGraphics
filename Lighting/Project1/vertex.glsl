#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;
out vec3 normal;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(position, 1.0);
	fragColor = vec3(modelTransform * vec4(position, 1.0));

	normal = color;
}