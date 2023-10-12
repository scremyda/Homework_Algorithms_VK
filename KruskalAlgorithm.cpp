#include <iostream>
#include <vector>
#include <algorithm>
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

class DSU {
 public:
    DSU(int count) : parent(count), rank(count, 0) {
        for (int i = 0; i < parent.size(); ++i) {
            parent[i] = i;
        }
    }

    int FindParent(int v) {
        if (v == parent[v]) {
            return v;
        }
        parent[v] = FindParent(parent[v]);
        return parent[v];
    }

    void Union(int v1, int v2) {
        int p1 = FindParent(v1);
        int p2 = FindParent(v2);
        if (p1 == p2) {
            return;
        }
        if (rank[p1] < rank[p2]) {
            parent[p1] = p2;
        } else {
            parent[p2] = p1;
            if (rank[p1] == rank[p2]) {
                ++rank[p2];
            }
        }
    }

 private:
    std::vector<int> parent;
    std::vector<int> rank;
};


int KruskalAlgorithm(const IGraph &graph) {
    std::vector<std::pair<int, std::pair<int, int>>> edgesList;
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (auto nextVertex : graph.GetNextVertices(i)) {
            edgesList.emplace_back(nextVertex.second,
            std::make_pair(i, nextVertex.first));
        }
    }
    std::sort(edgesList.begin(), edgesList.end());

    int minWeight = 0;
    DSU dsu(graph.VerticesCount());
    for (const auto& edge : edgesList) {
        int firstNeighbour = edge.second.first;
        int secondNeighbour = edge.second.second;
        if (dsu.FindParent(firstNeighbour) != dsu.FindParent(secondNeighbour)) {
            dsu.Union(firstNeighbour, secondNeighbour);
            minWeight += edge.first;
        }
    }
    return minWeight;
}


int main() {
    int n, m;
    std::cin >> n >> m;

    int from, to, time;
    ListsGraph graph(n);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> from >> to >> time;
        graph.AddEdge(from - 1, to - 1, time);
    }

    std::cout << KruskalAlgorithm(graph) << std::endl;
    return 0;
}
