#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Windows.h>
#include<WinUser.h>
#include "SOIL.h"
int main()
{
	// Initialize GLFW
	glfwInit();
	 
	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 560, "Dessor 0.0.1a", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	HICON newSmallIco, newBigIco;
	newSmallIco = reinterpret_cast<HICON>(LoadImage(nullptr, L"C:\\Users\\RLWA32\\Pictures\\Folder.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE));
	newBigIco = reinterpret_cast<HICON>(LoadImage(nullptr, L"C:\\Users\\RLWA32\\Pictures\\PropertyIcon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));

	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image("icon.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window.window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);
	//Load GLAD so it configures OPENGL
	gladLoadGL();

	//OpenGL viewport
	glViewport(0, 0, 1000, 560);

	glClearColor(0.12f, 0.12f, 0.19f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}