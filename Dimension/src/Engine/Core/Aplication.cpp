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

#include "FileReader.h"
#include "Render/Render2D.h"
#include <fstream>

Dimension::Aplication*	Dimension::Aplication::app;

Dimension::Aplication::Aplication(const char* title, int width, int height) : Running(true) {
	
	Dimension::Aplication::app = this;
	window = Window::Create(title, width, height);
	//events.OnEvent(std::bind(&LayerStack::OnEvent, &m_Layers, std::placeholders::_1));
	window->EventsHandler(&events);
	if (gladLoadGL() == 0) {
		DERROR("Failed to load glad");
	}
}

void Dimension::Aplication::Run() {

	Layer * layer = new Layer("Test");
	m_Layers.PushLayer(layer);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->Context(), true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	FileReader reader;
	Shader shader;
	shader.addVertexCode("/res/Vertex.glsl", reader.ReadFile("../res/Vertex.glsl"));
	shader.addFragmentCode("/res/Fragment.glsl", reader.ReadFile("../res/Fragment.glsl"));
	shader.compile();

	Texture texture;
	texture.Bind();
	texture.LoadPNG("../res/texture.png");
	texture.LoadData();
	texture.SetParameters();

	GraphicObject sprite;
	sprite.GetMaterial()->SetColor({ 0, 1, 1, 1 });
	sprite.GetMaterial()->AddTexture(texture, "diffuseMap");
	sprite.GetMesh()->GenRectangle(0.5f, 0.5f);


	Render2D render;

	while (Running) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0.75f, 0, 1);

		int display_w, display_h;
		glfwGetFramebufferSize((GLFWwindow*)window->Context(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		m_Layers.Update();


		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		render.draw(sprite, shader);

		window->Update();
		Close();
		
		if (Input::IsKeyPressed(GLFW_KEY_R)) {
			std::cout << "R is pressed!" << std::endl;
			//m_Layers.PopLayer(layer);
		}

		/*events.Dispacth<KeyTypedEvent>([&text](Event& e) {
			KeyTypedEvent& event = (KeyTypedEvent&)e;
			
			text += event.GetKey();
			return true;
		});*/
		//std::cout << text << std::endl;
	}

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
		std::cout << "key: " << e->GetKey() << std::endl;
		return true;
	});
}
