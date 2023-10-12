#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <set>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>>
    GetNextVertices(int vertex) const = 0;

    virtual std::vector<std::pair<int, int>>
    GetPrevVertices(int vertex) const = 0;
};


class ListsGraph : public IGraph {
 public:
    ListsGraph(int vertexCount);

    // IGraph
    virtual void AddEdge(int from, int to, int weight) override;

    virtual int VerticesCount() const override;

    virtual std::vector<std::pair<int, int>>
    GetNextVertices(int vertex) const override;

    virtual std::vector<std::pair<int, int>>
    GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<std::pair<int, int>>> adjLists;
    std::vector<std::vector<std::pair<int, int>>> prevAdjLists;
};


ListsGraph::ListsGraph(int vertexCount) {
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

void ListsGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());
    assert(weight >= 0);

    adjLists[from].emplace_back(std::make_pair(to, weight));
    prevAdjLists[to].emplace_back(std::make_pair(from, weight));
}

int ListsGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<std::pair<int, int>> ListsGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

std::vector<std::pair<int, int>> ListsGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
}

int Dijkstra(const IGraph &graph, int vertex, int end) {
    std::vector<int> distance(graph.VerticesCount(), 0);

    std::vector<bool> visited(graph.VerticesCount(), false);
    visited[vertex] = true;

    std::set<std::pair<int, int>> que;
    que.emplace(0, vertex);

    int currentVertex;
    while (!que.empty()) {
        currentVertex = que.begin()->second;
        que.erase(que.begin());
        visited[currentVertex] = true;

        for (const auto &nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex.first]) {
                que.emplace(distance[nextVertex.first], nextVertex.first);

                distance[nextVertex.first] =
                distance[currentVertex] + nextVertex.second;

                visited[nextVertex.first] = true;
                
            } else if (distance[nextVertex.first] >
            distance[currentVertex] + nextVertex.second) {
                que.erase(std::make_pair(distance[nextVertex.first],
                nextVertex.first));

                distance[nextVertex.first] =
                distance[currentVertex] + nextVertex.second;

                que.emplace(distance[nextVertex.first], nextVertex.first);
            }
        }
    }
    return distance[end];
}


int main() {
    int n, m;
    std::cin >> n >> m;

    int from, to, time;
    ListsGraph graph(n);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> from >> to >> time;
        graph.AddEdge(from, to, time);
        graph.AddEdge(to, from, time);
    }

    int start, end;
    std::cin >> start >> end;

    std::cout << Dijkstra(graph, start, end) << std::endl;

    return 0;
}
