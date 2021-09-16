#include <Windows.h>
#include <WinUser.h>
#include <WinNls32.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>

int dstTime = 0;
struct Color
{
	float r, g, b;
};

Color makeColor(float r, float g, float b)
{
	Color c = { r, g, b };
	return c;
};

Color lerp(Color a, Color b, float t)
{
	Color c;
	c.r = (1 - t) * a.r + t * b.r;
	c.g = (1 - t) * a.g + t * b.g;
	c.b = (1 - t) * a.b + t * b.b;

	return c;
}

int main()
{
	// Initialize GLFW
	glfwInit();
	clock_t begin = clock();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(1200, 700, "Dessor 0.1.0c", NULL, NULL);
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
	glViewport(0, 0, 1200, 700);



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
		static float size = 3.0;
		if (drawCube)
		{
			using namespace std;
			clock_t end = clock();
			double elapsed_secs = double(end - begin);
			const float t = std::cos(float(elapsed_secs) * 0.001) * 0.5 + 0.5;
			Color curColor = lerp(makeColor(size/10, 0.6, 0.7), makeColor(size / 10, .8, 0.7), t);
			glClearColor(curColor.r, curColor.g, curColor.b, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		// ImGUI window creation
		// Particular widget styling
		static int i2 = 3;
		static int ifov = 60;
		static char name[128] = "5";
		static const ImVec4 edge_color = ImVec4(0.25f, 0.25f, 0.90f, 1.00f);
		static const ImVec4 inside_color = ImVec4(0.55f, 0.55f, 0.90f, 1.00f);
		const ImVec2 size2 = ImVec2(250, 200);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Begin("Drawing HyperCube Options", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::PopStyleColor();
		ImGui::Checkbox("Draw Shape", &drawCube);
		ImGui::SliderInt("D", &i2, 0, atoi(name), "%d-dimensional hypercube");
		ImGui::InputText("", name, 7, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 0, 255, 255), name);
		ImGui::NewLine();
		ImGui::SliderFloat("SIZE", &size, 1.0, 10.0);
		ImGui::SliderInt("FOV", &ifov, 30, 120, "%d");
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