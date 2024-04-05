#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <ctime>
#include <vector>

#define NUM_OF_CYCLES 2
#define BIG_M 1000000000
#define COST_WEIGHT 0.6

using namespace std;

void progressBar(double progress)
{
    cout << "[";
    for (int i = 0; i < progress; i++)
    {
        if (i % 10 == 0)
            cout << "=";
    }
    cout << ">";
    for (int i = progress; i < 100; i++)
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

void saveResultsToFile(const vector<int>& results, const string& fileName)
{
    int maximumCycle = 0;
    int minimumCycle = BIG_M;
    int sum = 0;
    for (size_t i = 0; i < results.size(); i++)
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
    for (size_t i = 0; i < p1.size(); i++)
    {
        sum += pow(p1[i] - p2[i], 2);
    }
    return int(sqrt(sum) + 0.5);
}

int findFurthestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, int point)
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

int findClosestPoint(const vector<vector<int>>& distances, const vector<bool>& taken, int point)
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
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        sum += distances[points[i]][points[i + 1]];
    }
    sum += distances[points[points.size() - 1]][points[0]];
    return sum;
}

void showCycles(const vector<vector<int>>& cycles)
{
    for (size_t i = 0; i < cycles.size(); i++)
    {
        cout << "Cycle no." << i << ": ";
        for (size_t j = 0; j < cycles[i].size(); j++)
        {
            cout << cycles[i][j] << " ";
        }
        cout << "\n";
    }
}

pair<pair<int, int>, int> computeRegret(vector<pair<int, int>>& costs)
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

vector<vector<int>> regretCycle(const vector<vector<int>>& distances)
{
    vector<vector<int>> regretHeuristicsPoints = {};
    int regretHeuristicsBest = BIG_M;
    for (int startingPoint = 0; startingPoint < distances.size(); startingPoint++)
    {
        pair<vector<vector<int>>, int> regretHeuristicsResult = regretHeuristics(distances, startingPoint, COST_WEIGHT);

        int score = regretHeuristicsResult.second;
        if (score < regretHeuristicsBest)
        {
            regretHeuristicsPoints = regretHeuristicsResult.first;
            regretHeuristicsBest = score;
        }
    }
    return regretHeuristicsPoints;
}

vector<vector<int>> randomCycle(const vector<vector<int>>& distances)
{
    vector<vector<int>> cyclesPoints = {{}, {}};
    vector<int> freePoints = {};

    for (int i = 0; i < distances.size(); i++)
    {
        freePoints.push_back(i);
    }

    srand(time(NULL));
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

int insideVerticesExchange(const vector<int>& cycle, const int firstPointIndex,
    const int secondPointIndex, const vector<vector<int>>& distances)
{
    int delta = 0;

    int firstPoint = cycle[firstPointIndex];
    int firstPointBefore = cycle[(firstPointIndex - 1 + cycle.size()) % cycle.size()];
    int firstPointAfter = cycle[(firstPointIndex + 1) % cycle.size()];

    int secondPoint = cycle[secondPointIndex];
    int secondPointBefore = cycle[(secondPointIndex - 1 + cycle.size()) % cycle.size()];
    int secondPointAfter = cycle[(secondPointIndex + 1) % cycle.size()];

    delta = delta - distances[firstPointBefore][firstPoint] - distances[firstPoint][firstPointAfter];
    delta = delta - distances[secondPointBefore][secondPoint] - distances[secondPoint][secondPointAfter];
    delta = delta + distances[firstPointBefore][secondPoint] + distances[secondPoint][firstPointAfter];
    delta = delta + distances[secondPointBefore][firstPoint] + distances[firstPoint][secondPointAfter];

    // Condition in case the points are one after another
    if (((firstPointIndex + 1) % cycle.size() == secondPointIndex)
        || ((firstPointIndex - 1 + cycle.size()) % cycle.size() == secondPointIndex))
    {
        delta = delta + 2 * distances[firstPoint][secondPoint];
    }

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
        return insideVerticesExchange(cyclesPoints[move[3]], move[1], move[2], distances);
        break;

    case 3:
        return insideEdgesExchange(cyclesPoints[move[3]], move[1], move[2], distances);
        break;

    default:
        return BIG_M;
        break;
    }
}

vector<vector<int>> generateMoves(vector<vector<int>>& cyclesPoints, bool isVertices, const vector<vector<int>>& distances)
{
    // Create a vector of all possible moves
    vector<vector<int>> allPossibleMoves = {};
    for (int move = 1; move < 4; move++)
    {
        if ((isVertices && move == 3) || (!isVertices && move == 2))
        {
            continue;
        }

        for (int firstVertex = 0; firstVertex < cyclesPoints[0].size() - 1; firstVertex++)
        {
            for (int secondVertex = firstVertex + 1; secondVertex < cyclesPoints[0].size(); secondVertex++)
            {
                for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
                {
                    allPossibleMoves.push_back({move, firstVertex, secondVertex, cycleIndex});
                }
            }
        }
    }

    // Shuffle the possible moves
    random_device rd;
    mt19937 g(rd());
    shuffle(allPossibleMoves.begin(), allPossibleMoves.end(), g);

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
        newCyclesPoints[move[3]][move[1]] = cyclesPoints[move[3]][move[2]];
        newCyclesPoints[move[3]][move[2]] = cyclesPoints[move[3]][move[1]];
    }
    else if (move[0] == 3)
    {
        int firstPointIndex = move[1];
        int firstPointAfterIndex = (firstPointIndex + 1) % cyclesPoints[move[3]].size();
        int secondPointIndex = move[2];
        int secondPointAfterIndex = (secondPointIndex + 1) % cyclesPoints[move[3]].size();
        int lastIndexToChange = secondPointIndex;
        for (int i = firstPointAfterIndex; i < secondPointAfterIndex; i++)
        {
            newCyclesPoints[move[3]][i] = cyclesPoints[move[3]][lastIndexToChange];
            lastIndexToChange--;
        }
    }

    return newCyclesPoints;
}

void steepest(vector<vector<int>>& cyclesPoints, bool isVertices, const vector<vector<int>>& distances)
{
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;

        int bestDelta = BIG_M;
        // bestMove{<move>, <first vertex index>, <second vertex index>, <cycle index>}
        // move: 1 - exchanging vertices between two cycles
        //       2 - exchanging vertices in one cycle
        //       3 - exchanging edges in one cycle
        vector<int> bestMove = {0, 0, 0, 0};
        int delta;

        // Exchanging the vertices between two cycles
        for (int aPointIndex = 0; aPointIndex < cyclesPoints[0].size(); aPointIndex++)
        {
            for (int bPointIndex = aPointIndex; bPointIndex < cyclesPoints[1].size(); bPointIndex++)
            {
                delta = computeDeltaBasedOnMove({1, aPointIndex, bPointIndex, 0}, cyclesPoints, distances);
                if (delta < 0 && delta < bestDelta)
                {
                    bestDelta = delta;
                    bestMove = {1, aPointIndex, bPointIndex, 0};
                    stopCondition = false;
                }
            }
        }

        if (isVertices)
        {
            // Exchanging two vertices in one cycle
            for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
            {
                for (int firstPointIndex = 0; firstPointIndex < cyclesPoints[cycleIndex].size() - 1; firstPointIndex++)
                {
                    for (int secondPointIndex = firstPointIndex + 1; secondPointIndex < cyclesPoints[cycleIndex].size(); secondPointIndex++)
                    {
                        delta = computeDeltaBasedOnMove({2, firstPointIndex, secondPointIndex, cycleIndex}, cyclesPoints, distances);
                        if (delta < 0 && delta < bestDelta)
                        {
                            bestDelta = delta;
                            stopCondition = false;
                            bestMove = {2, firstPointIndex, secondPointIndex, cycleIndex};
                        }
                    }
                }
            }
        }
        else
        {
            // Exchanging two edges in one cycle
            for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
            {
                for (int firstPointIndex = 0; firstPointIndex < cyclesPoints[cycleIndex].size() - 1; firstPointIndex++)
                {
                    for (int secondPointIndex = firstPointIndex + 1; secondPointIndex < cyclesPoints[cycleIndex].size(); secondPointIndex++)
                    {
                        delta = computeDeltaBasedOnMove({3, firstPointIndex, secondPointIndex, cycleIndex}, cyclesPoints, distances);
                        if (delta < 0 && delta < bestDelta)
                        {
                            bestDelta = delta;
                            stopCondition = false;
                            bestMove = {3, firstPointIndex, secondPointIndex, cycleIndex};
                        }
                    }
                }
            }
        }

        if (!stopCondition)
        {
            cyclesPoints = makeMove(cyclesPoints, bestMove);
        }
    }
}

void greedy(vector<vector<int>>& cyclesPoints, bool isVertices, const vector<vector<int>>& distances)
{
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;
        vector<vector<int>> allPossibleMoves = generateMoves(cyclesPoints, isVertices, distances);
        for (int moveIndex = 0; moveIndex < allPossibleMoves.size(); moveIndex++)
        {
            int delta = computeDeltaBasedOnMove(allPossibleMoves[moveIndex], cyclesPoints, distances);
            if (delta < 0)
            {
                cyclesPoints = makeMove(cyclesPoints, allPossibleMoves[moveIndex]);
                stopCondition = false;
                break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0] << " <input_filename> <random|regret> <steepest|greedy>";
        cerr << " <vertices|edges>" << endl;
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

    // Generate initial cycles
    cout << "\nGenerating initial cycles...\n";
    vector<vector<int>> cyclesPoints;
    if (string(argv[2]) == "regret")
    {
        cyclesPoints = regretCycle(distances);
    }
    else if (string(argv[2]) == "random")
    {
        cyclesPoints = randomCycle(distances);
    }
    else
    {
        cerr << argv[2] << " can be 'random' or 'regret'" << endl;
        return 1;
    }

    // Neighbourhood types
    bool isVertices;
    if (string(argv[4]) == "vertices")
    {
        isVertices = true;
    }
    else if (string(argv[4]) == "edges")
    {
        isVertices = false;
    }
    else
    {
        cerr << argv[4] << " can be 'vertices' or 'edges'" << endl;
        return 1;
    }

    showCycles(cyclesPoints);
    cout << "Cycles total length: " << getLengthBasedOnPoints(cyclesPoints[0], distances)
        + getLengthBasedOnPoints(cyclesPoints[1], distances) << "\n";

    // Start algorithm
    cout << "\nStarting algorithm...\n";
    if (string(argv[3]) == "steepest")
    {
        steepest(cyclesPoints, isVertices, distances);
    }
    else if (string(argv[3]) == "greedy")
    {
        greedy(cyclesPoints, isVertices, distances);
    }
    else
    {
        cerr << argv[3] << " can be 'steepest' or 'greedy'" << endl;
        return 1;
    }

    showCycles(cyclesPoints);
    cout << "Cycles total length: " << getLengthBasedOnPoints(cyclesPoints[0], distances)
        + getLengthBasedOnPoints(cyclesPoints[1], distances) << "\n";

    return 0;
}
