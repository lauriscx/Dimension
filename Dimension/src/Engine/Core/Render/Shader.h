#include <map>
#include <vector>

namespace Dimension {
	class Shader {
	public:
		Shader();

		void addVertexCode(std::string name, std::string code);
		void addGeometryCode(std::string name, std::string code);
		void addFragmentCode(std::string name, std::string code);

		int		getID	();
		int		program	();
		void	compile	();

		void start();
		void stop();

		void update();

		void addAttribute(std::string name, int value);
		void setAttributes();

		void addUniform(std::string name);
		void addUniform(std::string name, ...);
		void AddStructureUniform(std::string name, std::string variables, ...);

		void AddUniformArray(std::string name, int from, int to);
		void AddStructureUniformArray(std::string name, int from, int to, std::string variables, ...);
		void setupUniforms();
		void addTextures(std::string names, ...);
		void setupTextures();
		int  getTextureSlot(std::string name);
		void addFragment(std::string name, int index);
		void setupFragments();

		/* Implement uniform blocks */
		void AddUniformBlock(std::string name, int index, int size);
		void SetUniformBlockIndex(std::string name, int index);
		void SetUniformBlockSize(std::string name, int size);
		void SetUpUniformBlocks();
		void SendUniformBlocks(std::string name, void* data);

		void sendUniform(std::string name, int 		value);
		void sendUniform(std::string name, float 		value);
		void sendUniform(std::string name, double 	value);
		void sendUniform(std::string name, bool 	value);
		/*void sendUniform(std::string name, Vector2f 	value);
		void sendUniform(std::string name, Vector3f 	value);
		void sendUniform(std::string name, Vector4f 	value);
		void sendUniform(std::string name, Matrix2f 	value);
		void sendUniform(std::string name, Matrix3f 	value);
		void sendUniform(std::string name, Matrix4f 	value);*/
		void sendUniform(std::string name, int* 		value);
		void sendUniform(std::string name, float* 		value);
		void sendUniform(std::string name, double* 		value);
		void sendUniform(std::string name, bool* 		value);/*
		void sendUniform(std::string name, Vector2f[] value);
		void sendUniform(std::string name, Vector3f[] value);
		void sendUniform(std::string name, Vector4f[] value);
		void sendUniform(std::string name, Matrix2f[] value);
		void sendUniform(std::string name, Matrix3f[] value);
		void sendUniform(std::string name, Matrix4f[] value);*/

		/*void sendUniformDirectionLight(std::string name, std::vector<DirectionLight> value);
		void sendUniformPointLight(std::string name, std::vector<PointLight> 	value);
		void sendUniformSpotLight(std::string name, std::vector<SpotLight> 	value);*/

		void cleanUp();

		~Shader();
	private:
		int getLocation(std::string name);
		void Check(unsigned int& Shader, unsigned int Flag, bool IsProgram, const std::string& ErrorMessage);

	private:
		int ID;//Will be used in shader manager
		int programID;

		std::map<std::string, std::pair<std::string, int>> vertex;/*FileName, <Code, ShaderID>*/
		std::map<std::string, std::pair<std::string, int>> geometry;/*FileName, <Code, ShaderID>*/
		std::map<std::string, std::pair<std::string, int>> fragment;/*FileName, <Code, ShaderID>*/

		std::map<std::string, int> attributes;/*Name, UniformID*/
		std::map<std::string, int> fragments;/*Name, UniformID*/
		std::map<std::string, int> uniforms;/*Name, UniformID*/
		std::map<std::string, std::pair<int, int>> textures;/*Name, <SlotID, UniformID>*/
	};
}