"""
D. Расстояние по Левенштейну
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дана текстовая строка. С ней можно выполнять следующие операции:

1. Заменить один символ строки на другой символ.

2. Удалить один произвольный символ.

3. Вставить произвольный символ в произвольное место строки.

Например, при помощи первой операции из строки «СОК» можно получить строку «СУК», при помощи второй операции — строку «ОК», при помощи третьей операции — строку «СТОК».

Минимальное количество таких операций, при помощи которых можно из одной строки получить другую, называется стоимостью редактирования или расстоянием Левенштейна.

Определите расстояние Левенштейна для двух данных строк.

Входные данные
Программа получает на вход две строки, длина каждой из которых не превосходит 1000 символов, строки состоят только из заглавных латинских букв.

Выходные данные
Требуется вывести одно число — расстояние Левенштейна для данных строк.
"""

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
