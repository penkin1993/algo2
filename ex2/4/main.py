"""
D. Продавец аквариумов
ограничение по времени на тест1 секунда
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Продавец аквариумов для кошек хочет объехать n городов, посетив каждый из них ровно один раз. Помогите ему найти кратчайший путь.


"""


import sys

from collections import defaultdict

INF = 1000000000000000

n = int(sys.stdin.readline())
node2length = defaultdict(list)

for i in range(n):
    node2length[i] = list(map(int, sys.stdin.readline().split()))
    node2length[i][i] = INF


def get_bit(mask, i):
    return (mask >> i) & 1


def get_ans(node2length, n):
    current_node_mask2length = defaultdict(lambda: INF)
    mask2order = {(i, 0): [i] for i in range(n)}

    for i in range(n):
        current_node_mask2length[(i, 0)] = 0  # node, mask

    for mask in range((1 << n)):  # внешний обход всех масок
        for i in range(n):
            if get_bit(mask, i) == 0:
                for j in range(n):
                    if (j != i) & (get_bit(mask, j) == 0):
                        new_length = node2length[i][j] + current_node_mask2length[(i, mask)]

                        if new_length < current_node_mask2length[(j, mask + (1 << i))]:
                            current_node_mask2length[(j, mask + (1 << i))] = new_length

                            mask2order[(j, mask + (1 << i))] = mask2order[(i, mask)].copy()
                            mask2order[(j, mask + (1 << i))].append(j)

    answer = []
    for i in range(n):
        key = (i, ((1 << n) - 1) - (1 << i))
        answer.append((current_node_mask2length[key], mask2order[key]))

    answer = min(answer, key=lambda x: x[0])
    return answer[0], [str(x + 1) for x in answer[1]]


answer = get_ans(node2length, n)
print(answer[0])
print(" ".join(answer[1]))
