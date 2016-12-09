// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPositionGeom;
out vec3 normalGeom;

void main(){
	worldPositionGeom = vec3(M * vec4(position, 1));
	normalGeom = normalize(mat3(transpose(inverse(M))) * normal);
	gl_Position = P * V * vec4(worldPositionGeom, 1);
}