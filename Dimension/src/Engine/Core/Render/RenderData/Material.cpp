#include "Material.h"
#include "Texture.h"

Material::Material() {
	color = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	specularity = glm::vec2(0.0f, 0.0f);
}

Material* Material::SetColor(glm::vec4 color) {
	this->color = color;

	return this;
}
Material* Material::SetSpecularity(glm::vec2 specularity) {
	this->specularity = specularity;

	return this;
}

Material* Material::AddTexture(Texture texture, std::string name) {
	textures.insert(std::pair(name, texture));

	return this;
}

glm::vec4 Material::GetColor() {
	return color;
}
glm::vec2 Material::GetSpecularity() {
	return specularity;
}

Texture Material::GetTexture(std::string name) {
	return textures.at(name);
}

std::map<std::string, Texture> Material::GetTextures() {
	return textures;
}

Material::~Material() {
}