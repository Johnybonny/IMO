import subprocess
import matplotlib.pyplot as plt
from pathlib import Path

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
    plt.close()

if __name__ == "__main__":

    for file in ['data/kroA100.tsp', 'data/kroB100.tsp']:
        for start in ['random', 'regret']:
            for policy in ['steepest', 'greedy', 'randomWalk', 'none']:
                for n_type in ['vertices', 'edges']:
                    print(f"Running: {file} {start} {policy} {n_type}")
                    directory = Path('out') / file.split('/')[1].split('.')[0] / start / policy / n_type
                    directory.mkdir(parents=True, exist_ok=True)
                    filename_1 = directory / "cycle1.txt"
                    filename_2 = directory / "cycle2.txt"
                    subprocess.run(['./out/bin/main.out', file, start, policy, n_type, '15000', str(filename_1), str(filename_2)])

                    vertices = read_vertex_coordinates(file)
                    connections1 = read_connections(str(filename_1))
                    connections2 = read_connections(str(filename_2))

                    filename_map = directory / "map.png"
                    draw_map(vertices, connections1, connections2, map_filename=str(filename_map), title=f"{file} {start} {policy} {n_type}", color1='b', color2='r')
