/*
D. Автоматное программирование
ограничение по времени на тест5 секунд
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
В один замечательный день в компанию «X» завезли k автоматов. И не простых автоматов, а автоматов-программистов! Это был последний неудачный шаг перед переходом на андроидов-программистов, но это уже совсем другая история.

В компании сейчас n задач, для каждой из которых известно время начала ее выполнения si, длительность ее выполнения ti и прибыль компании от ее завершения ci. Любой автомат может выполнять любую задачу, ровно одну в один момент времени. Если автомат начал выполнять задачу, то он занят все моменты времени с si по si+ti−1 включительно и не может переключиться на другую задачу.

Вам требуется выбрать набор задач, которые можно выполнить с помощью этих k автоматов и который принесет максимальную суммарную прибыль.

Входные данные
В первой строке записаны два целых числа n и k (1≤n≤1000, 1≤k≤50) — количество задач и количество автоматов, соответственно.

В следующих n строках через пробелы записаны тройки целых чисел si,ti,ci (1≤si,ti≤109, 1≤ci≤106), si — время начала выполнения i-го задания, ti — длительность i-го задания, а ci — прибыль от его выполнения.

Выходные данные
Выведите n целых чисел x1,x2,…,xn. Число xi должно быть равно 1, если задачу i следует выполнить, и 0 в противном случае.

Если оптимальных решений несколько, то выведите любое из них.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

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

    int_fast32_t get_max_flow(int_fast32_t start_node, int_fast32_t end_node, int_fast32_t k, int_fast32_t n,
                              std::vector<std::pair<int_fast32_t, bool>> job_index);


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

                if (!is_in_queue[edge.to]) {
                    queue.push(edge.to);// засунуть в очередь
                    is_in_queue[edge.to] = true;
                }
            }
        }
    }
    if (cost[end_node] == INF) { // условие выхода из функции
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


int_fast32_t Graph::get_max_flow(int_fast32_t start_node, int_fast32_t end_node,
                                 int_fast32_t k, int_fast32_t n, std::vector<std::pair<int_fast32_t, bool>> job_index) {
    cum_cost = 0;
    while (bellman_ford(start_node, end_node)) {}

    for (int i = k + 1; i < k + n + 1; i++) {
        for (auto &node : nodes[i]) {
            if (node.flow != 0) {
                job_index[i - k - 1].second = true;
            }
        }
    }

    sort(job_index.begin(), job_index.end());
    for (auto &i : job_index) {
        std::cout << i.second << " ";
    }

    return cum_cost;
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast32_t n, k;
    std::cin >> n >> k;
    Graph graph = Graph(k + 2 * n + 2);


    int_fast32_t start_t, duration_t, cost;
    std::vector<std::tuple<int_fast32_t, int_fast32_t, int_fast32_t, int_fast32_t>> job;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> start_t >> duration_t >> cost;
        job.emplace_back(start_t, start_t + duration_t, -cost, i);
    }

    sort(job.begin(), job.end());

    std::vector<std::pair<int_fast32_t, bool>> job_index; // индекс работ
    for (int i = 0; i < n; i++) {
        job_index.emplace_back(std::get<3>(job[i]), false);
    }

    // Путь из стока к работникам
    for (int_fast32_t i = 1; i < k + 1; i++) {
        graph.add_edge(0, i, 1, 0);
    }

    // соединение каждого работника с фейковой работой
    for (int_fast32_t i = 1; i < k + 1; i++) { // цикл по работникам
        for (int_fast32_t j = 0; j < n; j++) { // цикл по фейковым работам
            graph.add_edge(i, k + 1 + j, 1, 0);
        }
    }

    for (int_fast32_t i = 0; i < n; i++) { // соединение работы со стоком
        graph.add_edge(n + k + 1 + i, k + 2 * n + 1, 1, 0);
    }

    // соединение между фейковым работами и работами
    for (int_fast32_t i = 0; i < n; i++) {
        graph.add_edge(k + 1 + i, k + n + 1 + i, 1, std::get<2>(job[i]));
    }

    // переходы между работами
    for (int_fast32_t i = 0; i < n; i++) {
        for (int_fast32_t j = i + 1; j < n; j++) {
            if (std::get<1>(job[i]) <= std::get<0>(job[j])) { // время заверешнеия предыдущей не меньше начала следующей
                graph.add_edge(k + n + 1 + i, k + 1 + j, 1, 0);
            }
        }
    }


    graph.get_max_flow(0, k + 2 * n + 1, k, n, job_index);

    return 0;
}

