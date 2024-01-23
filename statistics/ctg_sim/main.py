import matplotlib.pyplot as plt
import numpy as np

from random_generator import generate_sequences


# CTG (Centralne Twierdzenie Graniczne) mówi o tym, że rozkład zbioru
# zmiennych losowych o tym samym rozkładzie (niekoniecznie normalnym) zbiega do
# rozkładu normalnego (przy n dążącym do nieskończoności).

def plot_histogram(data, n, color='blue'):
    plt.figure()
    plt.hist(data, bins=30, density=True, color=color, edgecolor='black')

    # Average and standard deviation of data
    mean_data, std_data = np.mean(data), np.std(data)
    normal_x = np.linspace(min(data), max(data), 100)
    normal_distribution = ((1 / (std_data * np.sqrt(2 * np.pi))) *
                           np.exp(-(normal_x - mean_data) ** 2 / (2 * std_data ** 2)))

    plt.plot(normal_x, normal_distribution, color='red', linewidth=2,
             label='Normal Distribution')
    plt.title(f'Histogram for n={n}')
    plt.legend()
    plt.show()


def f(x):
    return (x + 1) * (-1 < x < 0) + (-x + 1) * (0 <= x < 1)


if __name__ == '__main__':
    y = generate_sequences(distribution=f,
                           start=-1,
                           stop=1,
                           upper_bound=1,
                           number_of_sample=100_000)

    total_samples = 100_000
    sub_sequences = 10_000
    n_values = [1, 2, 5, 10]

    for n in n_values:
        # Generate a sequence of random variables
        y_sequence = generate_sequences(distribution=f, start=-1, stop=1, upper_bound=1,
                                        number_of_sample=total_samples)

        # Divide the sequence into m equal subsequences of length n. Example:
        # data = [1,2,3,4,5,6] the subsequence for n=2 is [data[1],data[1+2],data[3+2],...],
        y_subsequences = y_sequence[:sub_sequences * n].reshape(sub_sequences, n)

        # Calculate the average for each subsequence
        y_averages = np.mean(y_subsequences, axis=1)

        plot_histogram(y_averages, n, color='blue')
