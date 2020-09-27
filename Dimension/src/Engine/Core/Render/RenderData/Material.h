#include <glm/glm.hpp>
#include <map>
#include <string>

class Texture;

class Material {
public:
	Material();

	void SetColor(glm::vec4 color);
	void SetSpecularity(glm::vec2 specularity);

	void AddTexture(Texture texture, std::string name);

	glm::vec4 GetColor();
	glm::vec2 GetSpecularity();
	Texture GetTexture(std::string name);
	std::map<std::string, Texture> GetTextures();

	~Material();

private:
	glm::vec4 color;
	glm::vec2 specularity;

	std::map<std::string, Texture> textures;
};