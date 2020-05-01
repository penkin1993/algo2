#include <iostream>
#include <vector>
#include <queue>

int_fast64_t INF = 1e6;

struct Edge {
    int_fast64_t from{}, to{};
    int_fast64_t flow{}, capacity{}, cost{};
    int_fast64_t rev_node = 0;
};

class Graph {

public:
    explicit Graph(int_fast64_t n_nodes);

    ~Graph() = default;

    Graph(Graph &&) = default;

    Graph &operator=(const Graph &) = delete;

    Graph &operator=(Graph &&) = delete;

    void add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity, int_fast64_t cost);

    int_fast64_t get_max_flow(int_fast64_t start_node, int_fast64_t end_node);


private:
    int_fast64_t n_nodes;
    std::vector<int_fast64_t> dist;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<int_fast64_t> first_node_del;

    std::vector<std::pair<int_fast64_t, int_fast64_t>> edge_buffer;

    bool bfs(int_fast64_t start_node, int_fast64_t end_node);

    int_fast64_t dfs(int_fast64_t start_node, int_fast64_t end_node, int_fast64_t min_capacity);
};

Graph::Graph(int_fast64_t n_nodes_) {
    n_nodes = n_nodes_;
    for (int_fast64_t i = 0; i < n_nodes_; i++) {
        dist.push_back(-1);
        nodes.emplace_back();
        first_node_del.push_back(0);
    }
}

void Graph::add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity, int_fast64_t cost) {
    Edge e1{from, to, 0, capacity, cost}; // прямое ребро
    Edge e2{to, from, 0, 0, -cost}; // обратное ребро
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);

    nodes[from].back().rev_node = nodes[to].size() - 1;
    nodes[to].back().rev_node = nodes[from].size() - 1;
    edge_buffer.emplace_back(from, nodes[from].size());
}







int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n_nodes, n_edges;
    std::cin >> n_nodes;
    Graph graph = Graph(n_nodes);

    std::cin >> n_edges;
    int_fast64_t from, to, capacity, cost;

    for (int_fast64_t i = 0; i < n_edges; i++) {
        std::cin >> from >> to >> capacity >> cost;
        graph.add_edge(from - 1, to - 1, capacity, cost);
    }
    int_fast64_t res = graph.get_max_flow(0, n_nodes - 1);
    std::cout << res << "\n";

    return 0;
}
