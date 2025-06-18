import matplotlib.pyplot as plt

def read_tour(filename):
    """Read the best tour from the output file."""
    tour = []
    with open(filename, 'r') as file:
        # Skip the header lines
        for line in file:
            if line.strip() == "TOUR_SECTION":
                break
        # Read the tour
        for line in file:
            if line.strip() == "-1":
                break
            tour.append(int(line.strip()) - 1)  # Convert to 0-based index
    return tour

def read_cities(filename):
    """Read city coordinates from the input file."""
    cities = []
    with open(filename, 'r') as file:
        # Skip the header lines
        for line in file:
            if line.strip() == "NODE_COORD_SECTION":
                break
        # Read the cities
        for line in file:
            if line.strip() == "EOF":
                break
            parts = line.strip().split()
            cities.append((float(parts[1]), float(parts[2])))  # (x, y)
    return cities

def plot_tour(cities, tour):
    """Plot the TSP tour."""
    # Extract x and y coordinates of the cities
    x = [city[0] for city in cities]
    y = [city[1] for city in cities]

    # Reorder the cities according to the tour
    tour_x = [x[i] for i in tour]
    tour_y = [y[i] for i in tour]

    # Close the tour by connecting the last city to the first
    tour_x.append(tour_x[0])
    tour_y.append(tour_y[0])

    # Plot the cities and the tour
    plt.figure(figsize=(10, 6))
    plt.scatter(x, y, c='blue', label='Cities')
    plt.plot(tour_x, tour_y, 'r-', label='Tour')
    plt.scatter(tour_x[0], tour_y[0], c='green', label='Start/End')

    # Annotate the start/end point
    plt.annotate('Start/End', (tour_x[0], tour_y[0]), textcoords="offset points", xytext=(10, 10), ha='center')

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('TSP Tour Visualization')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # File paths
    cities_file = "lu980.txt"  # Input file with city coordinates
    tour_file = "best_tour.txt"  # Output file with the best tour

    # Read the cities and the best tour
    cities = read_cities(cities_file)
    tour = read_tour(tour_file)

    # Plot the tour
    plot_tour(cities, tour)
