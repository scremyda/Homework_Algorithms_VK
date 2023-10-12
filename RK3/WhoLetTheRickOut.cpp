#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <climits>
#include <cassert>


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

int Dijkstra(const IGraph& graph, int x, int y) {
    std::vector<int> distance(graph.VerticesCount(), INT_MAX);
    distance[x] = 0;

    std::priority_queue<std::pair<int, int>,
    std::vector<std::pair<int, int>>, std::greater<>> que;

    que.emplace(0, x);

    int currentVertex;
    while (!que.empty()) {
        currentVertex = que.top().second;
        que.pop();

        for (const auto& [nextVertex, weight] :
        graph.GetNextVertices(currentVertex)) {
            int newDistance = distance[currentVertex] + weight;
            if (newDistance < distance[nextVertex]) {
                distance[nextVertex] = newDistance;
                que.emplace(distance[nextVertex], nextVertex);
            }
        }
    }

    return distance[y];
}

int main() {
    int a, b, M, x, y;
    std::cin >> a >> b >> M >> x >> y;

    ListsGraph graph(M);

    for (int i = 0; i < M; ++i) {
        int nextVertex = (i + 1) % M;
        graph.AddEdge(i, nextVertex, a);

        nextVertex = ((long long)i * i + 1) % M;
        graph.AddEdge(i, nextVertex, b);
    }

    std::cout << Dijkstra(graph, x, y) << std::endl;

    return 0;
}
