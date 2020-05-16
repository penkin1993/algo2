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

