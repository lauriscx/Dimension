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
static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

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
	//sprite.GetBatch()->GenRectangle(0.1f, 0.1f);
	glm::mat4 test(1.0f);
	/*test = glm::translate(test, glm::vec3(0, 0.5f, 0));
	test = glm::rotate(test, 45.0f, glm::vec3(0, 0, 1.0f));
	test = glm::scale(test, glm::vec3(2, 2, 1));*/
	sprite.SetTransformation(test);

	objectsToRender.push_back(sprite);


	GraphicObject triangle;
	triangle.GetMaterial()->SetColor({ 1, 0, 0, 1 })->AddTexture(texture, "diffuseMap");
	//triangle.GetBatch()->GenTriangle(0.5f, 0.5f);
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
		render.SetClearColor({ color.x, color.y, color.z, color.w });
		t_start = std::chrono::high_resolution_clock::now();
		/* Render here */
		int display_w, display_h;
		glfwGetFramebufferSize((GLFWwindow*)window->Context(), &display_w, &display_h);
		render.SetWindowSize({ display_w, display_h });
		render.PrepareScene();
		m_Layers.Update();

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
			render.PackObject(&grpObj);
		}

		render.flush(&sprite, shader);

		RenderUI();
		window->Update();
		Close();

		t_end = std::chrono::high_resolution_clock::now();
		delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
	}
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
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
	bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo);




	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	static bool hdr = false;
	ImGui::Checkbox("With Alpha Preview", &alpha_preview);
	ImGui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
	ImGui::Checkbox("With Drag and Drop", &drag_and_drop);
	ImGui::Checkbox("With Options Menu", &options_menu); ImGui::SameLine(); HelpMarker("Right-click on the individual color widget to show options.");
	ImGui::Checkbox("With HDR", &hdr); ImGui::SameLine(); HelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	ImGui::Text("Color widget:");
	ImGui::SameLine(); HelpMarker(
		"Click on the colored square to open a color picker.\n"
		"CTRL+click on individual component to input value.\n");
	ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

	ImGui::Text("Color widget HSV with Alpha:");
	ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_DisplayHSV | misc_flags);

	ImGui::Text("Color widget with Float Display:");
	ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | misc_flags);

	ImGui::Text("Color button with Picker:");
	ImGui::SameLine(); HelpMarker(
		"With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\n"
		"With the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only "
		"be used for the tooltip and picker popup.");
	ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

	ImGui::Text("Color button with Custom Picker Popup:");

	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	static ImVec4 backup_color;
	bool open_popup = ImGui::ColorButton("MyColor##3b", color, misc_flags);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	open_popup |= ImGui::Button("Palette");
	if (open_popup)
	{
		ImGui::OpenPopup("mypicker");
		backup_color = color;
	}
	if (ImGui::BeginPopup("mypicker"))
	{
		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current");
		ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous");
		if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color = backup_color;
		ImGui::Separator();
		ImGui::Text("Palette");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
				color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}


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
