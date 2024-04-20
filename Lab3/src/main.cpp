#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#define BIG_M 1000000000
#define COST_WEIGHT 0.6
#define NUM_OF_CYCLES 2
#define NUM_OF_ITERATIONS 10

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
    for (size_t i = 0; i < points[0].size() - 1; i++)
    {
        file0 << points[0][i] << "\t" << points[0][i + 1] << "\n";
    }
    file0 << points[0][points[0].size() - 1] << "\t" << points[0][0];
    file0.close();

    ofstream file1;
    file1.open(fileName2);
    for (size_t i = 0; i < points[1].size() - 1; i++)
    {
        file1 << points[1][i] << "\t" << points[1][i + 1] << "\n";
    }
    file1 << points[1][points[1].size() - 1] << "\t" << points[1][0];
    file1.close();
}

int euclideanDistance(const vector<int>& p1, const vector<int>& p2)
{
    double sum = 0.0;
    for (size_t i = 0; i < p1.size(); i++)
    {
        sum += pow(p1[i] - p2[i], 2);
    }
    return int(sqrt(sum) + 0.5);
}

int findFurthestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, const int point)
{
    int maximumDistance = distances[point][point];
    int furthestPoint = point;
    for (size_t j = 0; j < distances[point].size(); j++)
    {
        if ((distances[point][j] > maximumDistance) && !taken[j])
        {
            maximumDistance = distances[point][j];
            furthestPoint = j;
        }
    }
    return furthestPoint;
}

int findClosestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, const int point)
{
    int minimumDistance = BIG_M;
    int closestPoint = point;
    for (size_t j = 0; j < distances[point].size(); j++)
    {
        if ((distances[point][j] < minimumDistance) && !taken[j])
        {
            minimumDistance = distances[point][j];
            closestPoint = j;
        }
    }
    return closestPoint;
}

vector<vector<int>> initializeCyclesPoints(const vector<vector<int>>& distances, const int startingPoint)
{
    // Choose second point
    int secondPoint = findFurthestPoint(distances, vector<bool>(distances.size(), false), startingPoint);

    vector<vector<int>> initializedCycles = {vector<int> {startingPoint}, vector<int> {secondPoint}};
    return initializedCycles;
}

int getLengthBasedOnPoints(const vector<int>& points, const vector<vector<int>>& distances)
{
    int sum = 0;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        sum += distances[points[i]][points[i + 1]];
    }
    sum += distances[points[points.size() - 1]][points[0]];
    return sum;
}

pair<pair<int, int>, int> computeRegret(const vector<pair<int, int>>& costs)
{
    int lowestValue = BIG_M;
    int lowestValuePlace = 0;
    int secondLowestValue = BIG_M;
    int secondLowestValuePlace = 0;

    for (size_t i = 0; i < costs.size(); i++)
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

pair<vector<vector<int>>, int> regretHeuristics(const vector<vector<int>>& distances, const int startingPoint, const double costWeight)
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
        for (size_t cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
        {
            if (!any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
                break;

            // Find new point with the highest regret to insert
            int highestRegret = -BIG_M;
            int bestPoint = 0;
            int bestPlace = 0;
            int lowestCost = BIG_M;
            vector<pair<pair<int, int>, int>> regretValues = {};
            for (size_t point = 0; point < taken.size(); point++)
            {
                if (taken[point])
                    continue;

                // Find place to insert the new point (after the place index)
                vector<pair<int, int>> costs = {};
                for (size_t place = 0; place < cyclesPoints[cycleIndex].size(); place++)
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

vector<vector<int>> randomCycle(const vector<vector<int>>& distances)
{
    vector<vector<int>> cyclesPoints = {{}, {}};
    vector<int> freePoints = {};

    for (int i = 0; i < distances.size(); i++)
    {
        freePoints.push_back(i);
    }

    while (freePoints.size() != 0)
    {
        for (size_t cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
        {
            if(freePoints.size() == 0)
                break;

            int randomIndex = rand() % freePoints.size();
            cyclesPoints[cycleIndex].push_back(freePoints[randomIndex]);
            freePoints.erase(freePoints.begin() + randomIndex);
        }
    }

    return cyclesPoints;
}

int outsideVerticesExchange(const vector<int>& cycleA, const vector<int>& cycleB,
    const int aPointIndex, const int bPointIndex, const vector<vector<int>>& distances)
{
    int delta = 0;

    int aPoint = cycleA[aPointIndex];
    int aPointBefore = cycleA[(aPointIndex - 1 + cycleA.size()) % cycleA.size()];
    int aPointAfter = cycleA[(aPointIndex + 1) % cycleA.size()];

    int bPoint = cycleB[bPointIndex];
    int bPointBefore = cycleB[(bPointIndex - 1 + cycleB.size()) % cycleB.size()];
    int bPointAfter = cycleB[(bPointIndex + 1) % cycleB.size()];

    delta = delta - distances[aPointBefore][aPoint] - distances[aPoint][aPointAfter];
    delta = delta - distances[bPointBefore][bPoint] - distances[bPoint][bPointAfter];
    delta = delta + distances[aPointBefore][bPoint] + distances[aPointAfter][bPoint];
    delta = delta + distances[bPointBefore][aPoint] + distances[bPointAfter][aPoint];

    return delta;
}

int insideEdgesExchange(const vector<int>& cycle, const int firstPointIndex,
    const int secondPointIndex, const vector<vector<int>>& distances)
{
    int delta = 0;

    int firstPoint = cycle[firstPointIndex];
    int firstPointAfter = cycle[(firstPointIndex + 1) % cycle.size()];

    int secondPoint = cycle[secondPointIndex];
    int secondPointAfter = cycle[(secondPointIndex + 1) % cycle.size()];

    delta = delta - distances[firstPoint][firstPointAfter] - distances[secondPoint][secondPointAfter];
    delta = delta + distances[firstPoint][secondPoint] + distances[firstPointAfter][secondPointAfter];

    return delta;
}

int computeDeltaBasedOnMove(const vector<int>& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    switch (move[0])
    {
    case 1:
        return outsideVerticesExchange(cyclesPoints[0], cyclesPoints[1], move[1], move[2], distances);
        break;
    case 2:
        return insideEdgesExchange(cyclesPoints[move[3]], move[1], move[2], distances);
        break;
    default:
        return BIG_M;
        break;
    }
}

vector<vector<int>> generateMoves(const int size, const vector<vector<int>>& distances)
{
    // Create a vector of all possible moves
    vector<vector<int>> allPossibleMoves = {};
    for (int move = 1; move < 3; move++)
    {
        for (int firstVertex = 0; firstVertex < size - 1; firstVertex++)
        {
            for (int secondVertex = firstVertex + 1; secondVertex < size; secondVertex++)
            {
                if (move == 1)
                {
                    allPossibleMoves.push_back({move, firstVertex, secondVertex, 0});
                }
                else
                {
                    for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
                    {
                        allPossibleMoves.push_back({move, firstVertex, secondVertex, cycleIndex});
                    }
                }
            }
        }
    }

    return allPossibleMoves;
}

vector<vector<int>> makeMove(const vector<vector<int>>& cyclesPoints, const vector<int>& move)
{
    vector<vector<int>> newCyclesPoints = cyclesPoints;
    if (move[0] == 1)
    {
        newCyclesPoints[0][move[1]] = cyclesPoints[1][move[2]];
        newCyclesPoints[1][move[2]] = cyclesPoints[0][move[1]];
    }
    else if (move[0] == 2)
    {
        int firstPointIndex = move[1];
        int firstPointAfterIndex = (firstPointIndex + 1) % cyclesPoints[move[3]].size();
        int secondPointIndex = move[2];
        int secondPointAfterIndex = (secondPointIndex + 1) % cyclesPoints[move[3]].size();

        int lastIndexToChange = secondPointIndex;
        int current = firstPointAfterIndex;
        while(current != secondPointAfterIndex)
        {
            newCyclesPoints[move[3]][current] = cyclesPoints[move[3]][lastIndexToChange];
            lastIndexToChange--;

            current = (current + 1) % cyclesPoints[move[3]].size();
        }
    }

    return newCyclesPoints;
}

void steepest(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances, vector<vector<int>>& allPossibleMoves)
{
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;
        int bestDelta = BIG_M;
        // bestMove{<move>, <first vertex index>, <second vertex index>, <cycle index>}
        // move: 1 - exchanging vertices between two cycles
        //       2 - exchanging edges in one cycle
        vector<int> bestMove = {0, 0, 0, 0};

        for (int moveIndex = 0; moveIndex < allPossibleMoves.size(); moveIndex++)
        {
            int delta = computeDeltaBasedOnMove(allPossibleMoves[moveIndex], cyclesPoints, distances);
            if (delta < 0 && delta < bestDelta)
            {
                bestDelta = delta;
                bestMove = allPossibleMoves[moveIndex];
                stopCondition = false;
            }
        }

        if (!stopCondition)
        {
            cyclesPoints = makeMove(cyclesPoints, bestMove);
        }
    }
}

void pastMoves(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    cout << "Past moves algorithm is yet to be implemented\n";
}

void candidates(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    cout << "Candidates algorithm is yet to be implemented\n";
}

pair<vector<vector<int>>, vector<int>> computeStatistics(const vector<vector<vector<int>>>& results,
    const vector<vector<int>>& distances, const vector<int>& times)
{
    int sum = 0;
    int bestResult = BIG_M;
    int worstResult = 0;
    vector<vector<int>> bestCycles;
    for (int res = 0; res < results.size(); res++)
    {
        int score = getLengthBasedOnPoints(results[res][0], distances)
            + getLengthBasedOnPoints(results[res][1], distances);

        if (score < bestResult)
        {
            bestCycles = results[res];
            bestResult = score;
        }
        else if (score > worstResult)
        {
            worstResult = score;
        }

        sum += score;
    }

    int minTime = *min_element(times.begin(), times.end());
    int maxTime = *max_element(times.begin(), times.end());
    float const count = static_cast<float>(times.size());
    int averageTime = int(reduce(times.begin(), times.end()) / count);

    return {bestCycles, {bestResult, int(sum/results.size()), worstResult, minTime, averageTime, maxTime}};
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0] << " <input_filename> <pastMoves|candidates|steepest|regret>";
        cerr << "<output_filename_1> <output_filename_2>" << endl;
        return 1;
    }

    string filename = argv[1];
    string filenameStripped = filename.substr(filename.find_last_of('/') + 1);
    filenameStripped = filenameStripped.substr(0, filenameStripped.find_last_of('.'));
    vector<vector<int>> parsedData = parseInput(filename);

    // Calculate Euclidean distances between each pair of vectors
    vector<vector<int>> distances(parsedData.size(), vector<int>(parsedData.size(), 0.0));
    for (size_t i = 0; i < parsedData.size(); i++)
    {
        for (size_t j = i + 1; j < parsedData.size(); j++)
        {
            int dist = euclideanDistance(parsedData[i], parsedData[j]);
            distances[i][j] = distances[j][i] = dist;
        }
    }

    vector<vector<vector<int>>> results = {};
    vector<int> times = {};
    if (string(argv[2]) == "regret")
    {
        pair<vector<vector<int>>, int> regretResult;
        int bestScore = BIG_M;
        vector<vector<int>> bestCycles;
        for (int firstPoint = 0; firstPoint < distances.size(); firstPoint++)
        {
            cout << (firstPoint + 1) * (100.0 / distances.size()) << "%\n";
            chrono::steady_clock::time_point beginTimeMeasurement = chrono::steady_clock::now();
            regretResult = regretHeuristics(distances, firstPoint, COST_WEIGHT);
            chrono::steady_clock::time_point endTimeMeasurement = chrono::steady_clock::now();

            times.push_back(chrono::duration_cast<std::chrono::microseconds>(endTimeMeasurement - beginTimeMeasurement).count());
            results.push_back(regretResult.first);
        }
        // statistics{<best cycles points>, {min, mean, max, avg time}}
        pair<vector<vector<int>>, vector<int>> statistics = computeStatistics(results, distances, times);
        cout << "Values:\tMin: " << statistics.second[0] << "\tMean: " << statistics.second[1] << "\tMax: " << statistics.second[2] << "\n";
        cout << "Time:\tMin: " << statistics.second[3] << "\tMean: " << statistics.second[4] << "\tMax: " << statistics.second[5] << "\n";

        saveCycleToFile(statistics.first, argv[3], argv[4]);
        return 0;
    }

    srand(time(NULL));
    vector<vector<int>> allPossibleMoves = generateMoves(distances.size() / 2, distances);
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++)
    {
        cout << (iteration + 1) * (100.0 / NUM_OF_ITERATIONS) << "%\n";

        // Start algorithm
        vector<vector<int>> cyclesPoints = randomCycle(distances);
        chrono::steady_clock::time_point beginTimeMeasurement = chrono::steady_clock::now();
        if (string(argv[2]) == "pastMoves")
        {
            pastMoves(cyclesPoints, distances);
        }
        else if (string(argv[2]) == "candidates")
        {
            candidates(cyclesPoints, distances);
        }
        else if (string(argv[2]) == "steepest")
        {
            steepest(cyclesPoints, distances, allPossibleMoves);
        }
        else if (string(argv[2]) == "none")
        {
        }
        else
        {
            cerr << argv[2] << " can be 'pastMoves', 'candidates', 'steepest' or 'regret'" << endl;
            return 1;
        }
        chrono::steady_clock::time_point endTimeMeasurement = chrono::steady_clock::now();

        results.push_back(cyclesPoints);
        times.push_back(chrono::duration_cast<std::chrono::microseconds>(endTimeMeasurement - beginTimeMeasurement).count());
    }

    // statistics{<best cycles points>, {min, mean, max, avg time}}
    pair<vector<vector<int>>, vector<int>> statistics = computeStatistics(results, distances, times);
    cout << "Values:\tMin: " << statistics.second[0] << "\tMean: " << statistics.second[1] << "\tMax: " << statistics.second[2] << "\n";
    cout << "Time:\tMin: " << statistics.second[3] << "\tMean: " << statistics.second[4] << "\tMax: " << statistics.second[5] << "\n";

    saveCycleToFile(statistics.first, argv[3], argv[4]);

    return 0;
}
