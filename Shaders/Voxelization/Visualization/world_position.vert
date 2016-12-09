// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPosition;

void main(){
	worldPosition = vec3(M * vec4(position, 1));
	gl_Position = P * V * vec4(worldPosition, 1);
}