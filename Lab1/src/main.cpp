#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define NUM_OF_CYCLES 2
#define BIG_M 1000000000

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

void saveRegretResultsToFile(const vector<vector<int>>& results, const string& fileName)
{
    ofstream file;
    file.open(fileName);
    file << "Weight\tMinimum\tAverage\tMaximum\n";
    int maximumCycle = 0;
    int minimumCycle = BIG_M;
    double costWeight = 0.0;
    for (size_t i = 0; i < results.size(); ++i)
    {
        int sum = 0;
        for (size_t j = 0; j < results[i].size(); ++j)
        {
            if (results[i][j] > maximumCycle)
            {
                maximumCycle = results[i][j];
            }
            else if (results[i][j] < minimumCycle)
            {
                minimumCycle = results[i][j];
            }
            sum += results[i][j];
        }
        file << costWeight << "\t" << minimumCycle << "\t" << sum / results[i].size() << "\t" << maximumCycle << "\n";
        costWeight = costWeight + 0.1;
    }

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

vector<vector<int>> computeCyclesLengths(const vector<vector<int>>& cycles, const vector<vector<int>>& distances)
{
    vector<vector<int>> lengths = {vector<int> {}, vector<int> {}};
    // First cycle
    for (size_t i = 0; i < cycles[0].size() - 1; ++i)
    {
        lengths[0].push_back(distances[cycles[0][i]][cycles[0][i + 1]]);
    }
    lengths[0].push_back(distances[cycles[0][cycles[0].size() - 1]][cycles[0][0]]);

    // Second cycle
    for (size_t i = 0; i < cycles[1].size() - 1; ++i)
    {
        lengths[1].push_back(distances[cycles[1][i]][cycles[1][i + 1]]);
    }
    lengths[1].push_back(distances[cycles[1][cycles[1].size() - 1]][cycles[1][0]]);

    return lengths;
}

int getCycleLength(const vector<int>& lengths)
{
    int sum = 0;
    for (size_t i = 0; i < lengths.size(); ++i)
    {
        sum += lengths[i];
    }
    return sum;
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

pair<vector<vector<int>>, int> nearestNeighbor(const vector<vector<int>>& distances, int startingPoint)
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

            // Find a new point to create a path to from all points in cycle
            int bestPointFrom = 0;
            int bestPointFromIndex = 0;
            int bestPointTo = 0;
            int closestDistance = BIG_M;
            for (size_t point = 0; point < cyclesPoints[cycleIndex].size(); ++point)
            {
                // Look for point closest to the one in cycle
                for (size_t freePoint = 0; freePoint < taken.size(); ++freePoint)
                {
                    if (taken[freePoint])
                        continue;

                    if (distances[cyclesPoints[cycleIndex][point]][freePoint] < closestDistance)
                    {
                        closestDistance = distances[cyclesPoints[cycleIndex][point]][freePoint];
                        bestPointFrom = cyclesPoints[cycleIndex][point];
                        bestPointFromIndex = point;
                        bestPointTo = freePoint;
                    }
                }
            }

            // Decide wheter to put the point before or after the chosen cycle point
            if (bestPointFromIndex == 0)
            {
                int addBeforeCost = distances[bestPointFrom][bestPointTo];
                int addAfterCost = distances[bestPointFrom][bestPointTo]
                    + distances[cyclesPoints[cycleIndex][1]][bestPointTo]
                    - distances[bestPointFrom][cyclesPoints[cycleIndex][1]];

                if (addBeforeCost < addAfterCost)
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + 0, bestPointTo);
                else
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + 1, bestPointTo);
            }
            else if (bestPointFromIndex == cyclesPoints[cycleIndex].size() - 1)
            {
                int addBeforeCost = distances[bestPointFrom][bestPointTo]
                    + distances[cyclesPoints[cycleIndex][cyclesPoints[cycleIndex].size() - 2]][bestPointTo]
                    - distances[bestPointFrom][cyclesPoints[cycleIndex][cyclesPoints[cycleIndex].size() - 2]];
                int addAfterCost = distances[bestPointFrom][bestPointTo];

                if (addBeforeCost < addAfterCost)
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].end() - 1, bestPointTo);
                else
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].end() + 0, bestPointTo);
            }
            else
            {
                int addBeforeCost = distances[bestPointFrom][bestPointTo]
                    + distances[cyclesPoints[cycleIndex][bestPointFromIndex - 1]][bestPointTo]
                    - distances[bestPointFrom][cyclesPoints[cycleIndex][bestPointFromIndex - 1]];
                int addAfterCost = distances[bestPointFrom][bestPointTo]
                    + distances[cyclesPoints[cycleIndex][bestPointFromIndex + 1]][bestPointTo]
                    - distances[bestPointFrom][cyclesPoints[cycleIndex][bestPointFromIndex + 1]];

                if (addBeforeCost < addAfterCost)
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + bestPointFromIndex, bestPointTo);
                else
                    cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + bestPointFromIndex + 1, bestPointTo);
            }

            // Mark point as taken
            taken[bestPointTo] = true;
        }
    }

    int totalLength = getLengthBasedOnPoints(cyclesPoints[0], distances) + getLengthBasedOnPoints(cyclesPoints[1], distances);
    return {cyclesPoints, totalLength};
}

pair<vector<vector<int>>, int> greedyCycle(const vector<vector<int>>& distances, int startingPoint)
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

    // Initialize the cyclesLengths container
    vector<vector<int>> cyclesLengths = computeCyclesLengths(cyclesPoints, distances);

    while (any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
    {
        // For each cycle
        for (size_t cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; ++cycleIndex)
        {
            if (!any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
                break;

            // Find new point to insert from all free points
            int shortestNewCycleLength = BIG_M;
            int bestPoint = 0;
            int bestPlace = 0;
            for (size_t point = 0; point < taken.size(); ++point)
            {
                if (taken[point])
                    continue;

                // Find place to insert the new point (after the place index)
                for (size_t place = 0; place < cyclesPoints[cycleIndex].size(); ++place)
                {
                    // Create new cyclePoints vector
                    vector<int> newCyclePoints = cyclesPoints[cycleIndex];
                    size_t indexToInsert = (place + 1) % (newCyclePoints.size() + 1);
                    newCyclePoints.insert(newCyclePoints.begin() + indexToInsert, point);

                    // Create new cycleLengths vector
                    vector<int> newCycleLengths = cyclesLengths[cycleIndex];
                    // Remove edge
                    newCycleLengths.erase(newCycleLengths.begin() + place);
                    // Insert first edge
                    int valueToInsert = distances[point][newCyclePoints[(place + 2) % newCyclePoints.size()]];
                    newCycleLengths.insert(newCycleLengths.begin() + place, valueToInsert);
                    // Insert second edge
                    valueToInsert = distances[newCyclePoints[place]][point];
                    newCycleLengths.insert(newCycleLengths.begin() + place, valueToInsert);

                    // Get new cycle total length
                    int newTotalLength = getCycleLength(newCycleLengths);
                    if (newTotalLength < shortestNewCycleLength)
                    {
                        shortestNewCycleLength = newTotalLength;
                        bestPoint = point;
                        bestPlace = place;
                    }
                }
            }

            // Add chosen point to the cycle
            size_t indexToInsert = (bestPlace + 1) % (cyclesPoints[cycleIndex].size() + 1);
            cyclesPoints[cycleIndex].insert(cyclesPoints[cycleIndex].begin() + indexToInsert, bestPoint);
            cyclesLengths[cycleIndex].erase(cyclesLengths[cycleIndex].begin() + bestPlace);
            int valueToInsert = distances[bestPoint][cyclesPoints[cycleIndex][(bestPlace + 2) % cyclesPoints[cycleIndex].size()]];
            cyclesLengths[cycleIndex].insert(cyclesLengths[cycleIndex].begin() + bestPlace, valueToInsert);
            valueToInsert = distances[cyclesPoints[cycleIndex][bestPlace]][bestPoint];
            cyclesLengths[cycleIndex].insert(cyclesLengths[cycleIndex].begin() + bestPlace, valueToInsert);

            // Mark point as taken
            taken[bestPoint] = true;
        }
    }

    int totalLength = getCycleLength(cyclesLengths[0]) + getCycleLength(cyclesLengths[1]);
    return {cyclesPoints, totalLength};
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

    // Initialize the cyclesLengths container
    vector<vector<int>> cyclesLengths = computeCyclesLengths(cyclesPoints, distances);

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
                    // Create new cyclePoints vector
                    vector<int> newCyclePoints = cyclesPoints[cycleIndex];
                    size_t indexToInsert = (place + 1) % (newCyclePoints.size() + 1);
                    newCyclePoints.insert(newCyclePoints.begin() + indexToInsert, point);

                    // Create new cycleLengths vector
                    vector<int> newCycleLengths = cyclesLengths[cycleIndex];
                    // Remove edge
                    newCycleLengths.erase(newCycleLengths.begin() + place);
                    // Insert first edge
                    int valueToInsert = distances[point][newCyclePoints[(place + 2) % newCyclePoints.size()]];
                    newCycleLengths.insert(newCycleLengths.begin() + place, valueToInsert);
                    // Insert second edge
                    valueToInsert = distances[newCyclePoints[place]][point];
                    newCycleLengths.insert(newCycleLengths.begin() + place, valueToInsert);

                    // Get the cost of inserting the point
                    int insertCost = getCycleLength(newCycleLengths) - getCycleLength(cyclesLengths[cycleIndex]);
                    costs.push_back({place, insertCost});
                }

                // Compute the regret update the highest regret
                // regret = {{<where to insert>, <regret value>}, <cost of insertion>}
                pair<pair<int, int>, int> regret = computeRegret(costs);
                int weightedRegret = int(regret.first.second  - costWeight * regret.second);
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
            cyclesLengths[cycleIndex].erase(cyclesLengths[cycleIndex].begin() + bestPlace);
            int valueToInsert = distances[bestPoint][cyclesPoints[cycleIndex][(bestPlace + 2) % cyclesPoints[cycleIndex].size()]];
            cyclesLengths[cycleIndex].insert(cyclesLengths[cycleIndex].begin() + bestPlace, valueToInsert);
            valueToInsert = distances[cyclesPoints[cycleIndex][bestPlace]][bestPoint];
            cyclesLengths[cycleIndex].insert(cyclesLengths[cycleIndex].begin() + bestPlace, valueToInsert);

            // Mark point as taken
            taken[bestPoint] = true;
        }
    }

    int totalLength = getCycleLength(cyclesLengths[0]) + getCycleLength(cyclesLengths[1]);
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

    // Nearest neighbor algorithm
    cout << "Nearest neighbor\n";
    vector<int> nearestNeighborResults = {};
    vector<vector<int>> nearestNeighborBestPoints = {};
    int nearestNeighborBest = BIG_M;
    for (int startingPoint = 0; startingPoint < distances.size(); ++startingPoint)
    {
        cout << startingPoint + 1 << "%" << "\n";
        pair<vector<vector<int>>, int> nearestNeighborResult = nearestNeighbor(distances, startingPoint);

        int score = nearestNeighborResult.second;
        nearestNeighborResults.push_back(score);
        if (score < nearestNeighborBest)
        {
            nearestNeighborBestPoints = nearestNeighborResult.first;
            nearestNeighborBest = score;
        }
    }

    // Save nearest neighbor results
    saveCycleToFile(nearestNeighborBestPoints, "out/" + filenameStripped + "_nearest_neighbor1.txt", "out/" + filenameStripped + "_nearest_neighbor2.txt");
    saveResultsToFile(nearestNeighborResults, "out/" + filenameStripped + "_nearest_neighbor_results.txt");

    // Greedy cycle algorithm
    cout << "Greedy cycle\n";
    vector<int> greedyCycleResults = {};
    vector<vector<int>> greedyCycleBestPoints = {};
    int greedyCycleBest = BIG_M;
    for (int startingPoint = 0; startingPoint < distances.size(); ++startingPoint)
    {
        cout << startingPoint + 1 << "%" << "\n";
        pair<vector<vector<int>>, int> greedyCycleResult = greedyCycle(distances, startingPoint);

        int score = greedyCycleResult.second;
        greedyCycleResults.push_back(score);
        if (score < greedyCycleBest)
        {
            greedyCycleBestPoints = greedyCycleResult.first;
            greedyCycleBest = score;
        }
    }

    // Save greedy cycle results
    saveCycleToFile(greedyCycleBestPoints, "out/" + filenameStripped + "_greedy_cycle1.txt", "out/" + filenameStripped + "_greedy_cycle2.txt");
    saveResultsToFile(greedyCycleResults, "out/" + filenameStripped + "_greedy_cycle_results.txt");

    // Regret heuristics algorithm
    cout << "Regret heuristics algorithm\n";
    vector<vector<int>> regretHeuristicsResults = {};
    vector<vector<int>> regretHeuristicsBestPoints = {};
    int regretHeuristicsBest = BIG_M;
    for (double costWeight = 0.0; costWeight <= 1.0; costWeight = costWeight + 0.1)
    {
        regretHeuristicsResults.push_back({});
        cout << int((costWeight) * 100) << "%" << "\n";
        for (int startingPoint = 0; startingPoint < distances.size(); ++startingPoint)
        {
            pair<vector<vector<int>>, int> regretHeuristicsResult = regretHeuristics(distances, startingPoint, costWeight);

            int score = regretHeuristicsResult.second;
            regretHeuristicsResults[regretHeuristicsResults.size() - 1].push_back(score);
            if (score < regretHeuristicsBest)
            {
                regretHeuristicsBestPoints = regretHeuristicsResult.first;
                regretHeuristicsBest = score;
            }
        }
    }

    // Save regret heuristics results
    saveCycleToFile(regretHeuristicsBestPoints, "out/" + filenameStripped + "_regret_heuristics1.txt", "out/" + filenameStripped + "_regret_heuristics2.txt");
    saveRegretResultsToFile(regretHeuristicsResults, "out/" + filenameStripped + "_regret_heuristics_results.txt");

    return 0;
}
