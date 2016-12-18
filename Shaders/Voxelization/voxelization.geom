// Simple non-conservative voxelization.
// Implementation inspired by Cheng-Tso Lin: 
// https://github.com/otaku690/SparseVoxelOctree/blob/master/WIN/SVO/shader/voxelize.geom.glsl.
// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 worldPositionGeom[];
in vec3 normalGeom[];

out vec3 worldPositionFrag;
out vec3 normalFrag;

void main(){
	const vec3 p1 = worldPositionGeom[1] - worldPositionGeom[0];
	const vec3 p2 = worldPositionGeom[2] - worldPositionGeom[0];
	const vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i){
		worldPositionFrag = worldPositionGeom[i];
		normalFrag = normalGeom[i];
		if(p.z > p.x && p.z > p.y){
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.y, 0, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = vec4(worldPositionFrag.y, worldPositionFrag.z, 0, 1);
		} else {
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.z, 0, 1);
		}
		EmitVertex();
	}
    EndPrimitive();
}