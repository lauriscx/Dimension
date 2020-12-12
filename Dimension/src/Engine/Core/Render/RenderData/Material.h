#include <glm/glm.hpp>
#include <map>
#include <string>

class Texture;

class Material {
public:
	Material();

	Material* SetColor(glm::vec4 color);
	Material* SetSpecularity(glm::vec2 specularity);

	Material* AddTexture(Texture texture, std::string name);

	glm::vec4 GetColor();
	glm::vec2 GetSpecularity();
	Texture GetTexture(std::string name);
	std::map<std::string, Texture> GetTextures();

	~Material();

	glm::vec4 color;
	glm::vec2 specularity;
private:

	std::map<std::string, Texture> textures;
};