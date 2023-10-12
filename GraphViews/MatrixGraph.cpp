#include "MatrixGraph.hpp"

#include <cassert>

MatrixGraph::MatrixGraph(int vertexCount) {
    adjMatrix.resize(vertexCount, std::vector<int>(vertexCount));
}

MatrixGraph::MatrixGraph(const IGraph& graph) {
    adjMatrix.resize(graph.VerticesCount(), std::vector<int>(graph.VerticesCount()));
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (const auto &vertices : nextVertices) {
            adjMatrix[i][vertices] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());

    adjMatrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> NextVertices;
    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[vertex][i]) {
            NextVertices.push_back(i);
        }
    }
    return NextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> PrevVertices;
    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[i][vertex]) {
            PrevVertices.push_back(i);
        }
    }
    return PrevVertices;
}