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

#include "glm/gtc/matrix_transform.hpp"

#include "FileReader.h"
#include "Render/Render2D.h"
#include <fstream>

Dimension::Aplication*	Dimension::Aplication::app;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	
	Dimension::Aplication::app = this;
	window = Window::Create(title, width, height);
	events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
	window->EventsHandler(&events);
	if (gladLoadGL() == 0) {
		DERROR("Failed to load glad");
	}
}
void RenderUI();

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
	sprite.GetMesh()->GenRectangle(0.5f, 0.5f);
	glm::mat4 test(1.0f);
	/*test = glm::translate(test, glm::vec3(0, 0.5f, 0));
	test = glm::rotate(test, 45.0f, glm::vec3(0, 0, 1.0f));
	test = glm::scale(test, glm::vec3(2, 2, 1));*/
	sprite.SetTransformation(test);

	objectsToRender.push_back(sprite);


	GraphicObject triangle;
	triangle.GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(texture, "diffuseMap");
	triangle.GetMesh()->GenTriangle(0.5f, 0.5f);
	glm::mat4 test2(1.0f);
	test2 = glm::translate(test2, glm::vec3(0, -0.5f, 0));
	test2 = glm::rotate(test2, -45.0f, glm::vec3(0, 0, 1.0f));
	test2 = glm::scale(test2, glm::vec3(2, 2, 1));
	triangle.SetTransformation(test2);

	//objectsToRender.push_back(triangle);

	Render2D render;
	auto t_end = std::chrono::high_resolution_clock::now();
	auto t_start = std::chrono::high_resolution_clock::now();
	double delta_time = 0;
	while (Running) {
		t_start = std::chrono::high_resolution_clock::now();
		/* Render here */
		int display_w, display_h;
		glfwGetFramebufferSize((GLFWwindow*)window->Context(), &display_w, &display_h);
		render.SetWindowSize({ display_w, display_h });
		render.PrepareScene();
		m_Layers.Update();

		if (Input::IsKeyPressed(GLFW_KEY_D)) {
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
		}

		for (GraphicObject grpObj : objectsToRender) {
			render.draw(&grpObj, shader);
		}

		RenderUI();
		window->Update();
		Close();

		// the work...
		t_end = std::chrono::high_resolution_clock::now();
		delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
	}
}

void RenderUI() {
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// render your GUI
	ImGui::Begin("Demo langas");
	ImGui::Button("Sveiki!");
	ImGui::End();

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Dimension::Aplication::~Aplication() {

}

void Dimension::Aplication::Close() {
	if (events.Dispacth<WindowCloseEvent>([](WindowCloseEvent* e) {return true;})) {
		std::cout << "Close" << std::endl;
		Running = false;
	}
	events.Dispacth<Error>([](Error* e) {
		//std::string er = e.GetErrorReport();
		std::cout << e->GetErrorReport() << std::endl;
		return true;
	});

	events.Dispacth<WindowResizeEvent>([](WindowResizeEvent* e) {
		//std::string er = e.GetErrorReport();
		std::cout << "Window width: " << e->GetWidth() << std::endl;
		return true;
	});

	events.Dispacth<KeyPressedEvent>([](KeyPressedEvent* e) {
		//std::string er = e.GetErrorReport();
		//std::cout << "key: " << e->GetKey() << std::endl;
		return true;
	});
}
