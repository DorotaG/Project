#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 color;
out vec3 normal;
out vec3 fPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = aColor;
	normal = mat3(transpose(inverse(model))) * aNormal;
	fPos = aPos;
	gl_Position =  projection * view * model * vec4(aPos, 1.0f);
	gl_PointSize = 30.0;
}