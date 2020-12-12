#version 430

in vec3 Position;

uniform mat4 ModelTransformation;
uniform mat4 Camera;
uniform mat4 Projection;

void main () {
	vec4 worldPosition 	= ModelTransformation * vec4(Position, 1.0f);
	gl_Position 		= Projection * Camera * worldPosition;
}