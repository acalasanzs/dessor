#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Windows.h>
#include<WinUser.h>
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

	//Load GLAD so it configures OPENGL
	gladLoadGL();


	HICON hIcon = static_cast<HICON>(::LoadImage(NULL,
		MAKEINTRESOURCE("icon.ico"),
		IMAGE_ICON,
		48, 48,    // or whatever size icon you want to load
		LR_DEFAULTCOLOR);

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