import sys


def get_metric(s1: str, s2: str):
    n = len(s1) + 1
    m = len(s2) + 1

    answer_matrix = []
    for _ in range(n):
        answer_matrix.append(m * [0])

    for i in range(n):
        for j in range(m):

            if i == 0:
                answer_matrix[i][j] = j

            elif j == 0:
                answer_matrix[i][j] = i

            elif s1[i - 1] == s2[j - 1]:
                answer_matrix[i][j] = answer_matrix[i - 1][j - 1]

            else:
                answer_matrix[i][j] = 1 + min(answer_matrix[i - 1][j - 1],
                                              answer_matrix[i - 1][j],
                                              answer_matrix[i][j - 1])

    # print(answer_matrix)

    return answer_matrix[-1][-1]


s1 = sys.stdin.readline()[:-1]
s2 = sys.stdin.readline()[:-1]

sys.stdout.write(str(get_metric(s1, s2)))
