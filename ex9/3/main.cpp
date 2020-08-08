/*
C. План эвакуации
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
В городе есть муниципальные здания и бомбоубежища, которые были специально построены для эвакуации служащих в случае ядерной войны. Каждое бомбоубежище имеет ограниченную вместительность по количеству людей, которые могут в нем находиться. В идеале все работники из одного муниципального здания должны были бы бежать к ближайшему бомбоубежищу. Однако, в таком случае, некоторые бомбоубежища могли бы переполниться, в то время как остальные остались бы наполовину пустыми.

Чтобы разрешить эту проблему Городской Совет разработал специальный план эвакуации. Вместо того, чтобы каждому служащему индивидуально приписать, в какое бомбоубежище он должен бежать, для каждого муниципального здания определили, сколько служащих из него в какое бомбоубежище должны бежать. Задача индивидуального распределения была переложена на внутреннее управление муниципальных зданий.

План эвакуации учитывает количество служащих в каждом здании — каждый служащий должен быть учтен в плане и в каждое бомбоубежище может быть направлено количество служащих, не превосходящее вместимости бомбоубежища.

Городской Совет заявляет, что их план эвакуации оптимален в том смысле, что суммарное время эвакуации всех служащих города минимально.

Мэр города, находящийся в постоянной конфронтации с Городским Советом, не слишком то верит этому заявлению. Поэтому он нанял Вас в качестве независимого эксперта для проверки плана эвакуации. Ваша задача состоит в том, чтобы либо убедиться в оптимальности плана Городского Совета, либо доказать обратное, представив в качестве доказательства другой план эвакуации с меньшим суммарным временем для эвакуации всех служащих.

Карта города может быть представлена в виде квадратной сетки. Расположение муниципальных зданий и бомбоубежищ задается парой целых чисел, а время эвакуации из муниципального здания с координатами (X i, Y i) в бомбоубежище с координатами (P j, Q j) составляет D ij = |X i - P j| + |Y i - Q j| + 1 минут.

Входные данные
Входной файл содержит описание карты города и плана эвакуации, предложенного Городским Советом. Первая строка входного файла содержит два целых числа N (1 ≤ N ≤ 100) и M (1 ≤ M ≤ 100), разделенных пробелом. N — число муниципальных зданий в городе (все они занумерованы числами от 1 до N), M — число бомбоубежищ (все они занумерованы числами от 1 до M).

Последующие N строк содержат описания муниципальных зданий. Каждая строка содержит целые числа X i, Y i и B i, разделенные пробелами, где X i, Y i ( - 1000 ≤ X i, Y i ≤ 1000) — координаты здания, а B i (1 ≤ B i ≤ 1000) — число служащих в здании.

Описание бомбоубежищ содержится в последующих M строках. Каждая строка содержит целые числа P j, Q j и C j, разделенные пробелами, где P j, Q j ( - 1000 ≤ P j, Q j ≤ 1000) — координаты бомбоубежища, а C j (1 ≤ C j ≤ 1000) — вместимость бомбоубежища.

В последующих N строках содержится описание плана эвакуации. Каждая строка представляет собой описание плана эвакуации для отдельного здания. План эвакуации из i-го здания состоит из M целых чисел E ij, разделенных пробелами. E ij (0 ≤ E ij ≤ 10000) — количество служащих, которые должны эвакуироваться из i-го здания в j-е бомбоубежище.

Гарантируется, что план, заданный во входном файле, корректен.

Выходные данные
Если план эвакуации Городского Совета оптимален, то выведите одно слово OPTIMAL. В противном случае выведите на первой строке слово SUBOPTIMAL, а в последующих N строках выведите Ваш план эвакуации (более оптимальный) в том же формате, что и во входном файле. Ваш план не обязан быть оптимальным, но должен быть лучше плана Городского Совета.
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

    void get_ans(int_fast32_t start_node, int_fast32_t end_node, int_fast32_t global_cost, int_fast32_t n);


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


void Graph::get_ans(int_fast32_t start_node, int_fast32_t end_node, int_fast32_t global_cost,  int_fast32_t n) {
    cum_cost = 0;
    while (bellman_ford(start_node, end_node)) {}

    if (cum_cost == global_cost) {
        std::cout << "OPTIMAL";
    } else {
        std::cout << "SUBOPTIMAL\n";
        for (int_fast32_t i = 1; i < n + 1; i++) {
            for (auto &edge : nodes[i]) {
                if (edge.capacity > 0) {
                    std::cout << edge.flow << " ";
                }
            }
            std::cout << "\n";
        }
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
        graph.add_edge(0, i + 1, std::get<2>(buildings[i]), 0);
    }

    int_fast32_t global_cost = 0;
    int_fast32_t local_cost = 0;
    int_fast32_t flow;
    for (int_fast32_t i = 0; i < n; i++) {
        for (int_fast32_t j = 0; j < m; j++) {
            std::cin >> flow;

            local_cost = 1 + std::abs(std::get<0>(buildings[i]) - std::get<0>(shelters[j])) +
                         std::abs(std::get<1>(buildings[i]) - std::get<1>(shelters[j]));
            global_cost += local_cost * flow;
            graph.add_edge(i + 1, j + n + 1, std::get<2>(buildings[i]), local_cost);
        }
    }

    for (int_fast32_t i = 0; i < m; i++) {
        graph.add_edge(n + i + 1, n + m + 1, std::get<2>(shelters[i]), 0);
    }

    graph.get_ans(0, n + m + 1, global_cost, n);

    return 0;
}
