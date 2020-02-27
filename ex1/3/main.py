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
