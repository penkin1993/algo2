"""
A. Кузнечик собирает монеты
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Кузнечик прыгает по столбикам, расположенным на одной линии на равных расстояниях друг от друга. Столбики имеют порядковые номера от 1 до N. В начале Кузнечик сидит на столбике с номером 1 и хочет добраться до столбика с номером N. Он может прыгнуть вперед на расстояние от 1 до K столбиков, считая от текущего.

На каждом столбике Кузнечик может получить или потерять несколько золотых монет (для каждого столбика это число известно). Определите, как нужно прыгать Кузнечику, чтобы собрать наибольшее количество золотых монет. Учитывайте, что Кузнечик не может прыгать назад.

Входные данные
В первой строке вводятся два натуральных числа: N и K (2 ≤ N, K ≤ 10000), разделённые пробелом. Во второй строке записаны через пробел N - 2 целых числа – количество монет, которое Кузнечик получает на каждом столбике, от 2-го до N - 1-го. Если это число отрицательное, Кузнечик теряет монеты. Гарантируется, что все числа по модулю не превосходят 10 000.

Выходные данные
В первой строке программа должна вывести наибольшее количество монет, которое может собрать Кузнечик. Во второй строке выводится число прыжков Кузнечика, а в третьей строке – номера всех столбиков, которые посетил Кузнечик (через пробел в порядке возрастания).

Если правильных ответов несколько, выведите любой из них.
"""


def get_path_score(score):
    path_score = [0] * n
    score.append(0)

    for i in range(1, n):
        left_ind = max(0, i - k)
        path_score[i] = score[i - 1] + max(path_score[left_ind:i])
    return path_score


def get_reverse_steps(path_score, k):
    ind = len(path_score)
    path = [str(ind)]

    while ind > 1:
        left_ind = max(0, ind - k - 1)
        right_ind = max(0, ind - 1)

        loc_path = path_score[left_ind:right_ind]
        loc_path = loc_path[::-1]

        step = -max((x, -i) for i, x in enumerate(loc_path))[1]
        ind -= (step + 1)

        path.append(str(ind))

    return path[::-1]


with open("input.txt", "r") as f:
    n, k = [int(i) for i in f.readline().split()]
    score = [int(i) for i in f.readline().split()]

path_score = get_path_score(score)
path = get_reverse_steps(path_score, k)

with open("output.txt", "w") as f:
    f.write(str(path_score[-1]) + "\n")
    f.write(str(len(path) - 1) + "\n")
    f.write(" ".join(path))

