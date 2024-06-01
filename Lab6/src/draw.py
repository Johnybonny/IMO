import argparse
import matplotlib.pyplot as plt

def read_numbers(filename):
    numbers = []
    with open(filename, 'r') as file:
        for line in file:
            number = float(line)
            numbers.append(number)
    return numbers

def draw_map(scores, differences_to_best, average_differences, map_filename, title='', color1='b', color2='r'):
    plt.figure(figsize=(10, 6))
    plt.scatter(scores, differences_to_best, color=color1, label='Podobieństwo do najlepszego wyniku')
    plt.scatter(scores, average_differences, color=color2, label='Średnie podobieństwo do innych rozwiązań')

    # Adding title and labels
    plt.title(title)
    plt.xlabel('Długość cykli')
    plt.ylabel('Podobieństwo')

    # Adding a legend to differentiate the datasets
    plt.legend()

    # Save the plot to the specified file
    plt.savefig(map_filename, dpi=300)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process vertex file')
    parser.add_argument('vertex_file', type=str, help='Path to the vertex file')
    parser.add_argument('algorithm', type=str, help='Type of algorithm', choices=['common-vertices', 'common-edges'])
    args = parser.parse_args()
    instance = args.vertex_file.split('.')[0].split('/')[1]
    scores_file = f"out/1.txt"
    differences_to_best_file = f"out/2.txt"
    average_differences_file = f"out/3.txt"

    scores = read_numbers(scores_file)
    differences_to_best = read_numbers(differences_to_best_file)
    average_differences = read_numbers(average_differences_file)

    draw_map(scores, differences_to_best, average_differences, map_filename=f"out/{instance}_map_{args.algorithm}.png", title=f"Instance: {instance}, Algorithm: {args.algorithm}", color1='b', color2='r')
