import numpy as np

from uniform import rand_one_number


def generate_sequences(
    distribution: callable,
    start: float,
    stop: float,
    upper_bound: float,
    number_of_sample,
):
    output = list()
    x = np.pi - np.floor(np.pi)
    y = np.e - np.floor(np.e)
    i = 0

    while len(output) <= number_of_sample:
        i += 1
        x = rand_one_number(seed=x, saw_teeth=7)
        y = rand_one_number(seed=y, saw_teeth=7)
        if distribution(x * (stop - start) + start) > upper_bound * y:
            output.append(x * (stop - start) + start)

    print(f"Accept level: {number_of_sample / i * 100:.2f} %")

    return np.array(output)
