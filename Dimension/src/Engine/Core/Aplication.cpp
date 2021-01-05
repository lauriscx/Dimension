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

/*C++ native functions*/
#include <functional>
#include <iostream>
#include <string>
#include <fstream>

Dimension::Aplication*	Dimension::Aplication::app;

/*Variables for interface*/
static ImVec4 color = ImVec4(0, 0, 0, 1);
static bool Vsync = true;
static bool FullScreen = false;
static bool showDemo = false;
static bool showCameraControlls = false;
static bool showEntytiesControlls = false;
static bool showResourcesControlls = false;
static bool systemMonitor = false;
static bool graphicObjectsList = false;
static std::vector<std::string> paths;
static std::map<std::string, bool> SelectedResources;
static std::map<std::string, Texture> Loadedtextures;
static std::map<std::string, objl::Loader> LoadedObjs;
static char str[100] = { "../res/" };
static std::vector<GraphicObject*> GraphicObjects;
static std::map<Render2D*, std::vector<GraphicObject*>> Batchedrenders;
static int LastGraphicsObjectsSize = 0;
static GraphicObject * SelectedObject = nullptr;
static Texture * defaultTexture = nullptr;
static objl::Loader * defaultObj = nullptr;
static int w_heigh = 0;
static int w_width = 0;
static Dimension::Shader* shader;
static std::vector<Render2D> BatchRenders;
static Timer timer;
static float CameraMovementSpeed = 1.0f;
static bool ShowRenderInformation = false;
static bool changed = true;
static bool cameraControll = true;

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
	Render2D* render = new Render2D();
	Batchedrenders[render];
	BatchRenders.push_back(*render);
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

	while (Running) {
		timer.Start();

		LoadRequestedResources();

		m_Layers.Update();

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

		/*Drawing elements in window*/
		PrepereRender();
		DrawObjects();
		RenderUI();

		/*Update OS window*/
		window->Update();
		window->SetFullScreen(FullScreen);
		window->SetVsync(Vsync);
		Close();

		timer.Stop();
	}
}

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
GraphicObject* Dimension::Aplication::CreateObject(Texture texture, objl::Loader mesh) {
	GraphicObject* graphicObject = new GraphicObject();
	graphicObject->GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(*defaultTexture, "diffuseMap");

	graphicObject->rotation = glm::vec3(0, 0, 0.0f);
	graphicObject->position = glm::vec3(0.0f, 0.0f, -10.0f);
	graphicObject->scale = glm::vec3(1.0f, 1.0f, 1.0f);

	graphicObject->Indices.insert(std::end(graphicObject->Indices), std::begin(defaultObj->LoadedIndices), std::end(defaultObj->LoadedIndices));
	for (int i = 0; i < defaultObj->LoadedVertices.size(); i++) {
		graphicObject->Positions.push_back(defaultObj->LoadedVertices[i].Position.X);
		graphicObject->Positions.push_back(defaultObj->LoadedVertices[i].Position.Y);
		graphicObject->Positions.push_back(defaultObj->LoadedVertices[i].Position.Z);

		graphicObject->TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.X);
		graphicObject->TexturesCoordinates.push_back(defaultObj->LoadedVertices[i].TextureCoordinate.Y);
		graphicObject->TexturesCoordinates.push_back(0.0f);

		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.r);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.b);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.g);
		graphicObject->Colors.push_back(graphicObject->GetMaterial()->color.a);
	}

	return graphicObject;
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
		if (ImGui::Button("Exit", ImVec2(100, 0.0f))) {
			Running = false;
		}


		ImGui::EndCombo();
	}

	ImGui::SetNextItemWidth(100);
	if(ImGui::BeginCombo("##2", "Settings")) {

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

		ImGui::EndCombo();
	}

	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("##3", "Windows", ImGuiComboFlags_::ImGuiComboFlags_None)) {
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show interface demonstartion", &showDemo);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show camera ctrls", &showCameraControlls);
		ImGui::SetNextItemWidth(100);
		ImGui::Checkbox("Show entyties ctrls", &showEntytiesControlls);
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
void Dimension::Aplication::AddObjectToRender(GraphicObject* obj) {
	if ((GraphicObjects.size() / 200) > Batchedrenders.size()) {
		Render2D* render = new Render2D();
		Batchedrenders[render];
	}
	if (Batchedrenders.size() == 0) {
		Render2D* render = new Render2D();
		Batchedrenders[render];
	}

	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		if (_Batch->second.size() < 200) {
			_Batch->second.push_back(obj);

			_Batch->first->StartBatching();
			_Batch->first->ClearBatch();
			for (int i = 0; i < _Batch->second.size(); i++) {
				_Batch->first->PackObject(_Batch->second[i], *shader);
			}
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
void Dimension::Aplication::SystemMonitor() {
	ImGui::Begin("System monitor");
	ImGui::Text(("Cycle time: " + std::to_string(timer.DeltaTimeMls())).c_str());
	float RenderTime = 0;
	for (Render2D render : BatchRenders) {
		RenderTime += render.GetRenderTime();
	}
	ImGui::Text(("Application processing time: " + std::to_string(timer.DeltaTimeMls() - (RenderTime * 1000))).c_str());
	ImGui::Text(("Rendering time: " + std::to_string(RenderTime * 1000)).c_str());
	ImGui::Text(("Draw calls: " + std::to_string(BatchRenders.size())).c_str());
	ImGui::End();
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
		GraphicObjects.clear();
		for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
			_Batch->first->ClearBatch();
			for (GraphicObject* obj : _Batch->second) {
				delete obj;
			}
			_Batch->second.clear();
			delete _Batch->first;
		}
		Batchedrenders.clear();
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
	BatchRenders[0].SetClearColor({ color.x, color.y, color.z, color.w });
	glfwGetFramebufferSize((GLFWwindow*)window->Context(), &w_width, &w_heigh);
	BatchRenders[0].SetWindowSize({ w_width, w_heigh });
	BatchRenders[0].PrepareScene();

	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		_Batch->first->StartScene();
	}

	/*for (int i = 0; i < BatchRenders.size(); i++) {
		BatchRenders[i].StartScene();
	}*/
}
void Dimension::Aplication::DrawObjects() {
	if (changed) {
		for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
			_Batch->first->ResetUniforms();
			for (GraphicObject* obj : _Batch->second) {
				_Batch->first->UpdateUniforms(obj);
			}
		}

		/*
		int batchRenderID = 0;
		for (int i = 0; i < BatchRenders.size(); i++) {
			BatchRenders[i].ResetUniforms();
			int LastID = ((batchRenderID + 1) * 200) + 200;
			if (LastID > GraphicObjects.size()) {
				LastID = GraphicObjects.size();
			}
			for (int i = 0 + (batchRenderID * 200); i < LastID; i++) {
				BatchRenders[i].UpdateUniforms(GraphicObjects[i]);
			}
			BatchRenders[i].flush(*shader);
		}*/
		changed = false;
	}
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		_Batch->first->flush(*shader);
	}
	/*for (int i = 0; i < BatchRenders.size(); i++) {
		BatchRenders[i].flush(*shader);
	}*/
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
	if (ShowRenderInformation) {
		renderInformation();
	}
	if (systemMonitor) {
		SystemMonitor();
	}
	if (graphicObjectsList) {
		CreateEditGraphicObject();
	}

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Dimension::Aplication::renderInformation() {
	ImGui::Begin("Render information");
	int ObjectCount = 0;
	float RenderTime = 0;
	float BatchTime = 0;
	float StreamDataTime = 0;
	float DrawTime = 0;
	int DrawCalls = 0;
	for (std::map<Render2D*, std::vector<GraphicObject*>>::iterator _Batch = Batchedrenders.begin(); _Batch != Batchedrenders.end(); ++_Batch) {
		ObjectCount +=		_Batch->first->GetObjectsCount();
		RenderTime +=		_Batch->first->GetRenderTime();
		BatchTime +=		_Batch->first->GetBatchTime();
		StreamDataTime +=	_Batch->first->GetStreamData();
		DrawTime +=			_Batch->first->GetDrawTime();
		DrawCalls +=		_Batch->first->DrawCalls();
	}
	ImGui::Text(("Objects count: " + std::to_string(ObjectCount)).c_str());
	ImGui::Text(("Frame render time: " + std::to_string(RenderTime * 1000)).c_str());
	ImGui::Text(("Last batch time: " + std::to_string(BatchTime * 1000) + " prc " + std::to_string((BatchTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Stream data time(to GPU): " + std::to_string(StreamDataTime * 1000) + " prc " + std::to_string((StreamDataTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Draw time: " + std::to_string(DrawTime * 1000) + " prc " + std::to_string((DrawTime / RenderTime) * 100)).c_str());
	ImGui::Text(("Draw calls: " + std::to_string(DrawCalls)).c_str());
	ImGui::Text(("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());
	ImGui::End();
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