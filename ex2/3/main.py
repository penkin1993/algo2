import sys
from collections import defaultdict


def dfs(top, dp_0, dp_1):
    [dfs(child, dp_0, dp_1) for child in graph_structure[top]]

    dp_0[top] += sum([max(dp_0[child], dp_1[child]) for child in graph_structure[top]])
    dp_1[top] += sum([dp_0[child] for child in graph_structure[top]])


n = int(sys.stdin.readline())
graph_structure = defaultdict(list)

for i in range(1, n + 1):
    edge = int(sys.stdin.readline())
    if edge != 0:
        graph_structure[edge].append(i)
    else:
        root = i

dp_0 = defaultdict(lambda: 0)  # если не храним текущую врешину
dp_1 = defaultdict(lambda: 1)  # если храним текущую врешину

dfs(root, dp_0, dp_1)

print(max(dp_0[root], dp_1[root]))
