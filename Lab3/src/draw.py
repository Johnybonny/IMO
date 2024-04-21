import argparse
import matplotlib.pyplot as plt

def read_vertex_coordinates(filename):
    vertices = {}
    with open(filename, 'r') as file:
        start_reading = False
        for line in file:
            if "EOF" in line:
                break
            if start_reading:
                vertex, x, y = line.strip().split()
                vertices[str(int(vertex)-1)] = (float(x), float(y))
            elif "NODE_COORD_SECTION" in line:
                start_reading = True
    return vertices


def read_connections(filename):
    connections = []
    with open(filename, 'r') as file:
        for line in file:
            vertex1, vertex2 = line.strip().split('\t')
            connections.append((vertex1, vertex2))
    return connections

def draw_map(vertices, connections1, connections2, map_filename, title='', color1='b', color2='r'):
    fig, ax = plt.subplots()

    # Plot vertices
    for vertex, (x, y) in vertices.items():
        ax.plot(x, y, 'ko')  # Draw vertices
        # ax.text(x, y, vertex, fontsize=12, ha='right')  # Label vertices

    # Plot connections from the first file
    for connection in connections1:
        vertex1, vertex2 = connection
        x1, y1 = vertices[vertex1]
        x2, y2 = vertices[vertex2]
        ax.plot([x1, x2], [y1, y2], color1, linewidth=2)  # Draw connections

    # Plot connections from the second file
    for connection in connections2:
        vertex1, vertex2 = connection
        x1, y1 = vertices[vertex1]
        x2, y2 = vertices[vertex2]
        ax.plot([x1, x2], [y1, y2], color2, linewidth=2)  # Draw connections

    ax.set_aspect('equal', adjustable='box')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title(title)
    plt.grid(True)
    plt.savefig(map_filename, dpi=300)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process vertex file')
    parser.add_argument('vertex_file', type=str, help='Path to the vertex file')
    parser.add_argument('algorithm', type=str, help='Type of algorithm', choices=['steepest', 'pastMoves', 'regret_heuristics'])
    args = parser.parse_args()
    instance = args.vertex_file.split('.')[0].split('/')[1]
    connection_file1 = f"out/1.txt"
    connection_file2 = f"out/2.txt"

    vertices = read_vertex_coordinates(args.vertex_file)
    connections1 = read_connections(connection_file1)
    connections2 = read_connections(connection_file2)

    draw_map(vertices, connections1, connections2, map_filename=f"out/{instance}_map_{args.algorithm}.png", title=f"Instance: {instance}, Algorithm: {args.algorithm}", color1='b', color2='r')
