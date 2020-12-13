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

/*Variables for interface*/
static ImVec4 color = ImVec4(0, 0, 0, 1);
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
static int w_heigh = 0;
static int w_width = 0;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	/*Application init*/
	Dimension::Aplication::app = this;

	/*GLFW inti*/
	window = Window::Create(title, width, height);
	events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
	window->EventsHandler(&events);

	/*OpenGL init*/
	if (gladLoadGL() == 0) {
		DERROR("Failed to load glad");
	}
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	FileReader reader;
	shader.addVertexCode("/res/Vertex.glsl", reader.ReadFile("../res/Vertex.glsl"));
	shader.addFragmentCode("/res/Fragment.glsl", reader.ReadFile("../res/Fragment.glsl"));
	shader.compile();

	/*Graphic user interface init*/
	IMGUI_CHECKVERSION();	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->Context(), true);	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();	// Setup Dear ImGui style
}

void Dimension::Aplication::Run() {
	Layer * layer = new Layer("Test");
	m_Layers.PushLayer(layer);

	LoadDefaultResources();

	auto t_end = std::chrono::high_resolution_clock::now();
	auto t_start = std::chrono::high_resolution_clock::now();
	double delta_time = 0;

	while (Running) {
		t_start = std::chrono::high_resolution_clock::now();

		LoadRequestedResources();
		
		m_Layers.Update();

		/*Drawing elements in window*/
		PrepereRender();
		DrawObjects();
		RenderUI();

		/*Update OS window*/
		window->Update();
		window->SetFullScreen(FullScreen);
		window->SetVsync(Vsync);
		Close();


		t_end = std::chrono::high_resolution_clock::now();
		delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
	}
}

void Dimension::Aplication::LoadDefaultResources() {
	defaultObj = new objl::Loader();
	if (!defaultObj->LoadFile("../res/box_stack.obj")) {
		std::cout << "Not loaded obj file" << std::endl;
	} else {
		std::cout << "Loaded obj file succesfully" << std::endl;
		std::cout << "Loaded png file" << std::endl;
		defaultTexture = new Texture();
		defaultTexture->Bind()->LoadPNG("../res/tree.png")->LoadData()->SetParameters();
	}

	GraphicObjects.push_back(CreateObject(*defaultTexture, *defaultObj));
	SelectedObject = &GraphicObjects[0];
}
GraphicObject Dimension::Aplication::CreateObject(Texture texture, objl::Loader mesh) {
	GraphicObject graphicObject;
	graphicObject.GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(*defaultTexture, "diffuseMap");

	graphicObject.position = glm::vec3(0.0f, 0.0f, -10.0f);
	graphicObject.rotation = glm::vec3(0, 0, 0.0f);
	graphicObject.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	graphicObject.Indices.insert(std::end(graphicObject.Indices), std::begin(defaultObj->LoadedIndices), std::end(defaultObj->LoadedIndices));
	for (int i = 0; i < defaultObj->LoadedVertices.size(); i++) {
		graphicObject.Positions.push_back(defaultObj->LoadedVertices[i].Position.X);
		graphicObject.Positions.push_back(defaultObj->LoadedVertices[i].Position.Y);
		graphicObject.Positions.push_back(defaultObj->LoadedVertices[i].Position.Z);

		graphicObject.TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.X);
		graphicObject.TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.Y);
		graphicObject.TexturesCoordinates.push_back(0.0f);
	}

	return graphicObject;
}
void Dimension::Aplication::MainMenuBar() {
	ImGui::BeginMainMenuBar();

	float frameRate = ImGui::GetIO().Framerate;
	ImGui::Text(("FPS: " + std::to_string(frameRate)).c_str());
	if (ImGui::Button("2D demonstration")) {
		Camera::Position.x = 0.0f;
		Camera::Position.y = 0.0f;
		Camera::Position.z = 0.0f;

		Camera::height = (float)w_heigh / 1000.0f;
		Camera::width = (float)w_width / 1000.0f;

		Camera::Rotation.x = 0;
		Camera::Rotation.y = 0;

		Camera::perspective = false;
	}

	if (ImGui::Button("3D demonstration")) {
		Camera::Position.x = 0.0f;
		Camera::Position.y = 0.0f;
		Camera::Position.z = 0.0f;

		Camera::height = w_heigh;
		Camera::width = w_width;

		Camera::Rotation.x = 0;
		Camera::Rotation.y = 0;

		Camera::perspective = true;
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
void Dimension::Aplication::CameraControll() {
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
	ImGui::SliderFloat("Window width", &Camera::width, 1, 1000, "%.3f", 1);
	ImGui::SliderFloat("Window height", &Camera::height, 1, 1000, "%.3f", 1);
	ImGui::Text(("Asspect ratio: " + std::to_string(Camera::width / Camera::height)).c_str());
	ImGui::SliderFloat("Camera ZNear", &Camera::zNear, 0.01f, 10, "%.3f", 1);
	ImGui::SliderFloat("Window ZFar", &Camera::zFar, 0.01f, 1000, "%.3f", 1);
	ImGui::ColorEdit4("Clear color", (float*)&color, ImGuiColorEditFlags_Float);
	ImGui::End();

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_D)) {
		Camera::CameraMove({ 1, 0, 0 }, 0.1f);
	}

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_A)) {
		Camera::CameraMove({ -1, 0, 0 }, 0.1f);
	}

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_S)) {
		Camera::CameraMove({ 0, 0, -1 }, 0.1f);
	}

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_W)) {
		Camera::CameraMove({ 0, 0, 1 }, 0.1f);
	}

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_SPACE)) {
		Camera::CameraMove({ 0, 1, 0 }, 0.1f);
	}

	if (Dimension::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		Camera::CameraMove({ 0, -1, 0 }, 0.1f);
	}
}
void Dimension::Aplication::ObjSelection() {
	ImGui::Begin("Obj loaded files");
	for (std::pair<std::string, objl::Loader> obj : LoadedObjs) {
		ImGui::Button(obj.first.c_str());
	}
	ImGui::End();
}
void Dimension::Aplication::PngSelection() {
	ImGui::Begin("Png loaded files");
	for (std::pair<std::string, Texture> png : Loadedtextures) {
		ImGui::Button(png.first.c_str());
	}
	ImGui::End();
}
void Dimension::Aplication::CreateEditGraphicObject() {
	ImGui::Begin("Graphic objects");
	if(ImGui::Button("Create graphic object")) {
		if (defaultObj != nullptr && defaultTexture != nullptr) {
			GraphicObjects.push_back(CreateObject(*defaultTexture, *defaultObj));
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
			ImGui::SliderFloat3("Position", &SelectedObject->position[0], -10, 10, "%.3f", 1);
			ImGui::SliderFloat3("rotation", &SelectedObject->rotation[0], -1, 1, "%.3f", 1);
			ImGui::SliderFloat3("scale", &SelectedObject->scale[0], 0.01f, 1, "%.3f", 1);
			ImGui::Button("Select obj/mesh");
			ImGui::Button("Select texture");
			ImGui::ColorEdit4("Material color", &SelectedObject->GetMaterial()->color[0], ImGuiColorEditFlags_Float);
			ImGui::SliderFloat2("Material specularity", &SelectedObject->GetMaterial()->specularity[0], -1, 1, "%.3f", 1);
		ImGui::End();
	}
}
void Dimension::Aplication::GraphicObjectsControll() {

}
void Dimension::Aplication::ResoursesControl() {
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
void Dimension::Aplication::EntytiesControll() {

}
void Dimension::Aplication::PrepereRender() {
	/* Render here */
	render.SetClearColor({ color.x, color.y, color.z, color.w });
	glfwGetFramebufferSize((GLFWwindow*)window->Context(), &w_width, &w_heigh);
	render.SetWindowSize({ w_width, w_heigh });
	render.PrepareScene();
}
void Dimension::Aplication::DrawObjects() {
	for (GraphicObject grpObj : GraphicObjects) {
		render.PackObject(grpObj);
	}
	render.flush(shader);
}
void Dimension::Aplication::LoadRequestedResources() {
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
}
void Dimension::Aplication::RenderUI() {
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
Dimension::Aplication::~Aplication() {}
