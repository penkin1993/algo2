import sys


def get_ans(number, all_del):
    ans = []
    for i in all_del:
        if all_del[i]:
            while number % i == 0:
                number = int(number / i)
                ans.append(str(i))

            for j in range(i, number, i):
                all_del[j] = False
    return ans


n = int(sys.stdin.readline())
all_del = {i: True for i in range(2, 300000)}

for _ in range(n):
    inp_num = int(sys.stdin.readline())
    ans = get_ans(inp_num, all_del)

    if len(ans) != 0:
        print(" ".join(ans))
    else:
        print(str(inp_num))


"""
4
60
14
3
55
"""


# TODO: 1. Второй массив с наименьшим общим делителем для каждого числа

# TODO: 2. Переписать на с++

