/*
C. Улиточки
ограничение по времени на тест2 секунды
ограничение по памяти на тест1024 мегабайта
вводстандартный ввод
выводстандартный вывод
Две улиточки Маша и Петя сейчас находятся в на лужайке с абрикосами и хотят добраться до своего домика. Лужайки пронумерованы числами от 1 до n и соединены дорожками (может быть несколько дорожек соединяющих две лужайки, могут быть дорожки, соединяющие лужайку с собой же). По соображениям гигиены, если по дорожке проползла улиточка, то вторая по той же дорожке уже ползти не может. Помогите Пете и Маше добраться до домика.

Входные данные
В первой строке файла записаны четыре целых числа — n, m, s и t (количество лужаек, количество дорог, номер лужайки с абрикосами и номер домика). В следующих m строках записаны пары чисел. Пара чисел (x, y) означает, что есть дорожка с лужайки x до лужайки y (из-за особенностей улиток и местности дорожки односторонние). Ограничения: .

Выходные данные
Если существует решение, то выведите YES и на двух отдельных строчках сначала последовательность лужаек для Машеньки (дам нужно пропускать вперед), затем путь для Пети. Если решения не существует, выведите NO. Если решений несколько, выведите любое.
*/
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

    ~Graph() = default;

    Graph(Graph &&) = default;

    Graph &operator=(const Graph &) = delete;

    Graph &operator=(Graph &&) = delete;

    void add_edge(int_fast64_t from, int_fast64_t to, int_fast64_t capacity);

    void get_ans(int_fast64_t start_node, int_fast64_t end_node);


private:
    int_fast64_t n_nodes;
    int_fast64_t flow = 0;
    std::vector<int_fast64_t> dist;
    std::vector<std::vector<Edge>> nodes; // все вершины в графе
    std::vector<int_fast64_t> first_node_del;

    std::vector<std::pair<int_fast64_t, int_fast64_t>> edge_buffer;

    bool bfs(int_fast64_t start_node, int_fast64_t end_node);

    void print_ans(int_fast64_t start_node, int_fast64_t end_node);

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
    Edge e2{to, from, 0, 0, 0}; // обратное ребро
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
    return (bool(dist[end_node] != -1));
}

void Graph::print_ans(int_fast64_t start_node, int_fast64_t end_node) {
    if (flow == 2) {
        std::cout << "YES" << "\n";
        for (int j = 0; j < 2; j++) {
            std::cout << start_node + 1 << " ";
            int_fast64_t current_node = start_node;
            while (current_node != end_node) {
                for (auto &node : nodes[current_node]) {
                    if (node.flow == 1) {
                        current_node = node.to;
                        node.flow = 0;
                        std::cout << current_node + 1 << " ";
                        break;
                    }
                }
            }
            std::cout << "\n";
        }

    } else {
        std::cout << "NO";
    }

}

void Graph::get_ans(int_fast64_t start_node, int_fast64_t end_node) {

    int_fast64_t add_flow;
    while (bfs(start_node, end_node)) {
        while (true) {
            add_flow = dfs(start_node, end_node, INF);
            if (add_flow == 0) {
                break;
            }
            flow += add_flow;
            if (flow == 2) {
                print_ans(start_node, end_node);
                return;
            }
        }
    }
    print_ans(start_node, end_node);
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n_nodes, n_edges, s, t;
    std::cin >> n_nodes >> n_edges >> s >> t;
    Graph graph = Graph(n_nodes);
    int_fast64_t from, to;

    for (int_fast64_t i = 0; i < n_edges; i++) {
        std::cin >> from >> to;
        graph.add_edge(from - 1, to - 1, 1);
    }

    graph.get_ans(s - 1, t - 1);

    return 0;
}
