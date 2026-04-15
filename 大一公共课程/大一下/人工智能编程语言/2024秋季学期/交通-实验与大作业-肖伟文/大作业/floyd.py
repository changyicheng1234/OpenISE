import pandas as pd
import numpy as np
from matplotlib.animation import FuncAnimation
import networkx as nx

# Load the edge data
filename = "edge.csv"  # The CSV file containing the edge data for the graph
edge_input = pd.read_csv(filename)  # Read the CSV file into a pandas DataFrame
edge_input = edge_input.iloc[:, 1:]  # Remove the first column (if it's an index or irrelevant)
print("Edge Input:")  # Print the loaded edge data for verification
print(edge_input)

# Initialize the graph
n = len(edge_input)  # Number of nodes in the graph (assumes square adjacency matrix)
INF = 1e9  # Representation of infinity (a very large number)
edge = np.zeros((n, n))  # Initialize an n x n adjacency matrix with zeros

# Fill the adjacency matrix
for i in range(n):  # Iterate over rows
    for j in range(n):  # Iterate over columns
        value = edge_input.iloc[i, j]  # Get the value from the DataFrame
        if value.isnumeric():  # Check if the value is numeric (valid edge weight)
            edge[i][j] = int(value)  # Convert to integer and store in the adjacency matrix
        else:
            edge[i][j] = INF  # If not numeric, treat it as no edge (infinity)
print("Adjacency Matrix:")  # Print the adjacency matrix for verification
print(edge)

def floyd_warshall(edge, n, INF=1e9):
    """
    Floyd-Warshall algorithm to compute shortest paths between all pairs of nodes.

    Parameters:
    - edge: 2D numpy array representing the adjacency matrix of the graph.
    - n: Number of nodes in the graph.
    - INF: Representation of infinity (default is 1e9).

    Returns:
    - dist: 2D numpy array representing the shortest path matrix.
    """
    dist = edge.copy()  # Initialize the distance matrix with the adjacency matrix

    # Perform the Floyd-Warshall algorithm
    for k in range(n):  # Iterate over all intermediate nodes
        for i in range(n):  # Iterate over all source nodes
            for j in range(n):  # Iterate over all destination nodes
                # Update the shortest distance
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    return dist

# Call the function and output the shortest path matrix
shortest_paths = floyd_warshall(edge, n, INF)
print("Shortest Path Matrix:")
for row in shortest_paths:
    print(" ".join(f"{int(x) if x < INF else '-1':>5}" for x in row))