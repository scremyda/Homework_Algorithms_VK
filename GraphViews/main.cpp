#include "SetGraph.hpp"
#include "ArcGraph.hpp"
#include "ListsGraph.hpp"
#include "MatrixGraph.hpp"

#include <iostream>
#include <functional>
#include <queue>


void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func) {
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);
        for (int nextVertex: graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void BFS(const IGraph &graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex: graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void DFS(const IGraph &graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            DFS(graph, i, visited, func);
        }
    }
}



int main() {
    ListsGraph listGraph(9);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    listGraph.AddEdge(7, 8);

    std::cout << "BFS LIST GRAPH:   ";
    BFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << "DFS LIST GRAPH:   ";
    DFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << std::endl;
    MatrixGraph matrixGraph(listGraph);
    std::cout << "BFS MATRIX GRAPH: ";
    BFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << "DFS MATRIX GRAPH: ";
    DFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << std::endl;
    SetGraph setGraph(matrixGraph);
    std::cout << "BFS SET GRAPH:    ";
    BFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << "DFS SET GRAPH:    ";
    DFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << std::endl;
    ArcGraph arcGraph(matrixGraph);
    std::cout << "BFS ARC GRAPH:    ";
    BFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << "DFS ARC GRAPH:    ";
    DFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    return 0;
}

