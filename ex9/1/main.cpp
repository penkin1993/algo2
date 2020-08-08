/*
A. Максимальный поток минимальной стоимости
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Задан ориентированный граф, каждое ребро которого обладает пропускной способностью и стоимостью. Найдите максимальный поток минимальной стоимости из вершины с номером 1 в вершину с номером n.

Входные данные
Первая строка входного файла содержит n и m — количество вершин и количество ребер графа (2≤n≤100, 0≤m≤1000). Следующие m строк содержат по четыре целых числа числа: номера вершин, которые соединяет соответствующее ребро графа, его пропускную способность и его стоимость. Пропускные способности и стоимости не превосходят 105.

Выходные данные
В выходной файл выведите одно число — цену максимального потока минимальной стоимости из вершины с номером 1 в вершину с номером n. Ответ не превышает 263−1. Гарантируется, что в графе нет циклов отрицательной стоимости.
*/
#include <iostream>
#include <vector>
#include <queue>

int_fast64_t INF = 1e9;

struct Edge {
    int_fast64_t from{}, to{};
    int_fast64_t flow{}, capacity{}, cost{};
    int_fast64_t index = 0;
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
    int_fast64_t cum_cost = 0;
    std::vector<int_fast64_t> cost;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<std::pair<int_fast64_t, int_fast64_t>> min_path_nodes;

    // индекс вершины и ребра в ней, с которой можно попасть в данную с наименьшей стоимостью
    bool bellman_ford(int_fast64_t start_node, int_fast64_t end_node);
};

Graph::Graph(int_fast64_t n_nodes_) {
    n_nodes = n_nodes_;
    for (int_fast64_t i = 0; i < n_nodes_; i++) {
        nodes.emplace_back();
        cost.push_back(INF);
        min_path_nodes.emplace_back(-1, -1);
    }
}

void Graph::add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity, int_fast64_t cost_) {
    Edge e1{from, to, 0, capacity, cost_}; // прямое ребро
    Edge e2{to, from, 0, 0, -cost_}; // обратное ребро
    nodes[from].push_back(e1);
    nodes[to].push_back(e2);

    nodes[from].back().index = nodes[from].size() - 1;
    nodes[to].back().index = nodes[to].size() - 1;

    nodes[from].back().rev_node = nodes[to].size() - 1;
    nodes[to].back().rev_node = nodes[from].size() - 1;
}


bool Graph::bellman_ford(int_fast64_t start_node, int_fast64_t end_node) {
    for (int_fast64_t i = 0; i < n_nodes; i++) {
        cost[i] = INF;

        min_path_nodes[i].first = -1;
        min_path_nodes[i].second = -1;
    }

    std::queue<int_fast64_t> queue;
    std::vector<bool> is_in_queue(n_nodes, false); // вспомогательный массив на проверку элемента в очереди

    queue.push(start_node);
    cost[start_node] = 0;

    while (!queue.empty()) {
        int_fast64_t next_node = queue.front();
        queue.pop();
        is_in_queue[next_node] = false;

        for (auto &edge : nodes[next_node]) {
            int_fast64_t sum_cost = cost[edge.from] + edge.cost;
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
    if (cost[end_node] == INF){ // условие выхода из функции*/
        return false;
    }

    // протолкнуть новый поток и прибавить его стоимость

    int_fast64_t max_flow = INF;
    int_fast64_t next_node = end_node;

    while (next_node != start_node) { // найти за линию максимум его пропускной способности
        int_fast64_t ind_node = min_path_nodes[next_node].first;
        int_fast64_t ind_edge = min_path_nodes[next_node].second;

        int_fast64_t flow = nodes[ind_node][ind_edge].capacity - nodes[ind_node][ind_edge].flow;
        max_flow = std::min(flow, max_flow);
        next_node = ind_node;
    }

    next_node = end_node;
    while (next_node != start_node) { // протолкнуть поток
        int_fast64_t ind_node = min_path_nodes[next_node].first;
        int_fast64_t ind_edge = min_path_nodes[next_node].second;

        cum_cost += max_flow * nodes[ind_node][ind_edge].cost;

        // увеличить поток !!!!
        nodes[ind_node][ind_edge].flow += max_flow;
        nodes[next_node][nodes[ind_node][ind_edge].rev_node].flow -= max_flow;
        next_node = ind_node;
    }
    return true;
}


int_fast64_t Graph::get_max_flow(int_fast64_t start_node, int_fast64_t end_node) {
    cum_cost = 0;
    while (bellman_ford(start_node, end_node)){}
    return cum_cost;
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
