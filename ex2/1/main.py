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

