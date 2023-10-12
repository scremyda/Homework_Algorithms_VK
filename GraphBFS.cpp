#include <iostream>
#include <cassert>
#include <vector>
#include <queue>

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
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
}

int ListsGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListsGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

std::vector<int> ListsGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
}

int countShortestWithBFS(const IGraph &graph, int vertex, int end) {
    std::queue<int> que;
    que.push(vertex);

    std::vector<int> pathsCounter(graph.VerticesCount(), 0);
    pathsCounter[vertex] = 1;

    std::vector<int> distance(graph.VerticesCount(), 0);
    distance[vertex] = 0;

    std::vector<bool> visited(graph.VerticesCount(), false);
    visited[vertex] = true;

    int currentVertex;
    while (!que.empty()) {
        currentVertex = que.front();
        que.pop();

        for (const int &nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                distance[nextVertex] = distance[currentVertex] + 1;
                visited[nextVertex] = true;

                que.push(nextVertex);
            }
            
            if (distance[nextVertex] > distance[currentVertex]) {
                pathsCounter[nextVertex] += pathsCounter[currentVertex];
            }
        }
    }
    return pathsCounter[end];
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

    int start, end;
    std::cin >> start >> end;

    std::cout << countShortestWithBFS(graph, start, end) << std::endl;

    return 0;
}
