import sys
import math


def get_ans(a, b, n):
    if a == 0:
        if b == 0:
            return -1
        elif b == 1:
            return 0
        else:
            return -1

    k = math.ceil(n ** 0.5)
    # k = round(n**0.5)

    r_ans = s_ans = -1

    s1 = dict()
    for r in range(1, k + 1):
        res = pow(a, r * k, n)
        if res not in s1:
            s1[res] = r

    for s in range(1, k + 1):
        res = (b * pow(a, s, n)) % n
        if res in s1:
            r_ans = s1[res]
            s_ans = s
            break

    if r_ans == -1:
        return -1

    # print(r_ans, k, s_ans)

    return r_ans * k - s_ans


a, b, n = [int(i) for i in sys.stdin.readline().split()]
print(get_ans(a, b, n))

# 2 4 7
# 41 145 239
# 7 121 122


