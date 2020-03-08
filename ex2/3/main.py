import sys
from collections import defaultdict


def dfs(top, dp_0, dp_1):
    [dfs(child, dp_0, dp_1) for child in graph_structure[top]]

    dp_0[top] = 0 + sum([dp_0[child] for child in graph_structure[top]])
    dp_1[top] = 1 + sum([dp_1[child] for child in graph_structure[top]])

    return max(dp_0[0], dp_1[0])


n = int(sys.stdin.readline())
graph_structure = defaultdict(list)

sys.stdin.readline()  # пропуск нуля

for i in range(n - 1):
    edge = int(sys.stdin.readline())
    graph_structure[edge - 1].append(i + 1)

print(graph_structure)

dp_0 = defaultdict(lambda: 0)  # если не храним текущую врешину
dp_1 = defaultdict(lambda: 1)  # если храним текущую врешину

print(dfs(0, dp_0, dp_1))

print(dp_0)
print(dp_1)

"""
5
0
1
1
2
3

"""
