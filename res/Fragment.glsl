#version 430

out vec4 Pixel;

void main () {
	Pixel = vec4(0.0, 0.5, 0.7, 1);// * (texture2D(DiffuseMap, Paralax) + material.color);
}