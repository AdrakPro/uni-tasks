import numpy as np


def rand_one_number(seed: float, saw_teeth: int = 7):
    y = saw_teeth * seed
    return y - np.floor(y)


def uniform(seed: float, saw_teeth: int = 7, number_of_samples: int = 1):
    y = np.zeros(number_of_samples)
    y[0] = seed
    for index, seed in enumerate(y[1:]):
        y[index + 1] = rand_one_number(y[index], saw_teeth)
    return y

