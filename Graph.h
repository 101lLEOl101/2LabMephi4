#ifndef INC_2LABMEPHI4_GRAPH_H
#define INC_2LABMEPHI4_GRAPH_H
#include <vector>
#include <imgui.h>
#include <cmath>
#include <iostream>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> matrix;

public:
    Graph(int V) {
        this->V = V;
        matrix.resize(V);
    }

    Graph(int vertexCount, int edgeCount) {
        if (edgeCount > vertexCount * (vertexCount - 1) / 2) {
            edgeCount = vertexCount * (vertexCount - 1) / 2;
        }

        this->V = vertexCount;
        matrix.resize(V);

        std::vector<std::pair<int, int>> possibleEdges;
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                possibleEdges.emplace_back(i, j);
            }
        }

        std::srand(static_cast<unsigned>(std::time(nullptr)));

        for (int i = 0; i < edgeCount; ++i) {
            int randomIndex = std::rand() % possibleEdges.size();
            auto edge = possibleEdges[randomIndex];

            addEdge(edge.first, edge.second);

            possibleEdges.erase(possibleEdges.begin() + randomIndex);
        }
    }

    void addEdge(int v, int w) {
        if (v >= V || w >= V || v < 0 || w < 0) return;
        matrix[v].push_back(w);
        matrix[w].push_back(v);
    }

    void removeEdge(int v, int w) {
        auto it = std::find(matrix[v].begin(), matrix[v].end(), w);
        if (it != matrix[v].end()) {
            matrix[v].erase(it);
        }

        it = std::find(matrix[w].begin(), matrix[w].end(), v);
        if (it != matrix[w].end()) {
            matrix[w].erase(it);
        }
    }

    void addVertex() {
        V++;
        matrix.resize(V);
    }

    void removeVertex(int v) {
        for (int i = 0; i < V; ++i) {
            auto it = std::find(matrix[i].begin(), matrix[i].end(), v);
            if (it != matrix[i].end()) {
                matrix[i].erase(it);
            }
        }
        matrix.erase(matrix.begin() + v);
        V--;

        for (int i = 0; i < V; ++i) {
            for (auto& neighbor : matrix[i]) {
                if (neighbor > v) {
                    --neighbor;
                }
            }
        }
    }

    bool hasEdge(int v, int w) {
        for (const auto& neighbor : matrix[v]) {
            if (neighbor == w) {
                return true;
            }
        }
        return false;
    }

    int vertexCount() const {
        return V;
    }

    std::vector<int> colorGraph() {
        std::vector<int> color(V, -1);
        color[0] = 0;

        for (int u = 1; u < V; ++u) {
            std::vector<bool> available(V, true);

            for (const int& neighbor : matrix[u]) {
                if (color[neighbor] != -1) {
                    available[color[neighbor]] = false;
                }
            }

            for (int c = 0; c < V; ++c) {
                if (available[c]) {
                    color[u] = c;
                    break;
                }
            }
        }
        return color;
    }
};

#endif