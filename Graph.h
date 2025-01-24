#ifndef INC_2LABMEPHI4_GRAPH_H
#define INC_2LABMEPHI4_GRAPH_H
#include <vector>
#include <imgui.h>
#include <cmath>
#include <list>
class Graph {
private:
    int V;
    std::vector<std::list<int>> matrix;

public:
    Graph(int V) {
        assert(V > 0);  // Проверка на корректность числа вершин
        this->V = V;
        matrix.resize(V);
    }

    void addEdge(int v, int w) {
        assert(v >= 0 && v < V && w >= 0 && w < V);  // Проверка индексов
        matrix[v].push_back(w);
        matrix[w].push_back(v);
    }

    void removeEdge(int v, int w) {
        assert(v >= 0 && v < V && w >= 0 && w < V);  // Проверка индексов
        matrix[v].remove(w);
        matrix[w].remove(v);
    }

    void addVertex() {
        V++;
        matrix.resize(V);
    }

    void removeVertex(int v) {
        assert(v >= 0 && v < V);  // Проверка индекса вершины
        for (int i = 0; i < V; ++i) {
            matrix[i].remove(v);
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
        assert(v >= 0 && v < V && w >= 0 && w < V);  // Проверка индексов
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
};

#endif
