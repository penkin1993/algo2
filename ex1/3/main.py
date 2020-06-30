"""
C. Наибольшая возрастающая подпоследовательность
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Пусть a 1, a 2, ..., a n — числовая последовательность. Длина последовательности — это количество элементов этой последовательности. Последовательность a i 1, a i 2, ..., a i k называется подпоследовательностью последовательности a, если 1 ≤ i 1 < i 2 < ... < i k ≤ n. Последовательность a называется возрастающей, если a 1 < a 2 < ... < a n.

Вам дана последовательность, содержащая n целых чисел. Найдите ее самую длинную возрастающую подпоследовательность.

Входные данные
В первой строке задано одно число n (1 ≤ n ≤ 2000) — длина подпоследовательности. В следующей строке задано n целых чисел a i ( - 109 ≤ a i ≤ 109) — элементы последовательности.

Выходные данные
В первой строке выведите число k — длину наибольшей возрастающей подпоследовательности. В следующей строке выведите k чисел — саму подпоследовательность.
"""

import sys


n = int(sys.stdin.readline())
sequence = [int(i) for i in sys.stdin.readline().split()]


def get_answer(sequence):
    answer = [(sequence[0], )]

    for i in range(1, n):
        max_seq_len, max_seq_ind = -1, -1

        for j in range(i):
            if (sequence[i] > sequence[j]) & (max_seq_len < len(answer[j])):
                max_seq_len, max_seq_ind = len(answer[j]), j

        if max_seq_ind == -1:
            answer.append((sequence[i], ))
        else:
            answer.append(answer[max_seq_ind] + (sequence[i], ))

    max_ind = -max((len(x), -i) for i, x in enumerate(answer))[1]

    sys.stdout.write(str(len(answer[max_ind])) + '\n')
    for i in answer[max_ind]:
        sys.stdout.write(str(i) + " ")


get_answer(sequence)
