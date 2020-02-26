import sys


def get_answer_matrix(matrix):
    n = len(matrix)
    m = len(matrix[0])

    answer = list()
    for _ in range(n + 1):
        answer.append([-100000000]*(m + 1))

    answer[1][1] = matrix[0][0]

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if (i != 1) or (j != 1):
                answer[i][j] = max(answer[i - 1][j], answer[i][j - 1]) + matrix[i - 1][j - 1]

    return answer


def get_path(answer):
    path = ""
    n = len(answer) - 1
    m = len(answer[0]) - 1
    while (n != 1) | (m != 1):
        if answer[n - 1][m] >= answer[n][m - 1]:
            path += "D"
            n -= 1
        else:
            path += "R"
            m -= 1

    return path[::-1]


n, m = [int(i) for i in sys.stdin.readline().split()]

matrix = list()
for _ in range(n):
    matrix.append([int(i) for i in sys.stdin.readline().split()])


answer = get_answer_matrix(matrix)
path = get_path(answer)

sys.stdout.write(str(answer[-1][-1]) + '\n')
sys.stdout.write(path)



