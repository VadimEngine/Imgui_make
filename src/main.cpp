#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <cmath> 

void buildImgui() {
    static float values[50];
    for (int i = 0; i < 50; ++i) {
        values[i] = 0.5f + 0.4f * cosf((2 * 3.14159265358979323846f / 50) * i + ImGui::GetTime());
        //values[i] = 0.5f + 0.4f * cosf((2 * 3.14159265358979323846f / 50));
    }
    static int start = 0;
    static int end = 50;


    // ImGui content
    ImGui::Begin("ImGui Window");
    ImGui::Text("Hello, ImGui!");
    ImGui::PlotLines("Samples", values, end, start);
    ImGui::InputInt("Start", &start);
    ImGui::InputInt("End", &end);
    ImGui::PlotHistogram("Histogram", values, end, start, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
    if (ImPlot::BeginPlot("My Plot")) {
        ImPlot::PlotBars("My Bar Plot", values, 11);
        ImPlot::EndPlot();
    }
    ImGui::End();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Context profiles are only defined for OpenGL version 3.2 and above
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple GLFW Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set up imgui
    const char* glsl_version = "#version 330";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //disable .ini file generations
    io.IniFilename = nullptr;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cout << "ImGui_ImplGlfw_InitForOpenGL failed" << std::endl;
    }

    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        std::cout << "ImGui_ImplOpenGL3_Init failed" << std::endl;
    }

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Poll for and process events
        glfwPollEvents();

        // render imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        buildImgui();

        //  Finish imgui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Clean up imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    // Clean up
    glfwTerminate();

    return 0;
}