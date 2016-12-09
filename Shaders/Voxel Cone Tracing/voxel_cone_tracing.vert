// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPositionFrag;
out vec3 normalFrag;

void main(){
	worldPositionFrag = vec3(M * vec4(position, 1));
	normalFrag = normalize(mat3(transpose(inverse(M))) * normal);
	gl_Position = P * V * vec4(worldPositionFrag, 1);
}