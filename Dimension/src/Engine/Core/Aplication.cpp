#include "Aplication.h"

/*Libraries includes*/
#include "imgui.h"
#include "GUI/imgui_impl_glfw.h"
#include "GUI/imgui_impl_opengl3.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"

/*Dimension system includes*/
#include "Layers/Layer.h"

#include "Input/Events/Error.h"
#include "Input/Input.h"

#include "../../Utils/FilesScaner.h"
#include "../../Utils/LoadObj.h"
#include "../../Utils/Timer.h"

#include "Render/Render2D.h"
#include "Render/RenderData/Texture.h"
#include "Render/RenderData/GraphicObject.h"
#include "Render/Shader.h"
#include "Render/Camera.h"

#include "FileReader.h"
#include "ResourceManager/Resources.h"
#include "TaskManager/TaskManager.h"

/*C++ native functions*/
#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <thread>
#include <mutex>

Dimension::Aplication*	Dimension::Aplication::app;

static ImVec4 color = ImVec4(0, 0, 0, 1);
static bool Vsync = true;
static bool FullScreen = false;
static bool showDemo = false;
static bool showCameraControlls = false;
static bool showEntytiesControlls = false;
static bool showResourcesControlls = false;
static bool ShowRenderInformation = false;
static bool systemMonitor = false;
static bool graphicObjectsList = false;
static std::vector<std::string> paths;
static std::map<std::string, bool> SelectedResources;
static std::map<std::string, Texture> Loadedtextures;
static std::map<std::string, objl::Loader> LoadedObjs;
static char str[100] = { "../res/" };
static std::vector<GraphicObject*> GraphicObjects;
static std::map<Render2D*, std::vector<GraphicObject*>> Batchedrenders;
static Render2D* render;
static int LastGraphicsObjectsSize = 0;
static GraphicObject * SelectedObject = nullptr;
static Texture * defaultTexture = nullptr;
static objl::Loader * defaultObj = nullptr;
static int w_heigh = 0;
static int w_width = 0;
static Dimension::Shader* shader;
static Timer timer;
static float CameraMovementSpeed = 1.0f;
static bool changed = true;
static bool cameraControll = true;
static bool CinimaEffect = false;
static float CinimaEffectRadius = 10.0f;
static float CameraProgress = 0;
static int BatchSize = 200;
static std::vector<float> CyclesTimerHistory;
static std::vector<float> RenderTimerHistory;
static std::string Vendor;
static std::string Renderer;
static std::string Version;
static std::vector<std::string> Extensions;
static std::vector<float> lines;

static Resources resources;
static TaskManager taskManager;

/*Debug objects*/
static Timer Loadresource;
static Timer CameracinematicsAndControll;
static Timer PrepareRenderFunction;
static Timer RenderDrawFunction;
static Timer UserInterfaceRender;
static Timer WhileLoopEnd;


static std::mutex test;
void LoadTexture(std::string path, std::map<std::string, void*>* data) {
	//std::lock_guard<std::mutex> lock(test);
	std::string* tekstas = new std::string();
	for (int i = 0; i < 10; i++) {
		//std::cout << "tried to load image " << path << std::endl;
		tekstas->append("tried to load image ");
		tekstas->append(path);
	}
	if (tekstas != nullptr) {
		std::cout << "\ntext size: " << tekstas->size() << std::endl;
	}
	data->insert(std::pair<std::string, void*>(path, (void*)tekstas));
	int g = 0;
}

void AsyncCreateObject(std::string path, std::map<std::string, void*>* data) {
//	data->insert(std::pair<std::string, void*>(path, Dimension::Aplication::CreateObject(*Dimension::Aplication::defaultTexture, *Dimension::Aplication::defaultObj)));
}

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	/*Application init*/
	Dimension::Aplication::app = this;

	/*GLFW inti*/
	window = Window::Create(title, width, height);
	events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
	window->EventsHandler(&events);

	/*OpenGL init*/
	if (gladLoadGL() == 0) {
		//DERROR("Failed to load glad");
		std::cout << "Failed to load glad" << std::endl;
	}
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	FileReader reader;
	shader = new Shader();
	shader->addVertexCode("/res/Vertex.glsl", reader.ReadFile("../res/Vertex.glsl"));
	shader->addFragmentCode("/res/Fragment.glsl", reader.ReadFile("../res/Fragment.glsl"));
	shader->compile();

	//render = new Render2D();
	//render2 = new Render2D();
	render = new Render2D();
	Batchedrenders[render];
	/*Graphic user interface init*/
	IMGUI_CHECKVERSION();	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->Context(), true);	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();	// Setup Dear ImGui style

	resources.AttachFunctionToResourceType(Resources::Type::Text, &LoadTexture);
	resources.AttachFunctionToResourceType(Resources::Type::UserCustom1, &AsyncCreateObject);

	Vendor = window->GetVendor();
	Renderer = window->GetRenderer();
	Version = window->GetVersion();

	for (int i = 0; i < window->GetExtensionNum(); i++) {
		Extensions.push_back((std::string(std::to_string(i + 1) + ". Extension ") + window->GetExtension(i)).c_str());
	}
}
bool tes = true;
void Dimension::Aplication::Run() {
	//Layer * layer = new Layer("Test");
	//m_Layers.PushLayer(layer);

	LoadDefaultResources();

	while (Running) {
		timer.Start();
		Loadresource.Start();
		LoadRequestedResources();
		Loadresource.Stop();
		CameracinematicsAndControll.Start();
		//m_Layers.Update();

		if (cameraControll) {
			if (Dimension::Input::IsKeyPressed(GLFW_KEY_D)) {
				Camera::CameraMove({ 1, 0, 0 }, CameraMovementSpeed * timer.DeltaTime());
			}

			if (Dimension::Input::IsKeyPressed(GLFW_KEY_A)) {
				Camera::CameraMove({ -1, 0, 0 }, CameraMovementSpeed * timer.DeltaTime());
			}

			if (Dimension::Input::IsKeyPressed(GLFW_KEY_S)) {
				Camera::CameraMove({ 0, 0, -1 }, CameraMovementSpeed * timer.DeltaTime());
			}

			if (Dimension::Input::IsKeyPressed(GLFW_KEY_W)) {
				Camera::CameraMove({ 0, 0, 1 }, CameraMovementSpeed * timer.DeltaTime());
			}

			if (Dimension::Input::IsKeyPressed(GLFW_KEY_SPACE)) {
				Camera::CameraMove({ 0, 1, 0 }, CameraMovementSpeed * timer.DeltaTime());
			}

			if (Dimension::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				Camera::CameraMove({ 0, -1, 0 }, CameraMovementSpeed * timer.DeltaTime());
			}
		}

		if (CinimaEffect) {
			CameraProgress += 1.0f * timer.DeltaTime();
			glm::vec3 Pos = { CinimaEffectRadius * glm::cos(CameraProgress) + (CinimaEffectRadius / 2.0f), 0, CinimaEffectRadius * glm::sin(CameraProgress) + (CinimaEffectRadius / 2.0f) };
			Camera::SetPosition(Pos);
			glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);// -Camera::GetPosition();
			Camera::SetViewDirection({ -Pos.x, 0, Pos.z });
		}
		CameracinematicsAndControll.Stop();
		/*Drawing elements in window*/
		PrepareRenderFunction.Start();
		PrepereRender();
		PrepareRenderFunction.Stop();
		RenderDrawFunction.Start();
		DrawObjects();
		RenderDrawFunction.Stop();
		UserInterfaceRender.Start();
		RenderUI();
		UserInterfaceRender.Stop();

		/*Update OS window*/
		WhileLoopEnd.Start();
		window->Update();
		window->SetFullScreen(FullScreen);
		window->SetVsync(Vsync);
		Close();
		WhileLoopEnd.Stop();
		timer.Stop();
	}
}

/*Functions to interact with system*/
void Dimension::Aplication::LoadDefaultResources() {
	defaultObj = new objl::Loader();
	if (!defaultObj->LoadFile("C:/Dev/Dimension/res/box_stack.obj")) {
		std::cout << "Not loaded obj file" << std::endl;
	}
	else {
		std::cout << "Loaded obj file succesfully" << std::endl;
		std::cout << "Loaded png file" << std::endl;
		defaultTexture = new Texture();
		defaultTexture->Bind()->LoadPNG("C:/Dev/Dimension/res/tree.png")->LoadData()->SetParameters();
	}

	SelectedObject = CreateObject(*defaultTexture, *defaultObj);
	AddObjectToRender(SelectedObject);
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
GraphicObject* Dimension::Aplication::CreateObject(Texture texture, objl::Loader mesh) {
	GraphicObject* graphicObject = new GraphicObject();
	graphicObject->GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(*defaultTexture, "diffuseMap");

	graphicObject->rotation = glm::vec3(0, 0, 0.0f);
	graphicObject->position = glm::vec3(0.0f, 0.0f, -10.0f);
	graphicObject->scale = glm::vec3(1.0f, 1.0f, 1.0f);

	//graphicObject->Indices.insert(std::end(graphicObject->Indices), std::begin(defaultObj->LoadedIndices), std::end(defaultObj->LoadedIndices));
	graphicObject->Indices = mesh.LoadedIndices;
	for (int i = 0; i < mesh.LoadedVertices.size(); i++) {
		graphicObject->Positions.push_back(mesh.LoadedVertices[i].Position.X);
		graphicObject->Positions.push_back(mesh.LoadedVertices[i].Position.Y);
		graphicObject->Positions.push_back(mesh.LoadedVertices[i].Position.Z);

		graphicObject->TexturesCoordinates.push_back(mesh.LoadedVertices[i].TextureCoordinate.X);
		graphicObject->TexturesCoordinates.push_back(mesh.LoadedVertices[i].TextureCoordinate.Y);
		graphicObject->TexturesCoordinates.push_back(0.0f);

		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.r);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.b);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.g);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.a);
	}

	return graphicObject;
}
void Dimension::Aplication::AddObjectToRender(GraphicObject* obj) {
	if ((GraphicObjects.size() / BatchSize) >= Batchedrenders.size()) {
		Render2D* render = new Render2D();
		Batchedrenders[render];
	}
	if (Batchedrenders.size() == 0) {
		Render2D* render = new Render2D();
		Batchedrenders[render];
	}

	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		if (_Batch->second.size() < BatchSize) {
			_Batch->second.push_back(obj);

			_Batch->first->StartBatching();
			_Batch->first->ClearBatch();
			for (int i = 0; i < _Batch->second.size(); i++) {
				if (!_Batch->first->IsObjectFit(_Batch->second[i])) {
					Render2D* render = new Render2D();
					Batchedrenders[render];
					break;
				}
				_Batch->first->PackObject(_Batch->second[i], *shader);
				if (_Batch->first->IsBatchFull()) {
					break;
				}
			}
			_Batch->first->Updatebatch();
			_Batch->first->Stopbatching();
			break;
		}
	}

	GraphicObjects.push_back(obj);
}
void Dimension::Aplication::RandomizeObject(GraphicObject * obj) {
	SelectedObject->position.x = glm::linearRand(-100.0f, 100.0f);
	SelectedObject->position.y = glm::linearRand(-10.0f, 10.0f);
	SelectedObject->position.z = glm::linearRand(-100.0f, 100.0f);

	SelectedObject->GetMaterial()->color.x = glm::linearRand(-1.0f, 1.0f);
	SelectedObject->GetMaterial()->color.y = glm::linearRand(-1.0f, 1.0f);
	SelectedObject->GetMaterial()->color.z = glm::linearRand(-1.0f, 1.0f);

	SelectedObject->rotation.x = glm::linearRand(-180.0f, 180.0f);
	SelectedObject->rotation.y = glm::linearRand(-180.0f, 180.0f);
	SelectedObject->rotation.z = glm::linearRand(-180.0f, 180.0f);
	changed = true;
}
void Dimension::Aplication::ReleaseGraphicObjects() {
	for (GraphicObject *obj : GraphicObjects) {
		delete obj;
	}
	GraphicObjects.clear();
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		_Batch->first->ClearBatch();
		_Batch->first->Updatebatch();
		_Batch->second.clear();

		//delete _Batch->first;
	}
	Batchedrenders.clear();

}

/*Graphic user interface with system controll*/
void Dimension::Aplication::RenderUI() {
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	MainMenuBar();

	if (showCameraControlls) {
		CameraControll();
	}
	if (showResourcesControlls) {
		ResoursesControl();
	}
	if (ShowRenderInformation) {
		renderInformation();
	}
	if (systemMonitor) {
		SystemMonitor();
	}
	if (graphicObjectsList) {
		CreateEditGraphicObject();
	}

	SystemHarwareInfo();

	ImGui::Begin("Debug timers");
	ImGui::Text(("Loading resource function: " + std::to_string(Loadresource.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((Loadresource.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	ImGui::Text(("Camera controls, cinema: " + std::to_string(CameracinematicsAndControll.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((CameracinematicsAndControll.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	ImGui::Text(("Render prepare function: " + std::to_string(PrepareRenderFunction.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((PrepareRenderFunction.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	ImGui::Text(("render draw function: " + std::to_string(RenderDrawFunction.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((RenderDrawFunction.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	ImGui::Text(("User interface rendering: " + std::to_string(UserInterfaceRender.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((UserInterfaceRender.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	ImGui::Text(("Ending of while loop: " + std::to_string(WhileLoopEnd.AverageDeltaTime(100) * 1000) + "ms %% " + std::to_string((WhileLoopEnd.AverageDeltaTime(100) / timer.AverageDeltaTime(100)) * 100.0f)).c_str());
	
	ImGui::End();
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Dimension::Aplication::MainMenuBar() {
	ImGui::BeginMainMenuBar();

	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("##1", "System")) {

		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Vsync", &Vsync);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Full screen", &FullScreen);
		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Debug setup", ImVec2(100, 0.0f))) {
			Vsync = false;
			FullScreen = false;
			showDemo = false;
			showCameraControlls = false;
			showEntytiesControlls = true;
			showResourcesControlls = true;
			ShowRenderInformation = true;
			systemMonitor = true;
			graphicObjectsList = true;

			Camera::Position.x = 0.0f;
			Camera::Position.y = 0.0f;
			Camera::Position.z = 100.0f;

			Camera::height = w_heigh;
			Camera::width = w_width;

			Camera::Rotation.x = 0;
			Camera::Rotation.y = 0;

			CameraMovementSpeed = 10.0f;

			Camera::perspective = true;
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("Stress test", ImVec2(100, 0.0f))) {
			/*TaskManager::CreateAsyncTask([](Texture, objl::Loader) {
				
			}, *defaultTexture, *defaultObj);*/
			//std::async(std::launch::async, [](Texture, objl::Loader) {}, *defaultTexture, *defaultObj);

			Vsync = false;
			if (defaultObj != nullptr && defaultTexture != nullptr) {
				for (int i = 0; i < 3500; i++) {
					if (i % 100 == 0) {
						std::cout << "Preparing stress test: " << (((float)i / 3500) * 100.0f) << "%" << std::endl;
					}
					SelectedObject = CreateObject(*defaultTexture, *defaultObj);
					RandomizeObject(SelectedObject);
					AddObjectToRender(SelectedObject);
				}
				std::cout << "Stress prepared: " << (100.0f) << "%" << std::endl;
			}
		}
		if (ImGui::Button("Reset", ImVec2(100, 0.0f))) {
			Vsync = true;
			ReleaseGraphicObjects();
		}
		if (ImGui::Button("Exit", ImVec2(100, 0.0f))) {
			Running = false;
		}


		ImGui::EndCombo();
	}

	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("##2", "Settings")) {

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("2D demonstration")) {
			Camera::Position.x = 0.0f;
			Camera::Position.y = 0.0f;
			Camera::Position.z = 0.0f;

			Camera::height = (float)w_heigh / 1000.0f;
			Camera::width = (float)w_width / 1000.0f;

			Camera::Rotation.x = 0;
			Camera::Rotation.y = 0;

			CameraMovementSpeed = 0.1f;

			Camera::perspective = false;
		}

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("3D demonstration")) {
			Camera::Position.x = 0.0f;
			Camera::Position.y = 0.0f;
			Camera::Position.z = 0.0f;

			Camera::height = w_heigh;
			Camera::width = w_width;

			Camera::Rotation.x = 0;
			Camera::Rotation.y = 0;

			CameraMovementSpeed = 1.0f;

			Camera::perspective = true;
		}

		ImGui::SetNextItemWidth(100);
		if (ImGui::Checkbox("Cinima effect", &CinimaEffect)) {
			Camera::SetPosition({ 0, 0, 0 });
		}

		ImGui::EndCombo();
	}

	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("##3", "Windows", ImGuiComboFlags_::ImGuiComboFlags_None)) {
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show interface demonstartion", &showDemo);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show camera ctrls", &showCameraControlls);
		//ImGui::SetNextItemWidth(100);
		//ImGui::Checkbox("Show entyties ctrls", &showEntytiesControlls);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show resources ctrls", &showResourcesControlls);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show render information", &ShowRenderInformation);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show system information", &systemMonitor);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show graphic objects list", &graphicObjectsList);

		ImGui::EndCombo();
	}
	if (showDemo) {
		ImGui::ShowDemoWindow(&showDemo);
	}

	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("##4", "Input", ImGuiComboFlags_::ImGuiComboFlags_None)) {
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Camera controlls", &cameraControll);

		ImGui::EndCombo();
	}

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
	ImGui::SliderFloat("Camera movement speed", &CameraMovementSpeed, 0.01f, 10, "%.3f", 1);
	ImGui::ColorEdit4("Clear color", (float*)&color, ImGuiColorEditFlags_Float);
	ImGui::End();
}
void Dimension::Aplication::CreateEditGraphicObject() {
	ImGui::Begin("Graphic objects");
	if(ImGui::Button("Create graphic object")) {
		if (defaultObj != nullptr && defaultTexture != nullptr) {
			SelectedObject = CreateObject(*defaultTexture, *defaultObj);
			RandomizeObject(SelectedObject);
			AddObjectToRender(SelectedObject);
		}
	}
	if (ImGui::Button("Create graphic object 10x")) {
		if (defaultObj != nullptr && defaultTexture != nullptr) {
			for (int i = 0; i < 10; i++) {
				SelectedObject = CreateObject(*defaultTexture, *defaultObj);
				RandomizeObject(SelectedObject);
				AddObjectToRender(SelectedObject);
			}
		}
	}
	if (ImGui::Button("Create graphic object 100x")) {
		if (defaultObj != nullptr && defaultTexture != nullptr) {
			for (int i = 0; i < 100; i++) {
				SelectedObject = CreateObject(*defaultTexture, *defaultObj);
				RandomizeObject(SelectedObject);
				AddObjectToRender(SelectedObject);
			}
		}
	}
	if (ImGui::Button("Clear all objects")) {
		ReleaseGraphicObjects();
	}

	int i = 0;
	for (std::vector<GraphicObject*>::iterator object = GraphicObjects.begin(); object != GraphicObjects.end(); ++object) {
		if (ImGui::Button(std::to_string(i).c_str())) {
			SelectedObject = *object;//Pointing to element in vector.
		}
		i++;
	}
	ImGui::End();

	if (SelectedObject != nullptr) {
		ImGui::Begin("Graphic object editor");
			changed |= ImGui::SliderFloat3("Position", &SelectedObject->position[0], -10, 10, "%.3f", 1);
			changed |= ImGui::SliderFloat3("rotation", &SelectedObject->rotation[0], -1, 1, "%.3f", 1);
			changed |= ImGui::SliderFloat3("scale", &SelectedObject->scale[0], 0.01f, 1, "%.3f", 1);
			if (ImGui::Button("Randomize")) {
				RandomizeObject(SelectedObject);
			}
			//ImGui::Button("Select obj/mesh");
			//ImGui::Button("Select texture");
			changed |= ImGui::ColorEdit4("Material color", &SelectedObject->GetMaterial()->color[0], ImGuiColorEditFlags_Float);
			changed |= ImGui::SliderFloat2("Material specularity", &SelectedObject->GetMaterial()->specularity[0], -1, 1, "%.3f", 1);
		ImGui::End();
	}
}
//System info window
void Dimension::Aplication::SystemMonitor() {
	ImGui::Begin("System monitor");
	float RenderTime = 0;
	int DrawCalls = 0;
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		RenderTime += _Batch->first->GetRenderTime();
		DrawCalls += _Batch->first->DrawCalls();
	}
	if (CyclesTimerHistory.size() > 100) {
		CyclesTimerHistory.erase(CyclesTimerHistory.begin(), CyclesTimerHistory.begin() + 1);
		RenderTimerHistory.erase(RenderTimerHistory.begin(), RenderTimerHistory.begin() + 1);
	}
	float cycleAverage = timer.AverageDeltaTime(100);

	ImGui::Text(("Average cycle time: " + (std::to_string(cycleAverage * 1000)) + "ms").c_str());
	ImGui::Text(("Application average processing time: " + (std::to_string((cycleAverage - RenderTime) * 1000)) + "ms").c_str());
	ImGui::Text(("Rendering time: " + (std::to_string(RenderTime * 1000)) + "ms").c_str());
	ImGui::Text(("Draw calls: " + (std::to_string(DrawCalls))).c_str());
	ImGui::End();
}
void Dimension::Aplication::SystemHarwareInfo() {
	ImGui::Begin("Hardware information panel");
	ImGui::Text((Vendor).c_str());
	ImGui::Text((Renderer).c_str());
	ImGui::Text((Version).c_str());
	ImGui::Text((std::string("Extensions count ") + std::to_string(Extensions.size())).c_str());
	for (int i = 0; i < Extensions.size(); i++) {
		ImGui::Text((Extensions[i]).c_str());
	}
	ImGui::End();
}
void Dimension::Aplication::renderInformation() {
	ImGui::Begin("Render information");
	ImGui::ColorEdit4("Material color", &color.x, ImGuiColorEditFlags_Float);
	int ObjectCount = 0;
	float RenderTime = 0;
	float BatchTime = 0;
	float StreamDataTime = 0;
	float DrawTime = 0;
	int DrawCalls = 0;
	int ReservedBytesMemory = 0;
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		ObjectCount +=		_Batch->first->GetObjectsCount();
		RenderTime +=		_Batch->first->GetRenderTime();
		BatchTime +=		_Batch->first->GetBatchTime();
		StreamDataTime +=	_Batch->first->GetStreamData();
		DrawTime +=			_Batch->first->GetDrawTime();
		DrawCalls +=		_Batch->first->DrawCalls();
		ReservedBytesMemory += _Batch->first->GetReservedMemoryInMB();
	}
	ImGui::Text(("Objects count: " + std::to_string(ObjectCount)).c_str());
	ImGui::Text(("Frame render time: " + std::to_string(RenderTime * 1000) + "ms").c_str());
	ImGui::Text(("Last batch time: " + std::to_string(BatchTime * 1000) + "ms %% " + std::to_string((BatchTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Batch size: " + std::to_string(BatchSize) + " Objects fits").c_str());
	ImGui::Text(("Batch count: " + std::to_string(Batchedrenders.size())).c_str());
	ImGui::Text(("Reserved data in GPU: " + std::to_string(ReservedBytesMemory) + "MB").c_str());
	ImGui::Text(("Stream data time(to GPU): " + std::to_string(StreamDataTime * 1000) + "ms %% " + std::to_string((StreamDataTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Draw time: " + std::to_string(DrawTime * 1000) + "ms %% " + std::to_string((DrawTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Draw calls: " + std::to_string(DrawCalls)).c_str());
	//ImGui::Text(("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());

	//ImGui::PlotLines("FPS diagram", [](void* data, int index) { data = (void*)&ImGui::GetIO().Framerate; return ImGui::GetIO().Framerate; }, (void*)&ImGui::GetIO().Framerate, 1);

	lines.push_back(ImGui::GetIO().Framerate);
	if (lines.size() > 250) {
		lines.erase(lines.begin(), lines.begin() + 1);
	}
	ImGui::PlotLines("##6", &lines[0], lines.size(), 0, ("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str(), 0, 300, {500, 100});

	ImGui::End();
}
//Files loading
void Dimension::Aplication::ResoursesControl() {
	ImGui::Begin("Resources selector");
	ImGui::InputText("Files", &str[0], IM_ARRAYSIZE(str));
	if (ImGui::Button("Scan")) {
		paths.clear();
		SelectedResources.clear();
		FilesScaner::Scan(str, &paths);
	}
	for (std::string path : paths) {
		SelectedResources[path] = ImGui::Button(path.c_str());
	}
	ImGui::End();


	ObjSelection();
	PngSelection();
}
void Dimension::Aplication::PngSelection() {
	ImGui::Begin("Png loaded files");
	for (std::pair<std::string, Texture> png : Loadedtextures) {
		ImGui::Button(png.first.c_str());
	}
	ImGui::End();
}
void Dimension::Aplication::ObjSelection() {
	ImGui::Begin("Obj loaded files");
	for (std::pair<std::string, objl::Loader> obj : LoadedObjs) {
		ImGui::Button(obj.first.c_str());
	}
	ImGui::End();
}

/*rendering stuff*/
void Dimension::Aplication::PrepereRender() {
	/* Render here */
	render->SetClearColor({ color.x, color.y, color.z, color.w });
	glfwGetFramebufferSize((GLFWwindow*)window->Context(), &w_width, &w_heigh);
	render->SetWindowSize({ w_width, w_heigh });
	render->PrepareScene();

	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		_Batch->first->StartScene();
	}
}
void Dimension::Aplication::DrawObjects() {
	if (changed) {
		for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
			_Batch->first->ResetUniforms();
			for (GraphicObject* obj : _Batch->second) {
				_Batch->first->UpdateUniforms(obj);
			}
		}
		changed = false;
	}
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		_Batch->first->flush(*shader);
	}
}

void Dimension::Aplication::Close() {
	if (events.Dispacth<WindowCloseEvent>([](WindowCloseEvent* e) {return true;})) {
		std::cout << "Close" << std::endl;
		Running = false;
	}
	events.Dispacth<MauseCursorEvent>([](MauseCursorEvent* e) {
		//std::string er = e.GetErrorReport();
		//if (p.GetMouseKey(GLFW_MOUSE_BUTTON_1)) {
		double Mousey = e->GetY();
		double Mousex = e->GetX();
			//Camera::SetRotation(glm::vec2(Mousex - 1000 / 2, Mousey - 800 / 2));
			//p.SetCursorPosition(a / 2, b / 2);
		//}
		//std::cout << e->GetX() << std::endl;
		return true;
	});
	
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
Dimension::Aplication::~Aplication() {
	//delete render;
	delete shader;
	//delete render2;
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		for (GraphicObject* obj : _Batch->second) {
			delete obj;
		}
		_Batch->second.clear();
		delete _Batch->first;
	}
}