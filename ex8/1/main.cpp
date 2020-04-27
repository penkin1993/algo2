#include <iostream>
#include <vector>
#include <queue>

int INF = 1e6;

struct Edge {
    int from{}, to{};
    int flow{}, capacity{};
    int rev_node = 0;
};

class Graph {

public:
    explicit Graph(int n_nodes);

    void get_all_flows();

    void add_edge(int from, int to, int capacity);

    int get_max_flow(int start_node, int end_node);


private:
    int n_nodes;
    std::vector<int> dist;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<std::pair<int, int>> edge_buffer;

    bool bfs(int start_node, int end_node);

    int dfs(int start_node, int end_node, int min_capacity);
};

Graph::Graph(int n_nodes_) {
    n_nodes = n_nodes_;
    for (int i = 0; i < n_nodes_; i++) {
        dist.push_back(-1);
        nodes.emplace_back();
    }
}

void Graph::add_edge(int from, int to, int capacity) {
    Edge e1{from, to, 0, capacity, 0}; // прямое ребро
    Edge e2{to, from, 0, capacity, 0}; // обратное ребро
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);

    nodes[from].back().rev_node = nodes[to].size() - 1;
    nodes[to].back().rev_node = nodes[from].size() - 1;
    edge_buffer.emplace_back(from, nodes[from].size());

}

int Graph::dfs(int start_node, int end_node, int min_capacity) {
    if (min_capacity == 0) {
        return 0;
    }
    if (start_node == end_node) {
        return min_capacity;
    }
    int next_flow;
    for (auto &node : nodes[start_node]) {
        int join_node = node.to;

        if (dist[join_node] != dist[start_node] + 1) {
            continue;
        }
        next_flow = dfs(join_node, end_node, std::min(min_capacity, node.capacity - node.flow));

        if (next_flow > 0) {
            next_flow = std::min(next_flow, node.capacity - node.flow);
            node.flow += next_flow;

            nodes[node.to][node.rev_node].flow -= next_flow;
            return next_flow;
        }
    }
    return 0;
}

int Graph::get_max_flow(int start_node, int end_node) {

    int flow = 0;
    int add_flow;
    while (bfs(start_node, end_node)) {
        while (true) {
            add_flow = dfs(start_node, end_node, INF);
            if (add_flow == 0) {
                break;
            }
            flow += add_flow;
        }
    }
    //std::cout << flow << " \n";
    return flow;
}

void Graph::get_all_flows() {
    for (auto &edge : edge_buffer) {
        std::cout << nodes[edge.first][edge.second - 1].flow << "\n";
    }
}

bool Graph::bfs(int start_node, int end_node) {
    for (int i = 0; i < n_nodes; i++) {
        dist[i] = -1;
    }

    std::queue<int> queue;
    queue.push(start_node);
    dist[start_node] = 0;

    while (!queue.empty()) {
        int next_node = queue.front();
        queue.pop();
        for (auto &node : nodes[next_node]) {
            int join_edge = node.to;

            if ((dist[join_edge] == -1) && (node.flow < node.capacity)) {
                queue.push(join_edge);

                dist[join_edge] = dist[next_node] + 1;
            }
        }
    }
    return bool(dist[end_node] != -1);
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n_nodes, n_edges;
    std::cin >> n_nodes;
    Graph graph = Graph(n_nodes);

    std::cin >> n_edges;
    int from, to, val;

    for (int i = 0; i < n_edges; i++) {
        std::cin >> from >> to >> val;
        graph.add_edge(from - 1, to - 1, val);
    }


    int res = graph.get_max_flow(0, n_nodes - 1);
    std::cout << res << "\n";
    graph.get_all_flows();

    return 0;
}

/*
2
2
1 2 1
2 1 3

7
11
1 2 7
1 2 7
1 3 7
1 4 7
2 3 7
2 5 7
3 6 7
4 7 7
5 4 7
5 6 7
6 7 7

 */