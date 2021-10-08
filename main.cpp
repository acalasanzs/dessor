#include <iostream>
#include <string>

#include <Windows.h>
#include <WinUser.h>
#include <WinNls32.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <cmath>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_truetype.h>
#include <stb/stb_rect_pack.h>
#include <fstream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;
unsigned int VAO2, VBO2;

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



// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;




// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Init clock
    clock_t begin = clock();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dessor 0.3.0g Glew", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    
    // Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiStyle* style = &ImGui::GetStyle();
    // Variables to be changed in the ImGUI window
    bool drawCube = false;
    float size = 0.5f;
    float color[4] = { 0.55f, 0.55f, 0.55f, 1.00f };
    GLFWimage images[1];
    images[0].pixels = stbi_load("icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
    bool sound = false;
    bool specs = false;


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        std::string all = std::to_string(width) + "px, " + std::to_string(height) + "px;";
        const char* cstr = all.c_str();
        // Render
        // Clear the colorbuffer
        glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (drawCube)
        {
            using namespace std;
            clock_t end = clock();
            double elapsed_secs = double(end - begin);
            const float t = std::cos(float(elapsed_secs) * 0.001) * 0.5 + 0.5;
            Color curColor = lerp(makeColor(size / 1, 0.6, 0.7), makeColor(size / 1, .8, 0.7), t);
            glClearColor(curColor.r, curColor.g, curColor.b, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        // ImGUI window creation
        // Particular widget styling
        static int i2 = 3;
        static int ifov = 60;
        static char name[128] = "5";
        static const ImVec4 edge_color = ImVec4(0.25f, 0.25f, 0.90f, 1.00f);
        const ImVec2 size2 = ImVec2(250, 200);
        if (!specs) {

            style->WindowPadding = ImVec2(0, 0);
            style->WindowRounding = 5.0f;
            style->FramePadding = ImVec2(0, 0);
            style->FrameRounding = 4.0f;
            style->ItemSpacing = ImVec2(0, 0);
            style->ItemInnerSpacing = ImVec2(0, 0);
            style->GrabMinSize = 5.0f;
            style->GrabRounding = 3.0f;
            // rgb(55, 66, 250)
            style->Colors[ImGuiCol_Text] = ImVec4((255.0f), (255.0f), (255.0f), 1.00f);
            style->Colors[ImGuiCol_Button] = ImVec4((55.0f / 255.0f), (66.0f / 255.0f), (250.0f / 255.0f), 1.00f);
            // rgb(83, 82, 237)
            style->Colors[ImGuiCol_ButtonHovered] = ImVec4((83.0f / 255.0f), (82.0f / 255.0f), (237.0f / 255.0f), 1.00f);
            style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            ImGui::SetNextWindowPos(ImVec2(width / 2 - 117.5 / 2, height / 2 + 50));
            ImGui::SetNextWindowBgAlpha(0);
            ImGui::SetNextWindowSize(ImVec2(100, 50));
            ImGui::Begin("Button", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_None | ImGuiTableColumnFlags_NoResize);
            if (ImGui::Button("Start", ImVec2(100, 50))) {
                specs = true;
            }
            ImGui::End();
        }
        else {
            style->WindowPadding = ImVec2(15, 15);
            style->WindowRounding = 5.0f;
            style->FramePadding = ImVec2(5, 5);
            style->FrameRounding = 4.0f;
            style->ItemSpacing = ImVec2(12, 8);
            style->ItemInnerSpacing = ImVec2(8, 6);
            style->IndentSpacing = 25.0f;
            style->ScrollbarSize = 15.0f;
            style->ScrollbarRounding = 9.0f;
            style->GrabMinSize = 5.0f;
            style->GrabRounding = 3.0f;

            style->Colors[ImGuiCol_Text] = ImVec4((255.0f), (255.0f), (255.0f), 1.00f);
            style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_Border] = ImVec4((55.0f / 255.0f), (66.0f / 255.0f), (250.0f / 255.0f), 1.00f);
            style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
            style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
            style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            // rgb(44, 62, 80)
            style->Colors[ImGuiCol_Button] = ImVec4((60.0f / 255.0f), (29.0f / 255.0f), (73.0f / 255.0f), 1.00f);
            // rgb(142, 68, 173)
            style->Colors[ImGuiCol_ButtonHovered] = ImVec4((142.0f / 255.0f), (68.0f / 255.0f), (173.0f / 255.0f), 1.00f);
            style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
            style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
            style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Begin("Drawing HyperCube Options", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::PopStyleColor();
            ImGui::Checkbox("Draw Shape", &drawCube);
            ImGui::SameLine();
            if (ImGui::Button("close", ImVec2(60, 25))) {
                specs = false;
            }
            ImGui::SliderInt("D", &i2, 0, atoi(name), "%d-dimensional hypercube");
            ImGui::TextColored(ImVec4(255, 0, 255, 255), cstr);
            ImGui::InputText("", name, 7, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(255, 0, 255, 255), name);
            ImGui::NewLine();
            ImGui::SliderFloat("SIZE", &size, 0.001, 1.0);
            ImGui::SliderInt("FOV", &ifov, 30, 120, "%d");
            ImGui::NewLine();
            ImGui::Button("Render", ImVec2(250, 60));
            ImGui::BeginChild("Inside color", size2);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "INSIDE COLOR");
            ImGui::ColorPicker3("", (float*)&color);
            ImGui::TextColored(edge_color, "EDGE COLOR");
            ImGui::ColorEdit3("", (float*)&edge_color);
            ImGui::EndChild();
            ImGui::NewLine();
            ImGui::End();
        }

        // Checkbox that appears in the window

        // Renders the ImGUI elements
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Swap the screen buffers
        glfwSwapBuffers(window);
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
    }
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();

    return EXIT_SUCCESS;
}