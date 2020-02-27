import sys


def get_metric(s1: str, s2: str):
    if not s1:
        return len(s2)

    if not s2:
        return len(s1)

    eq_operation = get_metric(s1[1:], s2[1:]) + int(not(s1[0] == s2[0]))
    sub_operation_1 = get_metric(s1[1:], s2) + 1
    sub_operation_2 = get_metric(s1, s2[1:]) + 1

    return min(eq_operation, sub_operation_1, sub_operation_2)


s1 = sys.stdin.readline()[:-1]
s2 = sys.stdin.readline()[:-1]

sys.stdout.write(str(get_metric(s1, s2)))
