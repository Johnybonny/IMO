#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

int findFurthestPoint(const vector<vector<double>>& distances, const vector<bool>& taken, int point)
{
    double maximumDistance = distances[point][point];
    int furthestPoint = point;
    for (size_t j = 0; j < distances[point].size(); ++j)
    {
        if ((distances[point][j] > maximumDistance) && !taken[j])
        {
            maximumDistance = distances[point][j];
            furthestPoint = j;
        }
    }
    return furthestPoint;
}

int findClosestPoint(const vector<vector<double>>& distances, const vector<bool>& taken, int point)
{
    double minimumDistance = 1000000000; // TODO: Choose good number for minimum distance
    int closestPoint = point;
    for (size_t j = 0; j < distances[point].size(); ++j)
    {
        if ((distances[point][j] < minimumDistance) && !taken[j])
        {
            minimumDistance = distances[point][j];
            closestPoint = j;
        }
    }
    return closestPoint;
}

vector<vector<int>> initializeCycles(const vector<vector<double>>& distances)
{
    // Choose starting point
    srand( time( NULL ) );
    int startingPoint = rand() % distances.size();

    // Choose second point
    int secondPoint = findFurthestPoint(distances, vector<bool>(distances.size(), false), startingPoint);

    vector<vector<int>> initializedCycles = {vector<int> {startingPoint}, vector<int> {secondPoint}};
    return initializedCycles;
}

void showCycles(const vector<vector<int>>& cycles)
{
    cout << "First cycle\tSecond cycle\n";
    for (size_t j = 0; j < cycles[0].size(); ++j)
    {
        cout << cycles[0][j] << "\t\t" << cycles[1][j] << "\n";
    }
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

    // Initialize cycles with first point in each
    vector<vector<int>> cycles = initializeCycles(distances);
    vector<bool> taken = vector<bool>(distances.size(), false);
    taken[cycles[0][0]] = 1;
    taken[cycles[1][0]] = 1;

    // Start the cycles with second point for each previously generated point
    int newPoint0 = findClosestPoint(distances, taken, cycles[0][0]);
    cycles[0].push_back(newPoint0);
    taken[newPoint0] = true;
    int newPoint1 = findClosestPoint(distances, taken, cycles[1][0]);
    cycles[1].push_back(newPoint1);
    taken[newPoint1] = true;

    showCycles(cycles);

    return 0;
}
