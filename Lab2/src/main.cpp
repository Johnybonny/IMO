#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define NUM_OF_CYCLES 2
#define BIG_M 1000000000
#define COST_WEIGHT 0.6

using namespace std;

void progressBar(double progress)
{
    cout << "[";
    int full = 100;
    for (int i = 0; i < progress; i++)
    {
        if (i % 10 == 0)
            cout << "=";
    }
    cout << ">";
    for (int i = progress; i < full; i++)
    {
        if (i % 10 == 0)
            cout << " ";
    }
    cout << "]\n";
}

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

void saveCycleToFile(const vector<vector<int>>& points, const string& fileName1, const string& fileName2)
{
    ofstream file0;
    file0.open(fileName1);
    for (size_t i = 0; i < points[0].size() - 1; ++i)
    {
        file0 << points[0][i] << "\t" << points[0][i + 1] << "\n";
    }
    file0 << points[0][points[0].size() - 1] << "\t" << points[0][0];
    file0.close();

    ofstream file1;
    file1.open(fileName2);
    for (size_t i = 0; i < points[1].size() - 1; ++i)
    {
        file1 << points[1][i] << "\t" << points[1][i + 1] << "\n";
    }
    file1 << points[1][points[1].size() - 1] << "\t" << points[1][0];
    file1.close();
}

void saveResultsToFile(const vector<int>& results, const string& fileName)
{
    int maximumCycle = 0;
    int minimumCycle = BIG_M;
    int sum = 0;
    for (size_t i = 0; i < results.size(); ++i)
    {
        if (results[i] > maximumCycle)
        {
            maximumCycle = results[i];
        }
        else if (results[i] < minimumCycle)
        {
            minimumCycle = results[i];
        }
        sum += results[i];
    }

    ofstream file;
    file.open(fileName);
    file << "Minimum:" << "\t" << minimumCycle << "\n";
    file << "Average:" << "\t" << sum / results.size() << "\n";
    file << "Maximum:" << "\t" << maximumCycle << "\n";
    file.close();
}

int euclideanDistance(const vector<int>& p1, const vector<int>& p2)
{
    double sum = 0.0;
    for (size_t i = 0; i < p1.size(); ++i)
    {
        sum += pow(p1[i] - p2[i], 2);
    }
    return int(sqrt(sum) + 0.5);
}

int findFurthestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, int point)
{
    int maximumDistance = distances[point][point];
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

int findClosestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, int point)
{
    int minimumDistance = BIG_M;
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

vector<vector<int>> initializeCyclesPoints(const vector<vector<int>>& distances, int startingPoint)
{
    // Choose second point
    int secondPoint = findFurthestPoint(distances, vector<bool>(distances.size(), false), startingPoint);

    vector<vector<int>> initializedCycles = {vector<int> {startingPoint}, vector<int> {secondPoint}};
    return initializedCycles;
}

int getLengthBasedOnPoints(const vector<int>& points, const vector<vector<int>>& distances)
{
    int sum = 0;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        sum += distances[points[i]][points[i + 1]];
    }
    sum += distances[points[points.size() - 1]][points[0]];
    return sum;
}

void showCycles(const vector<vector<int>>& cycles)
{
    cout << "First cycle\tSecond cycle\n";
    for (size_t j = 0; j < cycles[0].size(); ++j)
    {
        if (cycles[0].size() > j)
            cout << cycles[0][j];
        if (cycles[1].size() > j)
            cout << "\t\t" << cycles[1][j];
        cout << "\n";
    }
}

pair<pair<int, int>, int> computeRegret(vector<pair<int, int>>& costs)
{
    int lowestValue = BIG_M;
    int lowestValuePlace = 0;
    int secondLowestValue = BIG_M;
    int secondLowestValuePlace = 0;

    for (size_t i = 0; i < costs.size(); ++i)
    {
        if (costs[i].second < lowestValue)
        {
            secondLowestValue = lowestValue;
            lowestValue = costs[i].second;
            lowestValuePlace = costs[i].first;
        }
        else if (costs[i].second < secondLowestValue)
        {
            secondLowestValue = costs[i].second;
        }
    }

    return {{lowestValuePlace, int(secondLowestValue - lowestValue)}, lowestValue};
}

pair<vector<vector<int>>, int> regretHeuristics(const vector<vector<int>>& distances, int startingPoint, double costWeight)
{
    // Initialize cycles with first point in each
    vector<vector<int>> cyclesPoints = initializeCyclesPoints(distances, startingPoint);
    vector<bool> taken = vector<bool>(distances.size(), false);
    taken[cyclesPoints[0][0]] = true;
    taken[cyclesPoints[1][0]] = true;

    // Start the cycles with second point for each previously generated point
    int newPoint0 = findClosestPoint(distances, taken, cyclesPoints[0][0]);
    cyclesPoints[0].push_back(newPoint0);
    taken[newPoint0] = true;
    int newPoint1 = findClosestPoint(distances, taken, cyclesPoints[1][0]);
    cyclesPoints[1].push_back(newPoint1);
    taken[newPoint1] = true;

    while (any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
    {
        // For each cycle
        for (size_t cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; ++cycleIndex)
        {
            if (!any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
                break;

            // Find new point with the highest regret to insert
            int highestRegret = -BIG_M;
            int bestPoint = 0;
            int bestPlace = 0;
            int lowestCost = BIG_M;
            vector<pair<pair<int, int>, int>> regretValues = {};
            for (size_t point = 0; point < taken.size(); ++point)
            {
                if (taken[point])
                    continue;

                // Find place to insert the new point (after the place index)
                vector<pair<int, int>> costs = {};
                for (size_t place = 0; place < cyclesPoints[cycleIndex].size(); ++place)
                {
                    // Get the cost of inserting the point
                    int pointBefore = cyclesPoints[cycleIndex][place];
                    int pointAfter = cyclesPoints[cycleIndex][(place + 1) % cyclesPoints[cycleIndex].size()];
                    int insertCost = distances[pointBefore][point]
                        + distances[point][pointAfter]
                        - distances[pointBefore][pointAfter];

                    costs.push_back({place, insertCost});
                }

                // Compute the regret update the highest regret
                // regret = {{<where to insert>, <regret value>}, <cost of insertion>}
                pair<pair<int, int>, int> regret = computeRegret(costs);
                int weightedRegret = regret.first.second  - int(costWeight * regret.second);
                if ((weightedRegret > highestRegret)
                    || (weightedRegret == highestRegret && regret.second < lowestCost))
                {
                    highestRegret = weightedRegret;
                    bestPoint = point;
                    bestPlace = regret.first.first;
                    lowestCost = regret.second;
                }
            }

            // Add chosen point to the cycle
            size_t indexToInsert = (bestPlace + 1) % (cyclesPoints[cycleIndex].size() + 1);
            cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + indexToInsert, bestPoint);

            // Mark point as taken
            taken[bestPoint] = true;
        }
    }

    int totalLength = getLengthBasedOnPoints(cyclesPoints[0], distances)
        + getLengthBasedOnPoints(cyclesPoints[1], distances);

    return {cyclesPoints, totalLength};
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    string filenameStripped = filename.substr(filename.find_last_of('/') + 1);
    filenameStripped = filenameStripped.substr(0, filenameStripped.find_last_of('.'));
    vector<vector<int>> parsedData = parseInput(filename);

    // Calculate Euclidean distances between each pair of vectors
    vector<vector<int>> distances(parsedData.size(), vector<int>(parsedData.size(), 0.0));
    for (size_t i = 0; i < parsedData.size(); ++i)
    {
        for (size_t j = i + 1; j < parsedData.size(); ++j)
        {
            int dist = euclideanDistance(parsedData[i], parsedData[j]);
            distances[i][j] = distances[j][i] = dist;
        }
    }

    // Regret heuristics algorithm
    cout << "Regret heuristics algorithm\n";
    vector<vector<int>> regretHeuristicsResults = {};
    vector<vector<int>> regretHeuristicsBestPoints = {};
    int regretHeuristicsBest = BIG_M;
    regretHeuristicsResults.push_back({});
    for (int startingPoint = 0; startingPoint < distances.size(); ++startingPoint)
    {
        progressBar(startingPoint);
        pair<vector<vector<int>>, int> regretHeuristicsResult = regretHeuristics(distances, startingPoint, COST_WEIGHT);

        int score = regretHeuristicsResult.second;
        regretHeuristicsResults[regretHeuristicsResults.size() - 1].push_back(score);
        if (score < regretHeuristicsBest)
        {
            regretHeuristicsBestPoints = regretHeuristicsResult.first;
            regretHeuristicsBest = score;
        }
    }

    return 0;
}
