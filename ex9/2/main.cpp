#include <iostream>
#include <vector>
#include <queue>

int_fast32_t INF = 1e9;

struct Edge {
    int_fast32_t from{}, to{};
    int_fast32_t flow{}, capacity{}, cost{};
    int_fast32_t index = 0;
    int_fast32_t rev_node = 0;
};

class Graph {

public:
    explicit Graph(int_fast32_t n_nodes);

    ~Graph() = default;

    Graph(Graph &&) = default;

    Graph &operator=(const Graph &) = delete;

    Graph &operator=(Graph &&) = delete;

    void add_edge(int_fast32_t from, int_fast32_t to, int_fast32_t capacity, int_fast32_t cost);

    int_fast32_t get_max_flow(int_fast32_t start_node, int_fast32_t end_node);

    void print_ind_edges();

private:
    int_fast32_t n_nodes;
    int_fast32_t cum_cost = 0;
    std::vector<bool> is_done;
    std::vector<int_fast32_t> cost;
    std::vector<int_fast32_t> node_flow_ind;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<std::pair<int_fast32_t, int_fast32_t>> min_path_nodes;

    // индекс вершины и ребра в ней, с которой можно попасть в данную с наименьшей стоимостью
    bool bellman_ford(int_fast32_t start_node, int_fast32_t end_node);
};

Graph::Graph(int_fast32_t n_nodes_) {
    n_nodes = n_nodes_;
    for (int_fast32_t i = 0; i < n_nodes_; i++) {
        nodes.emplace_back();
        cost.push_back(INF);
        node_flow_ind.push_back(-1);
        min_path_nodes.emplace_back(-1, -1);
        is_done.push_back(false);
    }
}

void Graph::add_edge(int_fast32_t from, int_fast32_t to, int_fast32_t capacity, int_fast32_t cost_) {
    Edge e1{from, to, 0, capacity, cost_}; // прямое ребро
    Edge e2{to, from, 0, 0, -cost_}; // обратное ребро
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);

    nodes[from].back().index = nodes[from].size() - 1;
    nodes[to].back().index = nodes[to].size() - 1;

    nodes[from].back().rev_node = nodes[to].size() - 1;
    nodes[to].back().rev_node = nodes[from].size() - 1;
}


bool Graph::bellman_ford(int_fast32_t start_node, int_fast32_t end_node) {
    for (int_fast32_t i = 0; i < n_nodes; i++) {
        cost[i] = INF;
        min_path_nodes[i].first = -1;
        min_path_nodes[i].second = -1;
    }

    std::queue<int_fast32_t> queue;
    std::vector<bool> is_in_queue(n_nodes, false); // вспомогательный массив на проверку элемента в очереди

    queue.push(start_node);
    cost[start_node] = 0;

    while (!queue.empty()) {
        int_fast32_t next_node = queue.front();
        queue.pop();
        is_in_queue[next_node] = false;

        for (auto &edge : nodes[next_node]) {
            int_fast32_t sum_cost = cost[edge.from] + edge.cost;
            if ((edge.flow < edge.capacity) && (cost[edge.to] > sum_cost)) {
                cost[edge.to] = sum_cost; // обновить стоимость
                min_path_nodes[edge.to] = std::make_pair(edge.from, edge.index);
                // присвоить ссылку на ребро по которому пришли

                if ((!is_in_queue[edge.to])){ // && (edge.to > next_node)) {
                    queue.push(edge.to);// засунуть в очередь
                    is_in_queue[edge.to] = true;
                }
            }
        }
    }
    // протолкнуть новый поток и прибавить его стоимость

    int_fast32_t max_flow = 1;

    int_fast32_t next_node = end_node;
    while (next_node != start_node) { // протолкнуть поток

        int_fast32_t ind_node = min_path_nodes[next_node].first;
        int_fast32_t ind_edge = min_path_nodes[next_node].second;

        cum_cost += max_flow * nodes[ind_node][ind_edge].cost;

        // увеличить поток !!!!
        nodes[ind_node][ind_edge].flow += max_flow;
        nodes[next_node][nodes[ind_node][ind_edge].rev_node].flow -= max_flow;
        next_node = ind_node;

        if (nodes[ind_node][ind_edge].flow == 1) {
            node_flow_ind[ind_node] = nodes[ind_node][ind_edge].index;
        }
    }
    return true;
}


int_fast32_t Graph::get_max_flow(int_fast32_t start_node, int_fast32_t end_node) {
    cum_cost = 0;
    for (int_fast32_t i = 0; i < (n_nodes - 2) / 2; i++) {
        bellman_ford(start_node, end_node);
    }
    return cum_cost;
}

void Graph::print_ind_edges() {
    int_fast32_t n = (n_nodes - 2) / 2;
    for (int_fast32_t i = 1; i < n + 1; i++) {
        std::cout << i << " " << nodes[i][node_flow_ind[i]].index << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast32_t n;
    std::cin >> n;
    Graph graph = Graph(2 * n + 2);

    for (int_fast32_t i = 1; i < n + 1; i++) {
        graph.add_edge(0, i, 1, 0);
    }

    int_fast32_t cost;
    for (int_fast32_t i = 1; i < n + 1; i++) {
        for (int_fast32_t j = n + 1; j < 2 * n + 1; j++) {
            std::cin >> cost;
            graph.add_edge(i, j, 1, cost);
        }
    }

    for (int_fast32_t i = n + 1; i < 2 * n + 1; i++) {
        graph.add_edge(i, 2 * n + 1, 1, 0);
    }

    int_fast32_t res = graph.get_max_flow(0, 2 * n + 1);
    std::cout << res << "\n";
    graph.print_ind_edges();

    return 0;
}
