"""
A. Разбиение на пары
ограничение по времени на тест2 секунды
ограничение по памяти на тест64 мегабайта
вводстандартный ввод
выводстандартный вывод
Однажды n ребят решили поучаствовать в VK Cup. Для этого им нужно разбиться на команды по два человека. Однако выяснилось, что не все ребята хорошо ладят друг с другом. Вы выяснили для каждой пары ребят, могут ли они участвовать в одной команде. Помогите им сформировать максимальное число команд.
"""

import sys

from collections import defaultdict


def get_bit(mask, i):
    return (mask >> i) & 1


def get_ans(graph_structure, n):
    dp = {0: 0}
    for mask in range(1, (1 << n)):
        for i in range(n):  # обход графа
            if get_bit(mask, i) == 1:
                dp[mask] = dp[mask - (1 << i)]
                for j in graph_structure[i]:
                    if get_bit(mask, j) == 1:
                        dp[mask] = max(dp[mask], dp[mask - (1 << i) - (1 << j)] + 1)
    return 2 * dp[(1 << n) - 1]


n = int(sys.stdin.readline())
graph_structure = defaultdict(list)

for edge in range(n):
    sequence = list(sys.stdin.readline())
    [graph_structure[edge].append(i) for i, item in enumerate(sequence) if item == "Y"]

print(get_ans(graph_structure, n))

