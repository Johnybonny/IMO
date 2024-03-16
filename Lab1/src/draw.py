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

def draw_map(vertices, connections1, connections2, color1='b', color2='r'):
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
    plt.title('Map with Vertices and Connections')
    plt.grid(True)
    plt.savefig("data/map.png", dpi=300)

if __name__ == "__main__":
    vertex_file = "data/kroA100.tsp"
    connection_file1 = "data/connections1.txt"
    connection_file2 = "data/connections2.txt"


    vertices = read_vertex_coordinates(vertex_file)
    connections1 = read_connections(connection_file1)
    connections2 = read_connections(connection_file2)

    draw_map(vertices, connections1, connections2, color1='b', color2='r')
