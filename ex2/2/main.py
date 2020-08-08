"""
B. Удаление скобок 2.0
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дана строка, составленная из круглых, квадратных и фигурных скобок. Определите, какое наименьшее количество символов необходимо удалить из этой строки, чтобы оставшиеся символы образовывали правильную скобочную последовательность.
"""

import sys

from collections import defaultdict

INF = 10000000000000


def get_right_str(input_str, split_ind, key):
    if key[1] - key[0] <= 0:
        return ""
    elif split_ind[key] == -1:
        return input_str[key[0]] + get_right_str(input_str, split_ind, (key[0] + 1, key[1] - 1)) + input_str[key[1]]
    else:
        return (get_right_str(input_str, split_ind, (key[0], split_ind[key])) +
                get_right_str(input_str, split_ind, (split_ind[key] + 1, key[1])))


def get_splits(input_str):
    n = len(input_str)
    split_ind = defaultdict(lambda: -1)
    split_loss = defaultdict(lambda: INF)
    for i in range(n):
        split_ind[(i, i)] = i
        split_loss[(i, i)] = 1
        split_loss[(i, i - 1)] = 0

    for length in range(1, n):
        for i in range(0, n - length):
            j = i + length
            if ((input_str[i] == "[") & (input_str[j] == "]") |
                    (input_str[i] == "(") & (input_str[j] == ")") |
                    (input_str[i] == "{") & (input_str[j] == "}")):
                split_loss[(i, j)] = split_loss[(i + 1, j - 1)]

            for k in range(i, j):
                c_loss = split_loss[(i, k)] + split_loss[(k + 1, j)]
                if c_loss < split_loss[(i, j)]:
                    split_loss[(i, j)] = c_loss
                    split_ind[(i, j)] = k

    return split_ind


input_str = list(sys.stdin.readline()[:-1])

split_ind = get_splits(input_str)

key = (0, len(input_str) - 1)
print(get_right_str(input_str, split_ind, key))
