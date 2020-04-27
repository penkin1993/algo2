#include <iostream>
#include <vector>
#include <queue>

int_fast64_t INF_FLOW = 1e6;

struct Edge {
    int_fast64_t from, to;
    int_fast64_t flow, capacity;
    Edge *rev_node = nullptr;
};

class Graph {

public:
    explicit Graph(
            int_fast64_t n_nodes); // добавть top 5 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity);

    int_fast64_t get_max_flow(int_fast64_t start_node, int_fast64_t end_node);

    void get_all_flows();

private:
    int_fast64_t n_nodes;
    std::vector<int_fast64_t> dist;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<Edge *> edge_buffer;

    bool bfs(int_fast64_t start_node, int_fast64_t end_node);

    int_fast64_t dfs(int_fast64_t start_node, int_fast64_t end_node, int_fast64_t min_capacity);
};

Graph::Graph(int_fast64_t n_nodes_) {
    n_nodes = n_nodes_;
    dist.emplace_back(n_nodes_);
    nodes.emplace_back(n_nodes_);
}

void Graph::add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity) {
    Edge e1{from, to, 0, capacity, nullptr}; // прямое ребро
    Edge e2{to, from, 0, 0, &e2}; // обратное ребро
    edge_buffer.push_back(&e1);
    e1.rev_node = &e2;
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);
}

bool Graph::bfs(int_fast64_t start_node, int_fast64_t end_node) {
    for (int_fast64_t i = 1; i <= n_nodes; i++) {
        dist[i] = -1;
    }
    std::queue<int> queue;
    queue.push(start_node);
    dist[start_node] = 0;

    while (!queue.empty()) {
        int_fast64_t next_node = queue.front();
        queue.pop();
        for (auto &node : nodes[next_node]) {
            int_fast64_t join_edge = node.to;

            if ((dist[join_edge] == -1) && (node.flow < node.capacity)) {
                queue.push(join_edge);
                dist[join_edge] = dist[join_edge] + 1;
            }
        }
    }
    return dist[end_node] != -1;
}

int_fast64_t Graph::dfs(int_fast64_t start_node, int_fast64_t end_node, int_fast64_t min_capacity) {
    if (min_capacity == 0) return 0;
    if (start_node == end_node) return min_capacity;
    int_fast64_t current_node = start_node;

    for (auto &node : nodes[current_node]) {
        int_fast64_t join_edge = node.to;
        if (dist[join_edge] != dist[current_node] + 1) {
            continue;
        }
        int_fast64_t next_flow = dfs(join_edge, end_node, std::min(min_capacity, node.capacity - node.flow));
        if (next_flow > 0) {
            next_flow = std::min(next_flow, node.capacity - node.flow);
            node.flow += next_flow;
            node.rev_node->flow -= next_flow;
            return next_flow;
        }
    }
    return 0;
}

int_fast64_t Graph::get_max_flow(int_fast64_t start_node, int_fast64_t end_node) {
    int_fast64_t flow = 0;
    while (bfs(start_node, end_node)) {
        while (true) {
            int_fast64_t add_flow = dfs(start_node, end_node, INF_FLOW);
            if (add_flow == 0) break;
            flow += add_flow;
        }
    }
    return flow;
}

void Graph::get_all_flows() {
    for (auto &edge : edge_buffer) {
        std::cout << edge->flow << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n_nodes, n_edges;
    std::cin >> n_nodes;
    Graph graph = Graph(n_nodes);

    std::cin >> n_edges;
    int_fast64_t from, to, val;

    for (int_fast64_t i = 0; i < n_edges; i++) {
        std::cin >> from >> to >> val;
        graph.add_edge(from, to, val);
    }
    int_fast64_t res = graph.get_max_flow(1, n_edges);
    std::cout << res;
    graph.get_all_flows();

    return 0;
}