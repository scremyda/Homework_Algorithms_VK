#include "SetGraph.hpp"

#include <cassert>

SetGraph::SetGraph(int vertexCount) {
    adjSet.resize(vertexCount);

}

SetGraph::SetGraph(const IGraph& graph) {
    adjSet.resize(graph.VerticesCount());
    for (int i = 0; i < adjSet.size(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (const auto &vertices : nextVertices) {
            adjSet[i].insert(vertices);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjSet.size());
    assert(to >= 0 && to < adjSet.size());

    adjSet[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return adjSet.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjSet.size());

    std::vector<int> nextVertices(adjSet[vertex].begin(), adjSet[vertex].end());
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjSet.size());

    std::vector<int> prevVertices;
    for (int i = 0; i < adjSet.size(); ++i) {
        if (adjSet[i].find(vertex) != adjSet[i].end()) {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}