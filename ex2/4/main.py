import sys

from collections import defaultdict

INF = 100000000000

n = int(sys.stdin.readline())
node2length = defaultdict(list)

for i in range(n):
    node2length[i] = list(map(int, sys.stdin.readline().split()))
    node2length[i][i] = INF


def get_bit(mask, i):
    return (mask >> i) & 1


def get_ans(node2length, n):
    current_node_mask2length = defaultdict(lambda: INF)
    mask2order = defaultdict(list)

    for i in range(n):
        current_node_mask2length[(i, 0)] = 0  # node, mask
    for mask in range((1 << n)):  # внешний обход всех масок
        for i in range(n):
            if get_bit(mask, i) == 0:  # вершина i не посящалась
                for j in range(n):
                    if (j != i) & (get_bit(mask, j) == 0):  # вершина j ранее не посящалась
                        new_length = node2length[i][j] + current_node_mask2length[(i, mask)]

                        if new_length < current_node_mask2length[(j, mask + (1 << i))]:
                            current_node_mask2length[(j, mask + (1 << i))] = new_length

                            mask2order[mask + (1 << i)] = mask2order[mask].copy()
                            mask2order[mask + (1 << i)].append(i)

    min_pair = [(current_node_mask2length[(i, ((1 << n) - 1) - (1 << i))], i) for i in range(n)]
    min_val, min_path = min(min_pair, key=lambda x: x[0])
    min_path = mask2order[(((1 << n) - 1) - (1 << min_path))] + [min_path]
    min_path = [x + 1 for x in min_path]

    return min_val, min_path


print(get_ans(node2length, n))

"""
5
0 183 163 173 181
183 0 165 172 171
163 165 0 189 302
173 172 189 0 167
181 171 302 167 0
"""
