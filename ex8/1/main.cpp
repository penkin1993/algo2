#include <iostream>
#include <vector>
#include <queue>

int_fast64_t INF = 1e6;

struct Edge {
    int_fast64_t from{}, to{};
    int_fast64_t flow{}, capacity{};
    int_fast64_t rev_node = 0;
};

class Graph {

public:
    explicit Graph(int_fast64_t n_nodes);

    void get_all_flows();

    void add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity);

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

void Graph::add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity) {
    Edge e1{from, to, 0, capacity, 0}; // прямое ребро
    Edge e2{to, from, 0, capacity, 0}; // обратное ребро
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);

    nodes[from].back().rev_node = nodes[to].size() - 1;
    nodes[to].back().rev_node = nodes[from].size() - 1;
    edge_buffer.emplace_back(from, nodes[from].size());

}

int_fast64_t Graph::dfs(int_fast64_t start_node, int_fast64_t end_node, int_fast64_t min_capacity) {
    if (min_capacity == 0) {
        return 0;
    }
    if (start_node == end_node) {
        return min_capacity;
    }
    int_fast64_t next_flow;
    int_fast64_t join_node;
    while (first_node_del[start_node] < nodes[start_node].size()) {
        auto &node = nodes[start_node][first_node_del[start_node]];
        join_node = node.to;

        if (dist[join_node] == dist[start_node] + 1) {
            next_flow = dfs(join_node, end_node, std::min(min_capacity, node.capacity - node.flow));
            if (next_flow > 0) {
                node.flow += next_flow;
                nodes[node.to][node.rev_node].flow -= next_flow;
                return next_flow;
            }
        }
        first_node_del[start_node]++;
    }
    return 0;
}


bool Graph::bfs(int_fast64_t start_node, int_fast64_t end_node) {

    for (int_fast64_t i = 0; i < n_nodes; i++) {
        first_node_del[i] = 0;
        dist[i] = -1;
    }

    std::queue<int_fast64_t> queue;
    queue.push(start_node);
    dist[start_node] = 0;

    while (!queue.empty()) {
        int_fast64_t next_node = queue.front();
        queue.pop();
        for (auto &node : nodes[next_node]) {
            int_fast64_t join_node = node.to;

            if ((dist[join_node] == -1) && (node.flow < node.capacity)) {
                queue.push(join_node);

                dist[join_node] = dist[next_node] + 1;
            }
        }
    }
    return bool(dist[end_node] != -1);
}


int_fast64_t Graph::get_max_flow(int_fast64_t start_node, int_fast64_t end_node) {

    int_fast64_t flow = 0;
    int_fast64_t add_flow;
    while (bfs(start_node, end_node)) {
        while (true) {
            add_flow = dfs(start_node, end_node, INF);
            if (add_flow == 0) {
                break;
            }
            flow += add_flow;
        }
    }
    return flow;
}

void Graph::get_all_flows() {
    for (auto &edge : edge_buffer) {
        std::cout << nodes[edge.first][edge.second - 1].flow << "\n";
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
        graph.add_edge(from - 1, to - 1, val);
    }

    int_fast64_t res = graph.get_max_flow(0, n_nodes - 1);
    std::cout << res << "\n";
    graph.get_all_flows();

    return 0;
}
