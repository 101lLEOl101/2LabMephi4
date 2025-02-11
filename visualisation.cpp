#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include "visualisation.h"
#include "Graph.h"

Graph graph(3);

std::vector<int> colors;

void DrawArrow(ImDrawList* drawList, ImVec2 start, ImVec2 end, ImU32 color,
               float thickness = 1.0f,
               float arrowSize = 10.0f, bool w = false, int weight = 1) {
    ImVec2 dir = ImVec2(end.x - start.x, end.y - start.y);
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    dir.x /= length;
    dir.y /= length;

    ImVec2 ortho = ImVec2(-dir.y, dir.x);

    ImVec2 arrowTip = end;
    ImVec2 arrowBase1 = ImVec2(end.x - dir.x * arrowSize + ortho.x * (arrowSize / 2),
                               end.y - dir.y * arrowSize + ortho.y * (arrowSize / 2));
    ImVec2 arrowBase2 = ImVec2(end.x - dir.x * arrowSize - ortho.x * (arrowSize / 2),
                               end.y - dir.y * arrowSize - ortho.y * (arrowSize / 2));

    drawList->AddLine(start, end, color, thickness);

    drawList->AddTriangleFilled(arrowTip, arrowBase1, arrowBase2, color);
    if (w) {
        ImVec2 midPoint = ImVec2((start.x + end.x) / 2 - 10,
                                 (start.y + end.y) / 2 - 10);
        char we[2];
        snprintf(we, sizeof(we), "%d", weight);
        drawList->AddText(midPoint, IM_COL32(255, 255, 255, 255), we);
    }
}

void CreateGraphInterface(Graph& graph) {
    ImGui::Begin("Graph Control Panel");

    if (ImGui::Button("Add Vertex")) {
        graph.addVertex();
    }

    static int vertexToDelete = -1;
    ImGui::InputInt("Vertex to delete", &vertexToDelete);
    if (vertexToDelete >= 1 && vertexToDelete <= graph.vertexCount()) {
        if (ImGui::Button("Delete Vertex")) {
            graph.removeVertex(vertexToDelete - 1);
            vertexToDelete = -1;
        }
    }

    static int startVertex = -1;
    static int endVertex = -1;
    ImGui::InputInt("Start Vertex", &startVertex);
    ImGui::InputInt("End Vertex", &endVertex);

    if (startVertex >= 1 && startVertex <= graph.vertexCount() &&
        endVertex >= 1 && endVertex <= graph.vertexCount()) {
        if (ImGui::Button("Add Edge")) {
            graph.addEdge(startVertex - 1, endVertex - 1);
        }
    }

    static int edgeStartVertex = -1;
    static int edgeEndVertex = -1;
    ImGui::InputInt("Start Edge Vertex", &edgeStartVertex);
    ImGui::InputInt("End Edge Vertex", &edgeEndVertex);

    if (edgeStartVertex >= 1 && edgeStartVertex <= graph.vertexCount() &&
        edgeEndVertex >= 1 && edgeEndVertex <= graph.vertexCount()) {
        if (ImGui::Button("Delete Edge")) {
            graph.removeEdge(edgeStartVertex - 1, edgeEndVertex - 1);
        }
    }

    static int countVertex = -1;
    static int countEdge = -1;
    ImGui::Text("Generate new random Graph");
    ImGui::InputInt("CountVertex", &countVertex);
    ImGui::InputInt("CountEdge", &countEdge);
    if (countVertex >= 1 && countEdge >= 0) {
        if (ImGui::Button("Generate random Graph")) {
            graph = Graph(countVertex, countEdge);
        }
    }

    ImGui::End();
}

void DrawGraph(Graph& graph) {
    const int numNodes = graph.vertexCount();
    const float radius = 300.0f;
    const ImVec2 center(500, 400);
    const float nodeRadius = 15.0f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    std::vector<ImVec2> nodePositions(numNodes);

    for (int i = 0; i < numNodes; i++) {
        float angle = 2.0f * M_PI * i / numNodes;
        nodePositions[i] = ImVec2(center.x + radius * cos(angle), center.y + radius * sin(angle));
    }

    std::vector<int> colors = graph.colorGraph();

    for (int i = 0; i < numNodes; i++) {
        ImU32 nodeColor = IM_COL32(255, 255, 255, 255);
        if (!colors.empty() && colors[i] >= 0) {
            nodeColor = IM_COL32(((colors[i] + 1) * 50) % 255, ((colors[i] + 1) * 100) % 255, ((colors[i] + 1) * 150) % 255, 255);
        }
        drawList->AddCircleFilled(nodePositions[i], nodeRadius, nodeColor);
    }

    for (int i = 0; i < numNodes; i++) {
        for (int j = i + 1; j < numNodes; j++) {
            if (graph.hasEdge(i, j)) {
                DrawArrow(drawList, nodePositions[i], nodePositions[j], IM_COL32(0, 150, 0, 255), 2.0f, 15.0f, false);
                if (i != j) {
                    DrawArrow(drawList, nodePositions[j], nodePositions[i], IM_COL32(0, 150, 0, 255), 2.0f, 15.0f, false);
                }
            }
        }
    }

    ImGui::SetWindowFontScale(3.0f);
    int x_diff = 10, y_diff = 20;
    for (int i = 0; i < numNodes; i++) {
        char label[4];
        snprintf(label, sizeof(label), "%d", i + 1);
        if (i == 9) {
            ImGui::SetWindowFontScale(2.0f);
            y_diff = 15;
            x_diff = 15;
        }
        drawList->AddText(ImVec2(nodePositions[i].x - x_diff, nodePositions[i].y - y_diff), IM_COL32(0, 0, 0, 255), label);
    }
    ImGui::SetWindowFontScale(1.0f);
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int visualisation(){
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui", nullptr, nullptr);
    if (window == nullptr) return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(1);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Graph Viewer");
        DrawGraph(graph);
        ImGui::End();
        CreateGraphInterface(graph);
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}