#include "imgui.h"
#include "GUI/imgui_impl_glfw.h"
#include "GUI/imgui_impl_opengl3.h"
#include "GLAD/glad.h"
#include "Aplication.h"
#include <iostream>
#include "Input/Input.h"
#include "GLFW/glfw3.h"
#include <functional>
#include "Layers/Layer.h"
#include "Temporary/Logger.h"
#include "Input/Events/Error.h"
#include <string>
#include "Render/RenderData/Texture.h"
#include "../Core/Render/RenderData/GraphicObject.h"
#include "../../Utils/FilesScaner.h"

#include "glm/gtc/matrix_transform.hpp"

#include "FileReader.h"
#include "Render/Render2D.h"
#include <fstream>
#include "../../Utils/LoadObj.h"
#include "Render/Camera.h"

Dimension::Aplication*	Dimension::Aplication::app;
static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

/*Variables for interface*/
static bool Vsync = true;
static bool FullScreen = false;
static bool showDemo = false;
static bool showCameraControlls = false;
static bool showEntytiesControlls = false;
static bool showResourcesControlls = false;
static std::vector<std::string> paths;
static std::map<std::string, bool> SelectedResources;
static std::map<std::string, Texture> Loadedtextures;
static std::map<std::string, objl::Loader> LoadedObjs;
static char str[100] = { "../res/" };
static std::vector<GraphicObject> GraphicObjects;
static GraphicObject * SelectedObject = nullptr;
static Texture * defaultTexture = nullptr;
static  objl::Loader * defaultObj = nullptr;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	
	Dimension::Aplication::app = this;
	window = Window::Create(title, width, height);
	events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
	window->EventsHandler(&events);
	if (gladLoadGL() == 0) {
		DERROR("Failed to load glad");
	}
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
void RenderUI();
glm::vec3 position(0);
void Dimension::Aplication::Run() {
	Layer * layer = new Layer("Test");
	m_Layers.PushLayer(layer);

	IMGUI_CHECKVERSION();	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->Context(), true);	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();	// Setup Dear ImGui style

	FileReader reader;
	Shader shader;
	shader.addVertexCode("/res/Vertex.glsl", reader.ReadFile("../res/Vertex.glsl"));
	shader.addFragmentCode("/res/Fragment.glsl", reader.ReadFile("../res/Fragment.glsl"));
	shader.compile();

	Texture texture;
	texture.Bind()->LoadPNG("../res/texture.png")->LoadData()->SetParameters();

	GraphicObject sprite;
	sprite.GetMaterial()->SetColor({ 0, 1, 1, 1 })->AddTexture(texture, "diffuseMap");
	sprite.position = glm::vec3(0, 0.0f, 0);
	sprite.rotation = glm::vec3(0, 0, 45.0f);
	sprite.scale = glm::vec3(0.05f, 0.05f, 0.05f);

	//sprite.Indices = new std::vector<int>();
	sprite.Indices.push_back(1);
	sprite.Indices.push_back(2);
	sprite.Indices.push_back(0);
	sprite.Indices.push_back(0);
	sprite.Indices.push_back(2);
	sprite.Indices.push_back(3);

	float _width = 0.5f;
	float _height = 0.1f;

	sprite.Positions.push_back(-_width / 2.0f);
	sprite.Positions.push_back(_height / 2.0f + 0.5);
	sprite.Positions.push_back(0.0f);
	sprite.Positions.push_back(-_width / 2.0f);
	sprite.Positions.push_back(-_height / 2.0f + 0.5);
	sprite.Positions.push_back(0.0f);
	sprite.Positions.push_back(_width / 2.0f);
	sprite.Positions.push_back(-_height / 2.0f + 0.5);
	sprite.Positions.push_back(0.0f);
	sprite.Positions.push_back(_width / 2.0f);
	sprite.Positions.push_back(_height / 2.0f + 0.5);
	sprite.Positions.push_back(0.0f);

	sprite.TexturesCoordinates.push_back(0.0f);
	sprite.TexturesCoordinates.push_back(0.0f);
	sprite.TexturesCoordinates.push_back(0.0f);

	sprite.TexturesCoordinates.push_back(1.0f);
	sprite.TexturesCoordinates.push_back(0.0f);
	sprite.TexturesCoordinates.push_back(0.0f);

	sprite.TexturesCoordinates.push_back(1.0f);
	sprite.TexturesCoordinates.push_back(1.0f);
	sprite.TexturesCoordinates.push_back(0.0f);

	sprite.TexturesCoordinates.push_back(0.0f);
	sprite.TexturesCoordinates.push_back(1.0f);
	sprite.TexturesCoordinates.push_back(0.0f);



	Texture tree;
	tree.Bind()->LoadPNG("../res/tree.png")->LoadData()->SetParameters();

	GraphicObject triangle;
	triangle.GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(tree, "diffuseMap");

	triangle.position = glm::vec3(0, 0.0f, 0);
	triangle.rotation = glm::vec3(0, 0, 5.0f);
	triangle.scale = glm::vec3(0.05f, 0.05f, 0.05f);

	triangle.Indices.push_back(0);
	triangle.Indices.push_back(1);
	triangle.Indices.push_back(2);


	float width = 0.25f;
	float height = 0.25f;

	triangle.Positions.push_back(-width / 1.0f);
	triangle.Positions.push_back(-height / 1.0f - 0.5);
	triangle.Positions.push_back(0.0f);
	triangle.Positions.push_back(width / 1.0f);
	triangle.Positions.push_back(-height / 1.0f - 0.5);
	triangle.Positions.push_back(0.0f);
	triangle.Positions.push_back(0.0f);
	triangle.Positions.push_back(height / 1.0f - 0.5);
	triangle.Positions.push_back(0.0f);

	triangle.TexturesCoordinates.push_back(0.0f);
	triangle.TexturesCoordinates.push_back(0.0f);
	triangle.TexturesCoordinates.push_back(0.0f);
	triangle.TexturesCoordinates.push_back(1.0f);
	triangle.TexturesCoordinates.push_back(0.0f);
	triangle.TexturesCoordinates.push_back(0.0f);
	triangle.TexturesCoordinates.push_back(0.5f);
	triangle.TexturesCoordinates.push_back(1.0f);
	triangle.TexturesCoordinates.push_back(0.0f);

	objl::Loader loader;
	if (!loader.LoadFile("../res/box_stack.obj")) {
		std::cout << "Not loaded obj file" << std::endl;
	}
	else {
		triangle.Indices.clear();
		triangle.Indices.insert(std::end(triangle.Indices), std::begin(loader.LoadedIndices), std::end(loader.LoadedIndices));
		
		triangle.Positions.clear();
		triangle.TexturesCoordinates.clear();

		for (int i = 0; i < loader.LoadedVertices.size(); i++) {
			triangle.Positions.push_back(loader.LoadedVertices[i].Position.X);
			triangle.Positions.push_back(loader.LoadedVertices[i].Position.Y);
			triangle.Positions.push_back(loader.LoadedVertices[i].Position.Z);

			triangle.TexturesCoordinates.push_back(loader.LoadedVertices[i].TextureCoordinate.X);
			triangle.TexturesCoordinates.push_back(loader.LoadedVertices[i].TextureCoordinate.Y);
			triangle.TexturesCoordinates.push_back(0.0f);
		}
	}

	objectsToRender.push_back(sprite);
	objectsToRender.push_back(triangle);



	Render2D render;
	auto t_end = std::chrono::high_resolution_clock::now();
	auto t_start = std::chrono::high_resolution_clock::now();
	double delta_time = 0;
	while (Running) {
		for (std::pair<std::string, bool> loadFile : SelectedResources) {
			if (loadFile.second) {
				std::cout << "Loading file: " << loadFile.first << std::endl;
				int startpoint = loadFile.first.find_last_of(".", loadFile.first.size());
				std::string ext = loadFile.first.substr(startpoint, loadFile.first.size());
				if (ext == ".png") {
					Loadedtextures[loadFile.first].Bind()->LoadPNG(loadFile.first.c_str())->LoadData()->SetParameters();
					defaultTexture = &Loadedtextures[loadFile.first];
				}
				else if (ext == ".obj") {
					if (!LoadedObjs[loadFile.first].LoadFile(loadFile.first)) {
						std::cout << "Not loaded obj file" << std::endl;
					}
					else {
						defaultObj = &LoadedObjs[loadFile.first];
					}
				}
				else if (ext == ".glsl") {
					
				}
				else {
					std::cout << "File extension not supported by the system." << std::endl;
				}
			}
		}

		render.SetClearColor({ color.x, color.y, color.z, color.w });
		t_start = std::chrono::high_resolution_clock::now();
		/* Render here */
		int display_w, display_h;
		glfwGetFramebufferSize((GLFWwindow*)window->Context(), &display_w, &display_h);
		render.SetWindowSize({ display_w, display_h });
		render.PrepareScene();
		m_Layers.Update();

		glm::mat4 test(1.0f);
		test = glm::rotate(test, 45.0f, glm::vec3(0, 0, 1.0f));
		//test = glm::scale(test, glm::vec3(0.5f, 0.5f, 1));
		test = glm::translate(test, position);
		objectsToRender[0].SetTransformation(test);

		/*if (Input::IsKeyPressed(GLFW_KEY_D)) {
			objectsToRender.clear();
			glm::mat4 moveMatrix = sprite.GetTransformation();
			moveMatrix = glm::translate(moveMatrix, glm::vec3(1.0f * delta_time, 0.0f, 0.0f));
			sprite.SetTransformation(moveMatrix);
			objectsToRender.push_back(sprite);
		}

		if (Input::IsKeyPressed(GLFW_KEY_A)) {
			objectsToRender.clear();
			glm::mat4 moveMatrix = sprite.GetTransformation();
			moveMatrix = glm::translate(moveMatrix, glm::vec3(-1.0f * delta_time, 0.0f, 0.0f));
			sprite.SetTransformation(moveMatrix);
			objectsToRender.push_back(sprite);
		}

		if (Input::IsKeyPressed(GLFW_KEY_S)) {
			objectsToRender.clear();
			glm::mat4 moveMatrix = sprite.GetTransformation();
			moveMatrix = glm::translate(moveMatrix, glm::vec3(0.0f, -1.0f * delta_time, 0.0f));
			sprite.SetTransformation(moveMatrix);
			objectsToRender.push_back(sprite);
		}

		if (Input::IsKeyPressed(GLFW_KEY_W)) {
			objectsToRender.clear();
			glm::mat4 moveMatrix = sprite.GetTransformation();
			moveMatrix = glm::translate(moveMatrix, glm::vec3(0.0f, 1.0f * delta_time, 0.0f));
			sprite.SetTransformation(moveMatrix);
			objectsToRender.push_back(sprite);
		}*/

		for (GraphicObject grpObj : objectsToRender) {
			//render.draw(&grpObj, shader);
			render.PackObject(grpObj);
		}
		for (GraphicObject grpObj : GraphicObjects) {
			//render.draw(&grpObj, shader);
			render.PackObject(grpObj);
		}


		render.flush(&sprite, shader);

		RenderUI();
		window->Update();
		window->SetFullScreen(FullScreen);
		window->SetVsync(Vsync);
		Close();

		t_end = std::chrono::high_resolution_clock::now();
		delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
	}
}

void MainMenuBar() {
	ImGui::BeginMainMenuBar();

	float frameRate = ImGui::GetIO().Framerate;
	ImGui::Text(("FPS: " + std::to_string(frameRate)).c_str());
	if (ImGui::Button("2D demonstration")) {

	}

	if (ImGui::Button("3D demonstration")) {

	}

	ImGui::Checkbox("Show interface demonstartion", &showDemo);
	if (showDemo) {
		ImGui::ShowDemoWindow(&showDemo);
	}
	;
	ImGui::Checkbox("Vsync", &Vsync);
	ImGui::Checkbox("Full screen", &FullScreen);

	ImGui::Checkbox("Show camera ctrls", &showCameraControlls);
	ImGui::Checkbox("Show entyties ctrls", &showEntytiesControlls);
	ImGui::Checkbox("Show resources ctrls", &showResourcesControlls);

	ImGui::EndMainMenuBar();
}

void CameraControll() {
	ImGui::Begin("Camera variables");
	ImGui::SliderFloat3("Camera position", &Camera::Position[0], -10, 10, "%.3f", 1.0f);
	ImGui::SliderFloat2("Camera rotation", &Camera::Rotation[0], -0.2f, 0.2f, "%.3f", 1.0f);
	if (ImGui::Button("Stop rotation")) {
		Camera::Rotation.x = 0;
		Camera::Rotation.y = 0;
	}
	ImGui::SliderFloat3("Camera view direction", &Camera::ViewDirection[0], -100, 100, "%.3f", 1);
	ImGui::SliderFloat("Camera field of view", &Camera::fov, 0, 360, "%.3f", 1);
	ImGui::Checkbox("Camera is perspective", &Camera::perspective);
	ImGui::ColorEdit4("Clear color", (float*)&color, ImGuiColorEditFlags_Float);
	ImGui::End();
}

void ObjSelection() {
	ImGui::Begin("Obj loaded files");
	for (std::pair<std::string, objl::Loader> obj : LoadedObjs) {
		ImGui::Button(obj.first.c_str());
	}
	ImGui::End();
}
void PngSelection() {
	ImGui::Begin("Png loaded files");
	for (std::pair<std::string, Texture> png : Loadedtextures) {
		ImGui::Button(png.first.c_str());
	}
	ImGui::End();
}
void CreateEditGraphicObject() {
	ImGui::Begin("Graphic objects");
	if(ImGui::Button("Create graphic object")){
		if (defaultObj != nullptr && defaultTexture != nullptr) {
			GraphicObject obj;
			obj.Indices.insert(std::end(obj.Indices), std::begin(defaultObj->LoadedIndices), std::end(defaultObj->LoadedIndices));

			obj.Positions.clear();
			obj.TexturesCoordinates.clear();

			for (int i = 0; i < defaultObj->LoadedVertices.size(); i++) {
				obj.Positions.push_back(defaultObj->LoadedVertices[i].Position.X);
				obj.Positions.push_back(defaultObj->LoadedVertices[i].Position.Y);
				obj.Positions.push_back(defaultObj->LoadedVertices[i].Position.Z);

				obj.TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.X);
				obj.TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.Y);
				obj.TexturesCoordinates.push_back(0.0f);
			}

			obj.GetMaterial()->AddTexture(*defaultTexture, "diffuseMap");
			GraphicObjects.push_back(obj);
		}
	}
	int i = 0;
	for (std::vector<GraphicObject>::iterator object = GraphicObjects.begin(); object != GraphicObjects.end(); ++object) {
		if (ImGui::Button(std::to_string(i).c_str())) {
			SelectedObject = object._Ptr;//Pointing to element in vector.
		}
		i++;
	}
	ImGui::End();

	if (SelectedObject != nullptr) {
		ImGui::Begin("Graphic object editor");
			ImGui::SliderFloat3("Position", &SelectedObject->position[0], -1, 1, "%.3f", 1);
			ImGui::SliderFloat3("rotation", &SelectedObject->rotation[0], -1, 1, "%.3f", 1);
			ImGui::SliderFloat3("scale", &SelectedObject->scale[0], 0.01f, 1, "%.3f", 1);
			ImGui::Button("Select obj/mesh");
			ImGui::Button("Select texture");
			ImGui::ColorEdit4("Material color", &SelectedObject->GetMaterial()->color[0], ImGuiColorEditFlags_Float);
			ImGui::SliderFloat2("Material specularity", &SelectedObject->GetMaterial()->specularity[0], -1, 1, "%.3f", 1);
		ImGui::End();
	}
}
void GraphicObjectsControll() {

}
void ResoursesControl() {
	ImGui::Begin("Resources selector");
	ImGui::InputText("Files", &str[0], IM_ARRAYSIZE(str));
	if (ImGui::Button("Scan")) {
		paths.clear();
		FilesScaner::Scan(str, &paths);
	}
	for (std::string path : paths) {
		SelectedResources[path] = ImGui::Button(path.c_str());
	}
	ImGui::End();


	ObjSelection();
	PngSelection();
}

void EntytiesControll() {
	ImGui::Begin("Valdimas");
		ImGui::SliderFloat3("Pozicija", &position[0], -1, 1, "%.3f", 1);
	ImGui::End();
}

void RenderUI() {
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	MainMenuBar();

	if (showCameraControlls) {
		CameraControll();
	}
	if (showEntytiesControlls) {
		EntytiesControll();
	}
	if (showResourcesControlls) {
		ResoursesControl();
	}

	CreateEditGraphicObject();

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Dimension::Aplication::~Aplication() {}

void Dimension::Aplication::Close() {
	if (events.Dispacth<WindowCloseEvent>([](WindowCloseEvent* e) {return true;})) {
		std::cout << "Close" << std::endl;
		Running = false;
	}
	/*events.Dispacth<Error>([](Error* e) {
		//std::string er = e.GetErrorReport();
		std::cout << e->GetErrorReport() << std::endl;
		return true;
	});

	events.Dispacth<WindowResizeEvent>([](WindowResizeEvent* e) {
		//std::string er = e.GetErrorReport();
		//std::cout << "Window width: " << e->GetWidth() << std::endl;
		return true;
	});

	events.Dispacth<KeyPressedEvent>([](KeyPressedEvent* e) {
		//std::string er = e.GetErrorReport();
		//std::cout << "key: " << e->GetKey() << std::endl;
		return true;
	});*/
}
