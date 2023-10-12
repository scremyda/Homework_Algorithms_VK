#include "ArcGraph.hpp"

#include <cassert>

ArcGraph::ArcGraph(int vertexCount) {
    adjArc.resize(vertexCount);

}

ArcGraph::ArcGraph(const IGraph& graph) {
    adjArc.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (const auto &vertices : nextVertices) {
            adjArc.emplace_back(i, vertices);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjArc.size());
    assert(to >= 0 && to < adjArc.size());

    adjArc.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    int count = 0;
    for (const auto& edge : adjArc) {
        count = std::max(count, std::max(edge.first, edge.second));
    }
    return count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjArc.size());

    std::vector<int> nextVertices;
    for (const auto &edge : adjArc) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjArc.size());

    std::vector<int> prevVertices;
    for (const auto &edge : adjArc) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }
    return prevVertices;
}