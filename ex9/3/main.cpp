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

    void add_edge(int_fast32_t from, int_fast32_t to, int_fast32_t flow, int_fast32_t capacity, int_fast32_t cost);

    void get_ans(int_fast32_t start_node, int_fast32_t end_node, int_fast32_t n);

private:
    int_fast32_t n_nodes;
    int_fast32_t cum_cost = 0;
    std::vector<int_fast32_t> cost;
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
        min_path_nodes.emplace_back(-1, -1);
    }
}

void Graph::add_edge(int_fast32_t from, int_fast32_t to, int_fast32_t flow, int_fast32_t capacity, int_fast32_t cost_) {
    Edge e1{from, to, flow, capacity, cost_}; // прямое ребро
    Edge e2{to, from, -flow, 0, -cost_}; // обратное ребро

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

                if (!is_in_queue[edge.to]) {
                    queue.push(edge.to);// засунуть в очередь
                    is_in_queue[edge.to] = true;
                }
            }
        }
    }
    if (cost[end_node] == INF) { // условие выхода из функции*/
        return false;
    }
    // протолкнуть новый поток и прибавить его стоимость

    int_fast32_t max_flow = INF;
    int_fast32_t next_node = end_node;

    while (next_node != start_node) { // найти за линию максимум его пропускной способности
        int_fast32_t ind_node = min_path_nodes[next_node].first;
        int_fast32_t ind_edge = min_path_nodes[next_node].second;

        int_fast32_t flow = nodes[ind_node][ind_edge].capacity - nodes[ind_node][ind_edge].flow;
        max_flow = std::min(flow, max_flow);
        next_node = ind_node;
    }

    next_node = end_node;
    while (next_node != start_node) { // протолкнуть поток
        int_fast32_t ind_node = min_path_nodes[next_node].first;
        int_fast32_t ind_edge = min_path_nodes[next_node].second;

        cum_cost += max_flow * nodes[ind_node][ind_edge].cost;

        // увеличить поток !!!!
        nodes[ind_node][ind_edge].flow += max_flow;
        nodes[next_node][nodes[ind_node][ind_edge].rev_node].flow -= max_flow;
        next_node = ind_node;
    }
    return true;
}


void Graph::get_ans(int_fast32_t start_node, int_fast32_t end_node, int_fast32_t n) {
    cum_cost = 0;

    for (int i = 0; i < n_nodes; i++) {
        // TODO: Релаксация должна начинаться не с начального ребра ???
      bellman_ford(start_node, end_node);
    }
    if (cum_cost > 0){
        std::cout << "SUBOPTIMAL\n";
        for (int_fast32_t i = 1; i < n + 1; i++){
            for (auto & edge : nodes[i]){
                if (edge.capacity > 0){
                    std::cout << edge.flow << " ";
                }
            }
            std::cout << "\n";
        }

    } else{
        std::cout <<  "OPTIMAL\n";
    }

}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast32_t n, m;
    std::cin >> n >> m;
    Graph graph = Graph(n + m + 2);

    int_fast32_t x, y, b;
    std::vector<std::tuple<int_fast32_t, int_fast32_t, int_fast32_t>> buildings;
    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> x >> y >> b;
        buildings.emplace_back(x, y, b);
    }

    int_fast32_t p, q, c;
    std::vector<std::tuple<int_fast32_t, int_fast32_t, int_fast32_t>> shelters;
    for (int_fast32_t i = 0; i < m; i++) {
        std::cin >> p >> q >> c;
        shelters.emplace_back(p, q, c);
    }

    // предопределить исток
    for (int_fast32_t i = 0; i < n; i++) {
        graph.add_edge(0, i + 1, 0, std::get<2>(buildings[i]), 0);
    }

    int_fast32_t flow;
    for (int_fast32_t i = 0; i < n; i++) {
        for (int_fast32_t j = 0; j < m; j++) {
            std::cin >> flow;

            graph.add_edge(i + 1, j + n + 1, flow, std::get<2>(buildings[i]),
                           1 + std::abs(std::get<0>(buildings[i]) - std::get<0>(shelters[j])) +
                           std::abs(std::get<1>(buildings[i]) - std::get<1>(shelters[j])));
        }
    }

    for (int_fast32_t i = 0; i < m; i++) {
        graph.add_edge(n + i + 1, n + m + 1, 0, std::get<2>(shelters[i]), 0);
    }

    graph.get_ans(0, n + m + 1, n);

    return 0;
}

/*
3 4
-3 3 5
-2 -2 6
2 2 5
-1 1 3
1 1 4
-2 -2 7
0 -1 3
3 1 1 0
0 0 6 0
0 3 0 2

SUBOPTIMAL
3 0 1 1
0 0 6 0
0 4 0 1




3 4
-3 3 5
-2 -2 6
2 2 5
-1 1 3
1 1 4
-2 -2 7
0 -1 3
3 0 1 1
0 0 6 0
0 4 0 1

OPTIMAL
 */
