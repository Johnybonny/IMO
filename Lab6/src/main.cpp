#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>

#define BIG_M 1000000000
#define COST_WEIGHT 0.6
#define NUM_OF_CYCLES 2
#define NUM_OF_ITERATIONS 10

using namespace std;

enum MoveType
{
    VerticesExchange,
    EdgesExchange,
};

struct IndexedMove
{
    MoveType type;
    int firstIndex;
    int secondIndex;
    int cycle;

    void show() const
    {
        cout << type << " " << firstIndex << " " << secondIndex << " " << cycle << "\n";
    }
};

struct GlobalTestsResults
{
    vector<int> scores;
    vector<int> differencesToBest;
    vector<float> averageDifferences;
};

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

void saveResultsToFile(const GlobalTestsResults& results, const string& fileName1, const string& fileName2, const string& fileName3)
{
    ofstream file1;
    file1.open(fileName1);
    for (int i = 0; i < results.scores.size(); i++)
    {
        file1 << results.scores[i] << "\n";
    }
    file1.close();

    ofstream file2;
    file2.open(fileName2);
    for (int i = 0; i < results.differencesToBest.size(); i++)
    {
        file2 << results.differencesToBest[i] << "\n";
    }
    file2.close();

    ofstream file3;
    file3.open(fileName3);
    for (int i = 0; i < results.averageDifferences.size(); i++)
    {
        file3 << results.averageDifferences[i] << "\n";
    }
    file3.close();
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

template <typename T>
void showCorrelation(const vector<int>& x, const vector<T>& y, const string& title)
{
    if (x.size() != y.size())
        return;

    float xMean = reduce(x.begin(), x.end()) / static_cast<float>(x.size());
    float yMean = reduce(y.begin(), y.end()) / static_cast<float>(y.size());

    float numerator = 0.0;
    float denominatorX = 0.0;
    float denominatorY = 0.0;


    for (int i = 0; i < x.size(); i++)
    {
        numerator += (x[i] - xMean) * (y[i] - yMean);
        denominatorX += (x[i] - xMean) * (x[i] - xMean);
        denominatorY += (y[i] - yMean) * (y[i] - yMean);
    }

    float coefficient = numerator / sqrt(denominatorX * denominatorY);
    cout << title << coefficient << "\n";
}

int indexOfPoint(int point, const vector<int>& cycle)
{
    return find(cycle.begin(), cycle.end(), point) - cycle.begin();
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

int getFullLength(const vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    int totalLength = 0;
    for (int i = 0; i < cyclesPoints.size(); i++)
    {
        totalLength += getLengthBasedOnPoints(cyclesPoints[i], distances);
    }
    return totalLength;
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

vector<IndexedMove> generateMoves(const int size, const vector<vector<int>>& distances)
{
    vector<IndexedMove> allPossibleMoves{};

    // Add vertices exchange moves
    for (int firstVertex = 0; firstVertex < size; firstVertex++)
    {
        for (int secondVertex = 0; secondVertex < size; secondVertex++)
        {
            IndexedMove move = {VerticesExchange, firstVertex, secondVertex, 0};
            allPossibleMoves.push_back(move);
        }
    }

    // Add edges exchange moves
    for (int firstVertex = 0; firstVertex < size - 1; firstVertex++)
    {
        for (int secondVertex = firstVertex + 1; secondVertex < size; secondVertex++)
        {
            for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
            {
                IndexedMove move = {EdgesExchange, firstVertex, secondVertex, cycleIndex};
                allPossibleMoves.push_back(move);
            }
        }
    }

    return allPossibleMoves;
}

int verticesExchangeDelta(const IndexedMove& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    int delta = 0;

    int aPoint = cyclesPoints[0][move.firstIndex];
    int aPointBefore = cyclesPoints[0][(move.firstIndex - 1 + cyclesPoints[0].size()) % cyclesPoints[0].size()];
    int aPointAfter = cyclesPoints[0][(move.firstIndex + 1) % cyclesPoints[0].size()];

    int bPoint = cyclesPoints[1][move.secondIndex];
    int bPointBefore = cyclesPoints[1][(move.secondIndex - 1 + cyclesPoints[1].size()) % cyclesPoints[1].size()];
    int bPointAfter = cyclesPoints[1][(move.secondIndex + 1) % cyclesPoints[1].size()];

    delta = delta - distances[aPointBefore][aPoint] - distances[aPoint][aPointAfter];
    delta = delta - distances[bPointBefore][bPoint] - distances[bPoint][bPointAfter];
    delta = delta + distances[aPointBefore][bPoint] + distances[aPointAfter][bPoint];
    delta = delta + distances[bPointBefore][aPoint] + distances[bPointAfter][aPoint];

    return delta;
}

int edgesExchangeDelta(const IndexedMove& move, const vector<vector<int>>& distances, vector<vector<int>>& cyclesPoints)
{
    int delta = 0;

    int aPoint = cyclesPoints[move.cycle][move.firstIndex];
    int aPointAfter = cyclesPoints[move.cycle][(move.firstIndex + 1) % cyclesPoints[move.cycle].size()];

    int bPoint = cyclesPoints[move.cycle][move.secondIndex];
    int bPointAfter = cyclesPoints[move.cycle][(move.secondIndex + 1) % cyclesPoints[move.cycle].size()];

    delta = delta - distances[aPoint][aPointAfter] - distances[bPoint][bPointAfter];
    delta = delta + distances[aPoint][bPoint] + distances[aPointAfter][bPointAfter];

    return delta;
}

int computeDelta(const IndexedMove& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    if (move.type == VerticesExchange)
        return verticesExchangeDelta(move, cyclesPoints, distances);
    else
        return edgesExchangeDelta(move, distances, cyclesPoints);
}

vector<vector<int>> makeMove(const vector<vector<int>>& cyclesPoints, const IndexedMove& move)
{
    bool flag = false;

    vector<vector<int>> newCyclesPoints = cyclesPoints;

    if (move.type == VerticesExchange)
    {
        // Switch the points in cycles
        newCyclesPoints[0][move.firstIndex] = cyclesPoints[1][move.secondIndex];
        newCyclesPoints[1][move.secondIndex] = cyclesPoints[0][move.firstIndex];
    }
    else
    {
        int cycleSize = cyclesPoints[move.cycle].size();

        int aPointIndex = move.firstIndex;
        int aPointAfterIndex = (move.firstIndex + 1) % cyclesPoints[move.cycle].size();

        int bPointIndex = move.secondIndex;
        int bPointAfterIndex = (move.secondIndex + 1) % cyclesPoints[move.cycle].size();

        // Decide wheter change the direction of edges between the end of first edge and the start of second
        int distanceBetweenFront;
        // Or to change the direction of edges between the end of second edge and the start of first
        int distanceBetweenBack;
        if (aPointAfterIndex < bPointIndex)
        {
            distanceBetweenFront = abs(aPointAfterIndex - bPointIndex);
        }
        else
        {
            distanceBetweenFront = abs(aPointAfterIndex - bPointIndex - cycleSize);
        }
        distanceBetweenBack = cycleSize - distanceBetweenFront - 2;

        // Set start and end of the series to invert
        int startIndex;
        int endIndex;
        if (distanceBetweenFront <= distanceBetweenBack)
        {
            startIndex = aPointAfterIndex;
            endIndex = bPointIndex;
        }
        else
        {
            startIndex = bPointAfterIndex;
            endIndex = aPointIndex;
        }

        bool normalOrder = startIndex <= endIndex;

        // Switch the direction of all the vertices between start and end indices
        bool stop = false;
        do
        {
            newCyclesPoints[move.cycle][startIndex] = cyclesPoints[move.cycle][endIndex];
            newCyclesPoints[move.cycle][endIndex] = cyclesPoints[move.cycle][startIndex];

            endIndex = (endIndex - 1 + cycleSize) % cycleSize;
            startIndex = (startIndex + 1) % cycleSize;

            if ((normalOrder && startIndex >= endIndex && startIndex - endIndex <= 1)
                || (!normalOrder && endIndex >= startIndex
                    && (endIndex - startIndex == 1 || endIndex - startIndex == cycleSize - 1 || endIndex - startIndex == 0)))
            {
                stop = true;
            }
        } while (!stop);
    }

    return newCyclesPoints;
}

void greedy(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances, vector<IndexedMove>& allPossibleMoves)
{
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;

        // Shuffle moves vector
        std::shuffle(allPossibleMoves.begin(), allPossibleMoves.end(), gen);

        // Find move to make
        for (int moveIndex = 0; moveIndex < allPossibleMoves.size(); moveIndex++)
        {
            int delta = computeDelta(allPossibleMoves[moveIndex], cyclesPoints, distances);
            if (delta < 0)
            {
                cout << "delta: " << delta << "\n";
                makeMove(cyclesPoints, allPossibleMoves[moveIndex]);
                stopCondition = false;
            }
        }
    }
}

void steepest(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances, const vector<IndexedMove>& allPossibleMoves)
{
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;
        int bestDelta = BIG_M;
        IndexedMove bestMove;

        for (int moveIndex = 0; moveIndex < allPossibleMoves.size(); moveIndex++)
        {
            int delta = computeDelta(allPossibleMoves[moveIndex], cyclesPoints, distances);
            if (delta < 0 && delta < bestDelta)
            {
                bestDelta = delta;
                bestMove = allPossibleMoves[moveIndex];
                stopCondition = false;
            }
        }

        if (!stopCondition)
            cyclesPoints = makeMove(cyclesPoints, bestMove);
    }
}

pair<pair<int, int>, int> computeRegret(const vector<pair<int, int>>& costs)
{
    int lowestValue = BIG_M;
    int lowestValuePlace = 0;
    int secondLowestValue = BIG_M;
    int secondLowestValuePlace = 0;

    for (int i = 0; i < costs.size(); i++)
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

vector<vector<int>> regretHeuristics(const vector<vector<int>>& distances, const int startingPoint, const double costWeight)
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
        for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
        {
            if (!any_of(taken.begin(), taken.end(), [](bool value) { return !value; }))
                break;

            if (cyclesPoints[cycleIndex].size() == taken.size() / 2)
                continue;

            // Find new point with the highest regret to insert
            int highestRegret = -BIG_M;
            int bestPoint = 0;
            int bestPlace = 0;
            int lowestCost = BIG_M;
            vector<pair<pair<int, int>, int>> regretValues = {};
            for (int point = 0; point < taken.size(); point++)
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

        if ((cyclesPoints[0].size() == taken.size() / 2)
            && (cyclesPoints[1].size() == taken.size() / 2))
            break;
    }

    return cyclesPoints;
}

int commonVertices(const vector<vector<int>>& firstCycle, const vector<vector<int>>& secondCycle)
{
    int sameVertices = 0;
    for (int firstPointIndex = 0; firstPointIndex < firstCycle[0].size(); firstPointIndex++)
    {
        for (int secondPointIndex = 0; secondPointIndex < secondCycle[0].size(); secondPointIndex++)
        {
            if (firstCycle[0][firstPointIndex] == secondCycle[0][secondPointIndex])
                sameVertices++;
        }
    }

    // Simmilarity in cycles is between 0 and 0, 1 and 1
    if (sameVertices > (firstCycle[0].size() / 2))
        return sameVertices * 2;

    // Simmilarity in cycles is between 0 and 1, 1 and 0
    return (firstCycle[0].size() - sameVertices) * 2;
}

int commonEdges(const vector<vector<int>>& firstCycle, const vector<vector<int>>& secondCycle)
{
    int sameEdges = 0;
    for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
    {
        for (int pointIndex = 0; pointIndex < firstCycle[cycleIndex].size(); pointIndex++)
        {
            int firstEdgeA = firstCycle[cycleIndex][pointIndex];
            int firstEdgeB = firstCycle[cycleIndex][(pointIndex + 1) % firstCycle[cycleIndex].size()];

            int secondEdgeAIndex = indexOfPoint(firstEdgeA, secondCycle[0]);
            if (secondEdgeAIndex != secondCycle[0].size())
            {
                if (secondCycle[0][(secondEdgeAIndex + 1) % secondCycle[0].size()] == firstEdgeB)
                    sameEdges++;
            }
            else
            {
                secondEdgeAIndex = indexOfPoint(firstEdgeA, secondCycle[1]);
                if (secondCycle[1][(secondEdgeAIndex + 1) % secondCycle[1].size()] == firstEdgeB)
                    sameEdges++;
            }
        }
    }

    return sameEdges;
}

GlobalTestsResults globalTests(const vector<vector<int>>& distances, const bool useCommonVertices, const bool compareBest)
{
    vector<vector<int>> cyclesPoints;
    vector<vector<int>> bestCycles;
    int bestScore = BIG_M;
    int score;
    if (!compareBest)
    {
        // Create best solution
        cout << "Create best solution\n";
        vector<bool> taken = vector<bool>(distances.size(), false);
        for (int i = 0; i < 200; i++)
        {
            cout << (i/200.0) * 100.0 << "%\n";
            // cyclesPoints = randomCycle(distances);
            cyclesPoints = regretHeuristics(distances, i, COST_WEIGHT);
            score = getFullLength(cyclesPoints, distances);

            if (score < bestScore)
            {
                bestScore = score;
                bestCycles = cyclesPoints;
            }
        }
        cout << "Best score: " << bestScore << "\n";
    }

    vector<IndexedMove> allPossibleMoves = generateMoves(distances.size() / 2, distances);

    // Create 1000 solutions using greedy
    cout << "Create 1000 solutions using greedy\n";
    vector<vector<vector<int>>> solutions;
    for (int i = 0; i < 1000; i++)
    {
        cout << (i/1000.0) * 100.0 << "%\n";
        cyclesPoints = randomCycle(distances);
        // greedy(cyclesPoints, distances, allPossibleMoves);
        steepest(cyclesPoints, distances, allPossibleMoves);
        solutions.push_back(cyclesPoints);
        if (compareBest)
        {
            score = getFullLength(cyclesPoints, distances);
            if (score < bestScore)
            {
                bestScore = score;
                bestCycles = cyclesPoints;
            }
        }
    }


    vector<int> commonBests;
    vector<float> commonMeans;
    // Compute differences
    cout << "Compute differences\n";
    for (int i = 0; i < solutions.size(); i++)
    {
        if (useCommonVertices)
            commonBests.push_back(commonVertices(solutions[i], bestCycles));
        else
            commonBests.push_back(commonEdges(solutions[i], bestCycles));

        float commonMean = 0.0;
        for (int j = 0; j < solutions.size(); j++)
        {
            if (i != j)
            {
                if (useCommonVertices)
                    commonMean += commonVertices(solutions[i], solutions[j]);
                else
                    commonMean += commonEdges(solutions[i], solutions[j]);
            }
        }

        commonMeans.push_back(commonMean / solutions.size());
    }

    // Append scores
    vector<int> scores;
    for (int i = 0; i < solutions.size(); i++)
        scores.push_back(getFullLength(solutions[i], distances));

    // Show correlation coefficient
    if (compareBest)
    {
        int bestScore = BIG_M;
        int bestIndex = 0;
        for (int i = 0; i < solutions.size(); i++)
        {
            if (scores[i] < bestScore)
            {
                bestScore = scores[i];
                bestIndex = i;
            }
        }
        vector<int> removedBestScores = scores;
        removedBestScores.erase(removedBestScores.begin() + bestIndex);

        vector<int> removedBestCommonBests = commonBests;
        removedBestCommonBests.erase(removedBestCommonBests.begin() + bestIndex);

        vector<float> removedBestCommonMeans = commonMeans;
        removedBestCommonMeans.erase(removedBestCommonMeans.begin() + bestIndex);

        cout << "Współczynniki korelacji:\n";
        showCorrelation(removedBestScores, removedBestCommonBests, "Różnice w stosunku do najlepszego rozwiązania: ");
        showCorrelation(removedBestScores, removedBestCommonMeans, "Średnie różnice: ");
    }
    else
    {
        showCorrelation(scores, commonBests, "Różnice w stosunku do najlepszego rozwiązania: ");
        showCorrelation(scores, commonMeans, "Średnie różnice: ");
    }

    return {scores, commonBests, commonMeans};
}

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename_1> <output_filename_2>";
        cerr << " <output_filename_3> <common-vertices|common-edges> <compare-best|compare-regret>" << endl;
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

    srand(time(NULL));
    bool useCommonVertices = string(argv[5]) == "common-vertices";
    bool compareWithBest = string(argv[6]) == "compare-best";

    // Start algorithm
    GlobalTestsResults results = globalTests(distances, useCommonVertices, compareWithBest);

    saveResultsToFile(results, argv[2], argv[3], argv[4]);

    return 0;
}
