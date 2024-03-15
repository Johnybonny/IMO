#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

vector<vector<int>> parseInput(const string& filename)
{
    vector<vector<int>> result;
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return result;
    }

    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        vector<int> coordinates;
        int value;
        ss >> value; // Skip the first number
        while (ss >> value)
        {
            coordinates.push_back(value);
        }
        if(coordinates.size() > 0)
            result.push_back(coordinates);
    }

    inputFile.close();
    return result;
}

double euclideanDistance(const vector<int>& p1, const vector<int>& p2)
{
    double sum = 0.0;
    for (size_t i = 0; i < p1.size(); ++i)
    {
        sum += pow(p1[i] - p2[i], 2);
    }
    return sqrt(sum);
}

vector<vector<int>> initializeCycles(const vector<vector<double>>& distances)
{
    // Choose starting point
    srand( time( NULL ) );
    int startingPoint = rand() % distances.size();

    cout << "Wylosowany punkt: " << startingPoint << "\n";
    cout << "Odleglosci:\n";
    for (size_t i = 0; i < 10; ++i)
    {
        cout << distances[startingPoint][i] << "\n";
    }

    // Choose second point
    double maximumDistance = distances[startingPoint][startingPoint];
    int secondPoint = startingPoint;
    for (size_t j = 0; j < distances[startingPoint].size(); ++j)
    {
        if (distances[startingPoint][j] > maximumDistance)
        {
            maximumDistance = distances[startingPoint][j];
            secondPoint = j;
        }
    }

    vector<vector<int>> initializedCycles = {vector<int> {startingPoint}, vector<int> {secondPoint}};
    return initializedCycles;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    vector<vector<int>> parsedData = parseInput(filename);

    // Calculate Euclidean distances between each pair of vectors
    vector<vector<double>> distances(parsedData.size(), vector<double>(parsedData.size(), 0.0));
    for (size_t i = 0; i < parsedData.size(); ++i)
    {
        for (size_t j = i + 1; j < parsedData.size(); ++j)
        {
            double dist = euclideanDistance(parsedData[i], parsedData[j]);
            distances[i][j] = distances[j][i] = dist;
        }
    }

    // Output the distances
    for (size_t j = 0; j < distances[0].size(); ++j)
    {
        cout << distances[0][j] << " ";
    }
    cout << endl;

    // Initialize cycles with first point in each
    vector<vector<int>> cycles = initializeCycles(distances);
    cout << "First point: " << cycles[0][0] << "\n";
    cout << "Second point: " << cycles[1][0] << "\n";
    cout << "Distance between them: " << distances[cycles[0][0]][cycles[1][0]] << "\n";


    return 0;
}
