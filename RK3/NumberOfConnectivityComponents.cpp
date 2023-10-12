#include <iostream>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListsGraph : public IGraph {
 public:
    ListsGraph(int vertexCount);
    ListsGraph(const IGraph& graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> prevAdjLists;
};

ListsGraph::ListsGraph(int vertexCount) {
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

ListsGraph::ListsGraph(const IGraph& graph) {
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjLists[i] = graph.GetNextVertices(i);
        prevAdjLists[i] = graph.GetPrevVertices(i);
    }
}

void ListsGraph::AddEdge(int from, int to) {
    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
}

int ListsGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListsGraph::GetNextVertices(int vertex) const {
    return adjLists[vertex];
}

std::vector<int> ListsGraph::GetPrevVertices(int vertex) const {
    return prevAdjLists[vertex];
}

void DFS(const IGraph& graph, int vertex, std::vector<bool>& visited) {
    visited[vertex] = true;
    for (const int& nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited);
        }
    }
}

int componentsCounter(const IGraph& graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    int count = 0;
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            DFS(graph, i, visited);
            ++count;
        }
    }
    return count;
}

int main() {
    int n, m;
    std::cin >> n >> m;

    int from, to;
    ListsGraph graph(n);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    std::cout << componentsCounter(graph) << std::endl;

    return 0;
}
