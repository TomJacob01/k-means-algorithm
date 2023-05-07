from math import dist
import sys
import csv

def kmeans(k : int, iter: int, input_data):
    # parsing input
    with open(input_data, newline='') as csvfile:
        reader = csv.reader(csvfile)
        points = [[float(scalar) for scalar in row] for row in reader]
    number_of_points = len(points)
    if number_of_points == 0: raise ValueError("An Error Has Occurred")

    # handling errors
    if not (k in range(2, number_of_points)):
        raise ValueError("Invalid number of clusters!") 
    if not (iter in range(2, 1000)):
        raise ValueError("Invalid maximum iteration!")

    # call the functions that does the heavy lifting of the algorithm
    centroids = kmeans_sub(k, points, iter)
    for point in centroids:
        s = ""
        for coodinate in range(len(point)-1):
            s += formatting(str(round(point[coodinate],4))) +','
        print(s + formatting(str(round(point[-1],4))))    
    return 
    

def kmeans_sub(k: int, points: list, iter: int):
    # intializing 
    epsilon: float = 0.001
    centroids = [points[_] for _ in range(k)]
    new_centroids = [points[_] for _ in range(k)]
    delta = float('inf')
    dimension: int = len(points[0])

    # iterations
    while iter > 0 and delta >= epsilon:
        number_point_in_cluster = [0 for _ in range(k)]
        new_centroids = [[0.0 for _ in range(dimension)] for i in range(k)]
        delta = 0

        for point in points:
            # find closest centoid to the current point
            index, min_distance = 0, float('inf')
            for i, other_point in enumerate(centroids):
                curr_distance = dist(point, other_point)
                if curr_distance < min_distance:
                    index = i
                    min_distance = curr_distance

            # Adding the point to the right centeroid
            number_point_in_cluster[index] += 1    
            for coordinate in range(dimension):
                new_centroids[index][coordinate] += point[coordinate]
            
        # Update centroids
        for index in range(k):
            if (number_point_in_cluster[index] == 0): continue
            # Avareging the new centoid
            for coordinate in range(dimension):
                new_centroids[index][coordinate] /= number_point_in_cluster[index]

            # comparing the new centoid to the old one
            delta = max(delta, dist(new_centroids[index], centroids[index]))
            centroids[index] = new_centroids[index]

        iter -= 1
    return centroids

# used for the print in the end of the funcition
def formatting(add: str) -> str:
    if add[0] == "-" and len(add) < 7:
        add += "0"*(7-len(add))
    if add[0] != "-" and len(add) < 6:
        add += "0"*(6-len(add))
    return add

if __name__ == "__main__":
    if len(sys.argv) == 4:
        k, iteration_number, file = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]
    elif len(sys.argv) == 3:
        k, iteration_number, file = int(sys.argv[1]), 200, sys.argv[2]
    else:
        raise ValueError("Broken input")

    kmeans(k, iteration_number, file)