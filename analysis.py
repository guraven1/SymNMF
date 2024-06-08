import sys
import numpy as np
from sklearn.metrics import silhouette_score
import symnmf

def read_data_points(file_name):
    with open(file_name, "r") as file:
        data_points = []
        for line in file:
            data_point = [float(x) for x in line.strip().split(",")]
            data_points.append(data_point)
    return np.array(data_points)

#def kmeans_plus_plus(data_points, k):
    # Implement Kmeans++ method (from HW2 or any other implementation)

def assign_clusters_symnmf(H):
    return np.argmax(H, axis=1)

def main():
    if len(sys.argv) != 2:
        print("An Error Has Occurred")
        sys.exit(1)

    file_name = sys.argv[1]
    data_points = read_data_points(file_name)

    # Applying SymNMF
    H = symnmf.symnmf(data_points)
    symnmf_clusters = assign_clusters_symnmf(H)

    # Applying Kmeans++
    kmeans_clusters = kmeans_plus_plus(data_points, k)

    # Calculate silhouette scores
    symnmf_score = silhouette_score(data_points, symnmf_clusters)
    kmeans_score = silhouette_score(data_points, kmeans_clusters)

    print(f"nmf: {symnmf_score:.4f}")
    print(f"kmeans: {kmeans_score:.4f}")

if __name__ == "__main__":
    main()
