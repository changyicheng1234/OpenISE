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
        labels = {i: f"{i}\n{dist[i] if dist[i] < INF else '∞'}" for i in range(n)}
        nx.draw_networkx_labels(G, pos, labels=labels, font_size=10, font_color='black', ax=ax)

        ax.set_title(f"Dijkstra's Algorithm Execution from Node {src} (Step {frame + 1})", fontsize=14)

    ani = FuncAnimation(fig, update, frames=n, interval=1200, repeat=False)
    plt.show()
    ani.save("dijkstra_execution_with_distances.mp4", writer="pillow")

# Visualize Dijkstra's algorithm execution from node 0
visualize_dijkstra_execution(edge, src=0)