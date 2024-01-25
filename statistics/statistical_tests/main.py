import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats


def calculate_statistical_values(data):
    average = np.mean(data)
    median = np.median(data)
    mod = stats.mode(data, keepdims=False)[0]
    range_value = np.ptp(data)

    variance = np.var(data)
    standard_deviation = np.std(data)

    # Upper and lower quantile
    q_3 = np.percentile(data, 75)
    q_1 = np.percentile(data, 25)
    iqr = q_3 - q_1  # Interquartile Range

    variance_estimator = np.var(data, ddof=1)

    return [
        average,
        median,
        mod,
        range_value,
        variance,
        standard_deviation,
        q_3,
        q_1,
        iqr,
        variance_estimator,
    ]


if __name__ == "__main__":
    data_csv = pd.read_csv("data/5.csv")
    pd.set_option("display.max_rows", None)  # Display all rows

    first_population = data_csv.iloc[:, 0]
    second_population = data_csv.iloc[:, 1]

    # Display histogram
    plt.hist(first_population, alpha=0.5, label="First population")
    plt.hist(second_population, alpha=0.5, label="Second population")

    plt.xlabel("Values")
    plt.ylabel("Frequency")
    plt.title("Histograms for two samples from two different populations")
    plt.legend()
    plt.show()

    # Calculate statistical values
    result1 = calculate_statistical_values(first_population)
    result2 = calculate_statistical_values(second_population)

    variance1 = result1[4]
    variance2 = result2[4]

    variance_estimator1 = result1[9]
    variance_estimator2 = result2[9]

    average1 = result1[0]
    average2 = result2[0]

    row_headers = [
        "Average",
        "Median",
        "Mode",
        "Range",
        "Variance",
        "Standard deviation",
        "Upper percentile",
        "Lower percentile",
        "IQR",
        "Variance estimator",
    ]

    table1 = list(zip(row_headers, result1))
    table2 = list(zip(row_headers, result2))

    print(
        pd.DataFrame(table1, columns=["Population 1", "Value"]).to_string(index=False)
    )
    print("=======================================")
    print(
        pd.DataFrame(table2, columns=["Population 2", "Value"]).to_string(index=False)
    )
    print("=======================================")

    # Variables for tests
    x = 5
    alpha = 0.05
    # Degree of freedom
    df = 1000
    df1 = df - 1
    df2 = 2 * df1 - 2
    s_p = np.floor(
        ((variance1 / df) ** 2) / (((variance1 / df) ** 2) / df1)
        + ((variance1 / df) ** 2) / df1
    )

    decisions = []

    # Tests:
    # 1 H0: µ1 = 5, H1: µ1 != 5
    # 2 H0: µ2 = 5, H1: µ2 != 5
    # 3 H0: µ1 - µ2 = 0, H1: µ1 - µ2 != 5
    for u1, u2 in zip(first_population, second_population):
        test_statistic1 = (average1 - u1) / variance_estimator1 * np.sqrt(df)
        test_statistic2 = (average2 - u2) / variance_estimator2 * np.sqrt(df)
        test_statistic3 = ((average1 - average2) - (u1 - u2)) / (
            s_p * np.sqrt((1 / df) + (1 / df))
        )

        t_student1 = stats.t.cdf(np.abs(test_statistic1), df=df1)
        t_student2 = stats.t.cdf(np.abs(test_statistic2), df=df1)
        t_student3 = stats.t.cdf(np.abs(test_statistic3), df=df2)

        p_value1 = 2 * (1 - t_student1)
        p_value2 = 2 * (1 - t_student2)
        p_value3 = 2 * (1 - t_student3)

        decision1 = "Rejected" if p_value1 < alpha else "Accepted"
        decision2 = "Rejected" if p_value2 < alpha else "Accepted"
        decision3 = "Rejected" if p_value3 < alpha else "Accepted"

        decisions.append([decision1, decision2, decision3])

    print(pd.DataFrame(decisions, columns=["Test 1", "Test 2", "Test 3"]))
