import pandas as pd
import numpy as np
from matplotlib.animation import FuncAnimation
import networkx as nx
import os

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

# Dijkstra's Algorithm
def dijkstra(graph, src):
    """
    Implementation of Dijkstra's algorithm to find the shortest paths
    from a source node to all other nodes in a weighted graph.

    Parameters:
    - graph: 2D list or numpy array representing the adjacency matrix of the graph
    - src: The source node (0-indexed)

    Returns:
    - dist: List of shortest distances from the source node to each node
    """
    n = len(graph)  # Number of nodes in the graph
    dist = [INF] * n  # Initialize distances to all nodes as infinity
    dist[src] = 0  # Distance to the source node itself is 0
    visited = [False] * n  # Track whether each node has been visited

    for _ in range(n):  # Iterate n times (once for each node)
        # Find the unvisited node with the smallest distance
        min_dist = INF  # Initialize the minimum distance as infinity
        u = -1  # Initialize the node index
        for i in range(n):  # Iterate over all nodes
            if not visited[i] and dist[i] < min_dist:  # Check if node is unvisited and has a smaller distance
                min_dist = dist[i]  # Update the minimum distance
                u = i  # Update the node index

        if u == -1:  # If no unvisited node is reachable, break the loop
            break

        # Mark the node as visited
        visited[u] = True

        # Update distances to neighboring nodes
        for v in range(n):  # Iterate over all nodes
            if not visited[v] and graph[u][v] != INF:  # Check if the neighbor is unvisited and has an edge
                # Update the distance to the neighbor if a shorter path is found
                dist[v] = min(dist[v], dist[u] + graph[u][v])

    # Replace distances that are still infinity with -1 (indicating unreachable nodes)
    dist = [x if x < INF else -1 for x in dist]
    return dist

# Task 1: Run Dijkstra's algorithm from a source node (e.g., node 0)

source_node = 0  # Define the source node (0-indexed)
shortest_distances = dijkstra(edge, source_node)  # Compute shortest distances
print(f"Shortest distances from node {source_node}:")  # Print the results
print(shortest_distances)

# Task 2: Run Dijkstra's algorithm from a different source node (e.g., node 1)

source_node = 1  # Define the source node (0-indexed)
shortest_distances = dijkstra(edge, source_node)  # Compute shortest distances
print(f"Shortest distances from node {source_node}:")  # Print the results
print(shortest_distances)

# Task 3 : Analyze the shortest path statistics for all nodes in the graph

def analyze_shortest_paths(graph):
    """
    Analyze shortest path statistics for all nodes in the graph.

    Parameters:
    - graph: 2D numpy array representing the adjacency matrix of the graph

    Returns:
    - stats: A pandas DataFrame containing statistics for each node
    """
    n = len(graph)
    stats = []

    for src in range(n):
        distances = dijkstra(graph, src)  # Compute shortest distances from the source node
        reachable_distances = [d for d in distances if d != -1]  # Exclude unreachable nodes
        avg_distance = np.mean(reachable_distances) if reachable_distances else float('inf')  # Average distance
        max_distance = max(reachable_distances) if reachable_distances else -1  # Maximum distance
        reachable_count = len(reachable_distances)  # Number of reachable nodes
        stats.append({
            "Node": src,
            "Average Distance": avg_distance,
            "Max Distance": max_distance,
            "Reachable Nodes": reachable_count,
            # "Total Nodes": n
        })

    return pd.DataFrame(stats)

# Perform analysis and print results
shortest_path_stats = analyze_shortest_paths(edge)
print("Shortest Path Statistics:")
print(shortest_path_stats)

# Task 4: Visualize the shortest path between two nodes

import matplotlib.pyplot as plt

def visualize_shortest_path_between_two_nodes(graph, start, end):
    """
    Visualize the shortest path between two nodes in the graph.

    Parameters:
    - graph: 2D numpy array representing the adjacency matrix of the graph
    - start: The starting node (0-indexed)
    - end: The ending node (0-indexed)
    """
    # Create a directed graph using NetworkX
    G = nx.DiGraph()

    # Add edges to the graph with weights
    n = len(graph)
    for i in range(n):
        for j in range(n):
            if graph[i][j] != INF and graph[i][j] > 0:  # Add only valid edges
                G.add_edge(i, j, weight=graph[i][j])

    # Find the shortest path between the start and end nodes
    try:
        shortest_path = nx.shortest_path(G, source=start, target=end, weight='weight')
        edges_in_path = [(shortest_path[i], shortest_path[i + 1]) for i in range(len(shortest_path) - 1)]
    except nx.NetworkXNoPath:
        print(f"No path exists between node {start} and node {end}.")
        return

    # Arrange nodes in two rows
    pos = {}
    row1, row2 = n // 2, n - n // 2
    for i in range(row1):
        pos[i] = (i, 1)  # First row
    for i in range(row1, n):
        pos[i] = (i - row1, 0)  # Second row

    # Draw the graph
    plt.figure(figsize=(10, 6))
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10, font_weight='bold')
    nx.draw_networkx_edge_labels(G, pos, edge_labels={(i, j): f"{graph[i][j]}" for i, j in G.edges()}, font_size=8)

    # Highlight the shortest path and the start/end nodes
    nx.draw_networkx_edges(G, pos, edgelist=edges_in_path, edge_color='red', width=2)
    nx.draw_networkx_nodes(G, pos, nodelist=[start], node_color='green', node_size=700, label="Start Node")
    nx.draw_networkx_nodes(G, pos, nodelist=[end], node_color='yellow', node_size=700, label="End Node")

    plt.title(f"Shortest Path from Node {start} to Node {end}")
    plt.show()

# Example usage: Visualize the shortest path between node 0 and node 2
def save_shortest_path_images(graph, output_folder="image"):
    """
    Generate and save images of the shortest paths between all pairs of reachable nodes.

    Parameters:
    - graph: 2D numpy array representing the adjacency matrix of the graph
    - output_folder: Folder where the images will be saved
    """
    # Create the output folder if it doesn't exist
    os.makedirs(output_folder, exist_ok=True)

    n = len(graph)
    for start in range(n):
        for end in range(n):
            if start != end:  # Skip paths from a node to itself
                # Check if a path exists between the nodes
                try:
                    G = nx.DiGraph()
                    for i in range(n):
                        for j in range(n):
                            if graph[i][j] != INF and graph[i][j] > 0:
                                G.add_edge(i, j, weight=graph[i][j])
                    nx.shortest_path(G, source=start, target=end, weight='weight')  # Check path existence
                except nx.NetworkXNoPath:
                    continue  # Skip if no path exists

                # Visualize and save the shortest path
                plt.figure(figsize=(10, 6))
                pos = {}
                row1, row2 = n // 2, n - n // 2
                for i in range(row1):
                    pos[i] = (i, 1)  # First row
                for i in range(row1, n):
                    pos[i] = (i - row1, 0)  # Second row

                nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10, font_weight='bold')
                nx.draw_networkx_edge_labels(G, pos, edge_labels={(i, j): f"{graph[i][j]}" for i, j in G.edges()}, font_size=8)

                # Highlight the shortest path
                shortest_path = nx.shortest_path(G, source=start, target=end, weight='weight')
                edges_in_path = [(shortest_path[i], shortest_path[i + 1]) for i in range(len(shortest_path) - 1)]
                nx.draw_networkx_edges(G, pos, edgelist=edges_in_path, edge_color='red', width=2)
                nx.draw_networkx_nodes(G, pos, nodelist=[start], node_color='green', node_size=700, label="Start Node")
                nx.draw_networkx_nodes(G, pos, nodelist=[end], node_color='yellow', node_size=700, label="End Node")

                plt.title(f"Shortest Path from Node {start} to Node {end}")
                output_path = os.path.join(output_folder, f"shortest_path_{start}_to_{end}.png")
                plt.savefig(output_path)
                plt.close()

# Generate and save images for all reachable node pairs
save_shortest_path_images(edge)

def visualize_dijkstra_execution(graph, src):
    """
    Visualize the step-by-step execution of Dijkstra's algorithm with dynamic distance updates.

    Parameters:
    - graph: 2D numpy array representing the adjacency matrix of the graph
    - src: The source node (0-indexed)
    """
    n = len(graph)
    dist = [INF] * n
    dist[src] = 0
    visited = [False] * n

    G = nx.DiGraph()
    for i in range(n):
        for j in range(n):
            if graph[i][j] != INF and graph[i][j] > 0:
                G.add_edge(i, j, weight=graph[i][j])

    # Use a circular layout for better spacing
    pos = nx.circular_layout(G)

    fig, ax = plt.subplots(figsize=(12, 8))

    def update(frame):
        ax.clear()
        nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=600, font_size=10, font_weight='bold', ax=ax)
        nx.draw_networkx_edge_labels(G, pos, edge_labels={(i, j): f"{graph[i][j]}" for i, j in G.edges()}, font_size=8, ax=ax)

        # Find the unvisited node with the smallest distance
        min_dist = INF
        u = -1
        for i in range(n):
            if not visited[i] and dist[i] < min_dist:
                min_dist = dist[i]
                u = i

        if u == -1:
            return

        visited[u] = True

        # Update distances to neighboring nodes
        for v in range(n):
            if not visited[v] and graph[u][v] != INF:
                dist[v] = min(dist[v], dist[u] + graph[u][v])

        # Highlight visited nodes and edges
        visited_nodes = [i for i in range(n) if visited[i]]
        nx.draw_networkx_nodes(G, pos, nodelist=visited_nodes, node_color='green', node_size=700, ax=ax)
        nx.draw_networkx_nodes(G, pos, nodelist=[u], node_color='yellow', node_size=800, ax=ax)

        # Highlight edges leading to updated distances
        for v in range(n):
            if not visited[v] and graph[u][v] != INF and dist[v] == dist[u] + graph[u][v]:
                nx.draw_networkx_edges(G, pos, edgelist=[(u, v)], edge_color='red', width=2, ax=ax)

        # Display updated distances on nodes
        labels = {i: f"\n{dist[i] if dist[i] < INF else '∞'}" for i in range(n)}
        nx.draw_networkx_labels(G, pos, labels=labels, font_size=10, font_color='black', ax=ax)

        ax.set_title(f"Dijkstra's Algorithm Execution from Node {src} (Step {frame + 1})", fontsize=14)

    ani = FuncAnimation(fig, update, frames=n, interval=1200, repeat=False)
    ani.save("dijkstra_execution_with_distances.mp4", writer="ffmpeg")

# Visualize Dijkstra's algorithm execution from node 0
visualize_dijkstra_execution(edge, src=0)
plt.show()  # Ensure the plot is displayed properly
