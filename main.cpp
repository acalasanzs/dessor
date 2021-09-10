#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <WinUser.h>
#include <WinNls32.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(size * aPos.x, size * aPos.y, size * aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";



int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Dessor 0.1.0c", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Variables to be changed in the ImGUI window
	bool drawCube = false;
	float size = 1.0f;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


	// Attempting to set window icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("icon.ico", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
	

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Clean the back buffer and assign the new color to it

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (drawCube)
			glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

		// ImGUI window creation
		// Particular widget styling
		static int i2 = 3;
		static int ifov = 60;
		static float i3 = 3.0f;
		static char name[128] = "5";
		static const ImVec4 edge_color = ImVec4(0.25f, 0.25f, 0.90f, 1.00f);
		static const ImVec4 inside_color = ImVec4(0.55f, 0.55f, 0.90f, 1.00f);
		const ImVec2 size2 = ImVec2(250, 200);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Begin("Drawing HyperCube Options",NULL,ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::PopStyleColor();
		ImGui::Checkbox("Draw Shape", &drawCube);
		ImGui::SliderInt("D", &i2, 0, atoi(name), "%d-dimensional hypercube");
		ImGui::InputText("", name,7,ImGuiInputTextFlags_CharsDecimal|ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 0, 255, 255), name);
		ImGui::NewLine();
		ImGui::SliderInt("FOV", &ifov, 30, 120, "%d");
		ImGui::SliderFloat("SIZE", &i3, 1.0f, 10.0f);
		ImGui::NewLine();
		ImGui::Button("Render", ImVec2(250, 60));
		ImGui::BeginChild("Inside color", size2);
		ImGui::TextColored(inside_color, "INSIDE COLOR");
		ImGui::ColorPicker3("", (float*)&inside_color);
		ImGui::TextColored(edge_color, "EDGE COLOR");
		ImGui::ColorEdit3("", (float*)&edge_color);
		ImGui::EndChild();
		ImGui::NewLine();
		ImGui::End();
		// Checkbox that appears in the window

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}