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
#define NUM_OF_ITERATIONS 100

using namespace std;

enum MoveType
{
    VerticesExchange,
    EdgesExchange,
};

enum ApplicabilityStatus
{
    Applicable,
    MaybeApplicable,
    InApplicable,
};

struct Move
{
    MoveType type;
    int beforeFirst;
    int first;
    int afterFirst;
    int beforeSecond;
    int second;
    int afterSecond;
    int cycle;

    void show()
    {
        cout << type << " " << beforeFirst << " " << first << " " << afterFirst << " ";
        cout << beforeSecond << " "<< second << " " << afterSecond << " " << cycle << "\n";
    }
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

int indexOfPoint(int point, const vector<int>& cycle)
{
    return find(cycle.begin(), cycle.end(), point) - cycle.begin();
}

int cycleDirection(int a, int b, vector<int>& cycle)
{
    // 1 meaning a is before b, -1 meaning a is after b
    if ((indexOfPoint(a, cycle) + 1) % cycle.size() == indexOfPoint(b, cycle))
        return 1;
    else if ((indexOfPoint(b, cycle) + 1) % cycle.size() == indexOfPoint(a, cycle))
        return -1;

    // Error - something is not correct
    return 0;
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

int computeDeltaBasedOnLMMove(const vector<int>& LMmove, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    int firstVertexIndex;
    int secondVertexIndex;
    switch (LMmove[0])
    {
    case 1:
        firstVertexIndex = find(cyclesPoints[0].begin(), cyclesPoints[0].end(), LMmove[1])
            - cyclesPoints[0].begin();
        secondVertexIndex = find(cyclesPoints[1].begin(), cyclesPoints[1].end(), LMmove[3])
            - cyclesPoints[1].begin();

        return outsideVerticesExchange(cyclesPoints[0], cyclesPoints[1], firstVertexIndex, secondVertexIndex, distances);
        break;
    case 2:
        firstVertexIndex = find(cyclesPoints[LMmove[5]].begin(), cyclesPoints[LMmove[5]].end(), LMmove[1])
            - cyclesPoints[LMmove[5]].begin();
        secondVertexIndex = find(cyclesPoints[LMmove[5]].begin(), cyclesPoints[LMmove[5]].end(), LMmove[3])
            - cyclesPoints[LMmove[5]].begin();

        return insideEdgesExchange(cyclesPoints[LMmove[5]], firstVertexIndex, secondVertexIndex, distances);
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
    for (int firstVertex = 0; firstVertex < size - 1; firstVertex++)
    {
        for (int secondVertex = firstVertex + 1; secondVertex < size; secondVertex++)
        {
            for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
            {
                allPossibleMoves.push_back({2, firstVertex, secondVertex, cycleIndex});
            }
        }
    }

    for (int firstVertex = 0; firstVertex < size; firstVertex++)
    {
        for (int secondVertex = 0; secondVertex < size; secondVertex++)
        {
            for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
            {
                allPossibleMoves.push_back({1, firstVertex, secondVertex, 0});
            }
        }
    }


    // for (int move = 1; move < 3; move++)
    // {
    //     for (int firstVertex = 0; firstVertex < size - 1; firstVertex++)
    //     {
    //         for (int secondVertex = firstVertex + 1; secondVertex < size; secondVertex++)
    //         {
    //             if (move == 1)
    //             {
    //                 allPossibleMoves.push_back({move, firstVertex, secondVertex, 0});
    //             }
    //             else
    //             {
    //                 for (int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
    //                 {
    //                     allPossibleMoves.push_back({move, firstVertex, secondVertex, cycleIndex});
    //                 }
    //             }
    //         }
    //     }
    // }

    return allPossibleMoves;
}

vector<vector<int>> makeMove2(const vector<vector<int>>& cyclesPoints, const vector<int>& move)
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

vector<vector<int>> makeLMMove(const vector<vector<int>>& cyclesPoints, const vector<int>& LMmove)
{
    vector<vector<int>> newCyclesPoints = cyclesPoints;
    if (LMmove[0] == 1)
    {
        int firstVertexIndex = find(cyclesPoints[0].begin(), cyclesPoints[0].end(), LMmove[1])
        - cyclesPoints[0].begin();
        int secondVertexIndex = find(cyclesPoints[1].begin(), cyclesPoints[1].end(), LMmove[3])
            - cyclesPoints[1].begin();
        newCyclesPoints[0][firstVertexIndex] = LMmove[3];
        newCyclesPoints[1][secondVertexIndex] = LMmove[1];
    }
    else if (LMmove[0] == 2)
    {
        int firstVertexIndex = find(cyclesPoints[LMmove[5]].begin(), cyclesPoints[LMmove[5]].end(), LMmove[1])
            - cyclesPoints[LMmove[5]].begin();
        int secondVertexIndex = find(cyclesPoints[LMmove[5]].begin(), cyclesPoints[LMmove[5]].end(), LMmove[3])
            - cyclesPoints[LMmove[5]].begin();
        int firstVertexAfterIndex = (firstVertexIndex + 1) % cyclesPoints[LMmove[5]].size(); // 6
        int secondVertexAfterIndex = (secondVertexIndex + 1) % cyclesPoints[LMmove[5]].size(); // 2

        int current = firstVertexAfterIndex; // 6
        int lastIndexToChange = secondVertexIndex; // 2

        while(current != lastIndexToChange)
        {
            newCyclesPoints[LMmove[5]][current] = cyclesPoints[LMmove[5]][lastIndexToChange];
            newCyclesPoints[LMmove[5]][lastIndexToChange] = cyclesPoints[LMmove[5]][current];

            int difference;
            if (current <= lastIndexToChange)
            {
                difference = lastIndexToChange - current;
            }
            else
            {
                difference = cyclesPoints[LMmove[5]].size() - current + lastIndexToChange;
            }

            if(difference == 1)
            {
                break;
            }

            lastIndexToChange = (lastIndexToChange - 1 + cyclesPoints[LMmove[5]].size()) % cyclesPoints[LMmove[5]].size();
            current = (current + 1) % cyclesPoints[LMmove[5]].size();
        }
    }

    return newCyclesPoints;
}

vector<vector<int>> makeMove(const vector<vector<int>>& cyclesPoints, const Move& move)
{
    bool flag = false;

    vector<vector<int>> newCyclesPoints = cyclesPoints;

    if (move.type == VerticesExchange)
    {
        int firstVertexIndex = indexOfPoint(move.first, cyclesPoints[0]);
        int secondVertexIndex = indexOfPoint(move.second, cyclesPoints[1]);

        // Switch the points in cycles
        newCyclesPoints[0][firstVertexIndex] = move.second;
        newCyclesPoints[1][secondVertexIndex] = move.first;
    }
    else
    {
        int cycleSize = cyclesPoints[move.cycle].size();

        int aPointIndex = indexOfPoint(move.first, cyclesPoints[move.cycle]);
        int aPointAfterIndex = indexOfPoint(move.afterFirst, cyclesPoints[move.cycle]);

        int bPointIndex = indexOfPoint(move.second, cyclesPoints[move.cycle]);
        int bPointAfterIndex = indexOfPoint(move.afterSecond, cyclesPoints[move.cycle]);

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

void steepest(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances, const vector<vector<int>>& allPossibleMoves)
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
            cyclesPoints = makeMove2(cyclesPoints, bestMove);
        }
    }
}

vector<int> translateMoveToLM(const vector<int>& move, vector<vector<int>>& cyclesPoints)
{
    if (move[0] == 1)
    {
        return vector<int>{
            move[0],
            cyclesPoints[0][move[1]],
            0,
            cyclesPoints[1][move[2]],
            0,
            0
        };
    }

    return vector<int>{
        move[0],
        cyclesPoints[move[3]][move[1]],
        cyclesPoints[move[3]][(move[1] + 1) % cyclesPoints[move[3]].size()],
        cyclesPoints[move[3]][move[2]],
        cyclesPoints[move[3]][(move[2] + 1) % cyclesPoints[move[3]].size()],
        move[3]
    };
}

int verticesExchangeDelta(const Move& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    int delta = 0;

    int aPoint = move.first;
    int aPointBefore = move.beforeFirst;
    int aPointAfter = move.afterFirst;

    int bPoint = move.second;
    int bPointBefore = move.beforeSecond;
    int bPointAfter = move.afterSecond;

    delta = delta - distances[aPointBefore][aPoint] - distances[aPoint][aPointAfter];
    delta = delta - distances[bPointBefore][bPoint] - distances[bPoint][bPointAfter];
    delta = delta + distances[aPointBefore][bPoint] + distances[aPointAfter][bPoint];
    delta = delta + distances[bPointBefore][aPoint] + distances[bPointAfter][aPoint];

    return delta;
}

int edgesExchangeDelta(const Move& move, const vector<vector<int>>& distances, vector<vector<int>>& cyclesPoints)
{
    int delta = 0;

    int aPoint = move.first;
    int aPointAfter = move.afterFirst;

    int bPoint = move.second;
    int bPointAfter = move.afterSecond;

    if (cycleDirection(aPoint, aPointAfter, cyclesPoints[move.cycle]) == cycleDirection(bPoint, bPointAfter, cyclesPoints[move.cycle]))
    {
        delta = delta - distances[aPoint][aPointAfter] - distances[bPoint][bPointAfter];
        delta = delta + distances[aPoint][bPoint] + distances[aPointAfter][bPointAfter];
    }
    else
    {
        delta = delta - distances[aPoint][aPointAfter] - distances[bPoint][bPointAfter];
        delta = delta + distances[aPoint][bPointAfter] + distances[aPointAfter][bPoint];
    }

    return delta;
}

int computeDelta(const Move& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    if (move.type == VerticesExchange)
        return verticesExchangeDelta(move, cyclesPoints, distances);
    else
        return edgesExchangeDelta(move, distances, cyclesPoints);
}

int findPlaceForMove(vector<pair<Move, int>>& LM, int delta, int left, int right)
{
    int middle = (left + right) / 2;

    if (left >= right)
    {
        if (LM[middle].second > delta)
            return left;
        else
            return right + 1;
    }

    if (LM[middle].second == delta)
        return middle;
    else if (LM[middle].second > delta)
        return findPlaceForMove(LM, delta, left, middle - 1);
    else
        return findPlaceForMove(LM, delta, middle + 1, right);
}

void addMoveToLM(vector<pair<Move, int>>& LM, const Move& move, vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    int delta = computeDelta(move, cyclesPoints, distances);

    if (delta < 0)
    {
        if (LM.size() == 0)
        {
            LM.push_back({move, delta});
        }
        else
        {
            int whereToInsert = findPlaceForMove(LM, delta, 0, LM.size() - 1);
            LM.insert(LM.begin() + whereToInsert, {move, delta});
        }
    }
}

vector<pair<Move, int>> initializeLM(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    vector<pair<Move, int>> LM;

    // Add exchanging vertices a + 1 and b + 1
    for (int a = 0; a < cyclesPoints[0].size(); a++)
    {
        for (int b = 0; b < cyclesPoints[1].size(); b++)
        {
            Move move = {
                VerticesExchange,
                cyclesPoints[0][a],
                cyclesPoints[0][(a + 1) % cyclesPoints[0].size()],
                cyclesPoints[0][(a + 2) % cyclesPoints[0].size()],
                cyclesPoints[1][b],
                cyclesPoints[1][(b + 1) % cyclesPoints[1].size()],
                cyclesPoints[1][(b + 2) % cyclesPoints[1].size()],
                0};
            addMoveToLM(LM, move, cyclesPoints, distances);
        }
    }

    // Add exchanging edges a+1 -> a+2 and b+1 -> b+2
    for(int cycleIndex = 0; cycleIndex < NUM_OF_CYCLES; cycleIndex++)
    {
        for (int a = 0; a < cyclesPoints[0].size(); a++)
        {
            for (int b = 0; b < cyclesPoints[1].size(); b++)
            {
                if (a + 1 != b + 1)
                {
                    Move move = {EdgesExchange,
                        cyclesPoints[cycleIndex][a],
                        cyclesPoints[cycleIndex][(a + 1) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(a + 2) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][b],
                        cyclesPoints[cycleIndex][(b + 1) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(b + 2) % cyclesPoints[cycleIndex].size()],
                        cycleIndex};
                    addMoveToLM(LM, move, cyclesPoints, distances);

                    Move invertedEdgeMove = {EdgesExchange,
                        cyclesPoints[cycleIndex][a],
                        cyclesPoints[cycleIndex][(a + 1) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(a + 2) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(b + 3) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(b + 2) % cyclesPoints[cycleIndex].size()],
                        cyclesPoints[cycleIndex][(b + 1) % cyclesPoints[cycleIndex].size()],
                        cycleIndex};
                    addMoveToLM(LM, invertedEdgeMove, cyclesPoints, distances);
                }
            }
        }
    }

    return LM;
}

ApplicabilityStatus getApplicabilityStatus(const Move& move, vector<vector<int>>& cyclesPoints)
{
    if (move.type == VerticesExchange)
    {
        // Check if vertices are in correct cycles
        int aPointBeforeIndex = indexOfPoint(move.beforeFirst, cyclesPoints[0]);
        int aPointIndex = indexOfPoint(move.first, cyclesPoints[0]);
        int aPointAfterIndex = indexOfPoint(move.afterFirst, cyclesPoints[0]);

        int bPointBeforeIndex = indexOfPoint(move.beforeSecond, cyclesPoints[1]);
        int bPointIndex = indexOfPoint(move.second, cyclesPoints[1]);
        int bPointAfterIndex = indexOfPoint(move.afterSecond, cyclesPoints[1]);

        // Points are not in the correct cycles
        if (aPointBeforeIndex == cyclesPoints[0].size()
            || aPointIndex == cyclesPoints[0].size()
            || aPointAfterIndex == cyclesPoints[0].size()
            || bPointBeforeIndex == cyclesPoints[1].size()
            || bPointIndex == cyclesPoints[1].size()
            || bPointAfterIndex == cyclesPoints[1].size())
        {
            return InApplicable;
        }

        // Points are in the same order
        if ((((aPointBeforeIndex + 1) % cyclesPoints[0].size() == aPointIndex)
            && ((aPointIndex + 1) % cyclesPoints[0].size() == aPointAfterIndex)
            && ((bPointBeforeIndex + 1) % cyclesPoints[1].size() == bPointIndex)
            && ((bPointIndex + 1) % cyclesPoints[1].size() == bPointAfterIndex)))
        {
            return Applicable;
        }

        // Points are in different order - delta changed
        return InApplicable;

    }
    else
    {
        int cycleSize = cyclesPoints[move.cycle].size();

        int aPointIndex = indexOfPoint(move.first, cyclesPoints[move.cycle]);
        int aPointAfterIndex = indexOfPoint(move.afterFirst, cyclesPoints[move.cycle]);

        int bPointIndex = indexOfPoint(move.second, cyclesPoints[move.cycle]);
        int bPointAfterIndex = indexOfPoint(move.afterSecond, cyclesPoints[move.cycle]);

        // Points are not in the correct cycles
        if (aPointIndex == cycleSize
            || aPointAfterIndex == cycleSize
            || bPointIndex == cycleSize
            || bPointAfterIndex == cycleSize)
        {
            return InApplicable;
        }

        // Edges are in the same order
        if (((aPointIndex + 1) % cycleSize == aPointAfterIndex)
            && ((bPointIndex + 1) % cycleSize == bPointAfterIndex))
        {
            return Applicable;
        }

        // Edges are in reversed order
        if ((((aPointIndex + 1) % cycleSize == aPointAfterIndex)
                && ((bPointAfterIndex + 1) % cycleSize == bPointIndex))
            || (((aPointAfterIndex + 1) % cycleSize == aPointIndex)
                && ((bPointIndex + 1) % cycleSize == bPointAfterIndex)))
        {
            return MaybeApplicable;
        }

        return InApplicable;
    }

}

void addNewMoves(vector<pair<Move, int>>& LM, vector<vector<int>>& cyclesPoints,
    const Move& lastMove, const vector<vector<int>>& distances)
{
    Move newMove;
    if (lastMove.type == VerticesExchange)
    {
        for (int i = 0; i < cyclesPoints[0].size(); i++)
        {
            int beforeBeforeFirstIndex = (indexOfPoint(lastMove.beforeFirst, cyclesPoints[0]) - 1 + cyclesPoints[0].size()) % cyclesPoints[0].size();
            int beforeBeforeFirst = cyclesPoints[0][beforeBeforeFirstIndex];
            int beforeBeforeSecondIndex = (indexOfPoint(lastMove.beforeSecond, cyclesPoints[1]) - 1 + cyclesPoints[1].size()) % cyclesPoints[1].size();
            int beforeBeforeSecond = cyclesPoints[1][beforeBeforeSecondIndex];
            // Add moves that exchange newly added edges with every other one in first cycle
            if (cyclesPoints[0][(i + 1) % cyclesPoints[0].size()] != lastMove.beforeFirst)
            {
                // Create new move
                newMove = {EdgesExchange,
                    beforeBeforeFirst,
                    lastMove.beforeFirst,
                    lastMove.second,
                    cyclesPoints[0][i],
                    cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                    0};
                addMoveToLM(LM, newMove, cyclesPoints, distances);

                // Create one for inverted order of edges
                newMove = {EdgesExchange,
                    beforeBeforeFirst,
                    lastMove.beforeFirst,
                    lastMove.second,
                    cyclesPoints[0][(i + 3) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                    0};
                addMoveToLM(LM, newMove, cyclesPoints, distances);
            }

            if (cyclesPoints[0][(i + 1) % cyclesPoints[0].size()] != lastMove.second)
            {
                // Create new move
                newMove = {EdgesExchange,
                    lastMove.beforeFirst,
                    lastMove.second,
                    lastMove.afterFirst,
                    cyclesPoints[0][i],
                    cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                    0};
                addMoveToLM(LM, newMove, cyclesPoints, distances);

                // Create one for inverted order of edges
                newMove = {EdgesExchange,
                    lastMove.beforeFirst,
                    lastMove.second,
                    lastMove.afterFirst,
                    cyclesPoints[0][(i + 3) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                    cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                    0};
                addMoveToLM(LM, newMove, cyclesPoints, distances);
            }

            // Add moves that exchange newly added edges with every other one in second cycle
            if (cyclesPoints[1][(i + 1) % cyclesPoints[1].size()] != lastMove.beforeSecond)
            {
                // Create new move
                newMove = {EdgesExchange,
                    beforeBeforeSecond,
                    lastMove.beforeSecond,
                    lastMove.first,
                    cyclesPoints[1][i],
                    cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                    1};
                addMoveToLM(LM, newMove, cyclesPoints, distances);

                // Create one for inverted order of edges
                newMove = {EdgesExchange,
                    beforeBeforeSecond,
                    lastMove.beforeSecond,
                    lastMove.first,
                    cyclesPoints[1][(i + 3) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                    1};
                addMoveToLM(LM, newMove, cyclesPoints, distances);
            }

            if (cyclesPoints[1][(i + 1) % cyclesPoints[1].size()] != lastMove.first)
            {
                // Create new move
                newMove = {EdgesExchange,
                    lastMove.beforeSecond,
                    lastMove.first,
                    lastMove.afterSecond,
                    cyclesPoints[1][i],
                    cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                    1};
                addMoveToLM(LM, newMove, cyclesPoints, distances);

                // Create one for inverted order of edges
                newMove = {EdgesExchange,
                    lastMove.beforeSecond,
                    lastMove.first,
                    lastMove.afterSecond,
                    cyclesPoints[1][(i + 3) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                    cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                    1};
                addMoveToLM(LM, newMove, cyclesPoints, distances);
            }

            // Add moves that exchange modified vertices
            newMove = {VerticesExchange,
                lastMove.beforeSecond,
                lastMove.first,
                lastMove.afterSecond,
                cyclesPoints[0][i],
                cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                0};
            addMoveToLM(LM, newMove, cyclesPoints, distances);

            newMove = {VerticesExchange,
                lastMove.beforeFirst,
                lastMove.second,
                lastMove.afterFirst,
                cyclesPoints[1][i],
                cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                0};
            addMoveToLM(LM, newMove, cyclesPoints, distances);
        }
    }
    else
    {
        for (int i = 0; i < cyclesPoints[lastMove.cycle].size(); i++)
        {
            // Add new edges exchange moves
            // If the cycle direction is first -> second
            if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == 1)
            {
                if (cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()] != lastMove.first)
                {
                    // Create new move
                    newMove = {EdgesExchange,
                        lastMove.beforeFirst,
                        lastMove.first,
                        lastMove.second,
                        cyclesPoints[lastMove.cycle][i],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    // Create one for inverted order of edges
                    newMove = {EdgesExchange,
                        lastMove.beforeFirst,
                        lastMove.first,
                        lastMove.second,
                        cyclesPoints[lastMove.cycle][(i + 3) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }

                // Add moves that exchange second added edge with every other one
                if (cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()] != lastMove.afterFirst)
                {
                    int afterAfterFirstIndex = (indexOfPoint(lastMove.afterFirst, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterFirst = cyclesPoints[lastMove.cycle][afterAfterFirstIndex];
                    // Create new move
                    newMove = {EdgesExchange,
                        afterAfterFirst,
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        cyclesPoints[lastMove.cycle][i],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    // Create one for inverted order of edges
                    newMove = {EdgesExchange,
                        afterAfterFirst,
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        cyclesPoints[lastMove.cycle][(i + 3) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
            }
            else if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == -1)
            {
                if (cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()] != lastMove.second)
                {
                    // Create new move
                    newMove = {EdgesExchange,
                        lastMove.beforeSecond,
                        lastMove.second,
                        lastMove.first,
                        cyclesPoints[lastMove.cycle][i],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    // Create one for inverted order of edges
                    newMove = {EdgesExchange,
                        lastMove.beforeSecond,
                        lastMove.second,
                        lastMove.first,
                        cyclesPoints[lastMove.cycle][(i + 3) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }

                if (cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()] != lastMove.afterSecond)
                {
                    int beforeAfterSecondIndex = (indexOfPoint(lastMove.afterSecond, cyclesPoints[lastMove.cycle])
                        - 1 + cyclesPoints[lastMove.cycle].size()) % cyclesPoints[lastMove.cycle].size();
                    int beforeAfterSecond = cyclesPoints[lastMove.cycle][beforeAfterSecondIndex];
                    // Create new move
                    newMove = {EdgesExchange,
                        beforeAfterSecond,
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        cyclesPoints[lastMove.cycle][i],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    // Create one for inverted order of edges
                    newMove = {EdgesExchange,
                        beforeAfterSecond,
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        cyclesPoints[lastMove.cycle][(i + 3) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 2) % cyclesPoints[lastMove.cycle].size()],
                        cyclesPoints[lastMove.cycle][(i + 1) % cyclesPoints[lastMove.cycle].size()],
                        lastMove.cycle};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
            }

            // Add moves that use vertices that were part of the edges
            if (lastMove.cycle == 0)
            {
                if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == 1)
                {
                    newMove = {VerticesExchange,
                        lastMove.beforeFirst,
                        lastMove.first,
                        lastMove.second,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterFirstIndex = (indexOfPoint(lastMove.afterFirst, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterFirst = cyclesPoints[lastMove.cycle][afterAfterFirstIndex];

                    newMove = {VerticesExchange,
                        afterAfterFirst,
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    newMove = {VerticesExchange,
                        lastMove.first,
                        lastMove.second,
                        lastMove.beforeSecond,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterSecondIndex = (indexOfPoint(lastMove.afterSecond, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterSecond = cyclesPoints[lastMove.cycle][afterAfterSecondIndex];

                    newMove = {VerticesExchange,
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        afterAfterSecond,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
                else if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == -1)
                {
                    newMove = {VerticesExchange,
                        lastMove.second,
                        lastMove.first,
                        lastMove.beforeFirst,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterFirstIndex = (indexOfPoint(lastMove.afterFirst, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterFirst = cyclesPoints[lastMove.cycle][afterAfterFirstIndex];

                    newMove = {VerticesExchange,
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        afterAfterFirst,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    newMove = {VerticesExchange,
                        lastMove.beforeSecond,
                        lastMove.second,
                        lastMove.first,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterSecondIndex = (indexOfPoint(lastMove.afterSecond, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterSecond = cyclesPoints[lastMove.cycle][afterAfterSecondIndex];

                    newMove = {VerticesExchange,
                        afterAfterSecond,
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        cyclesPoints[1][i],
                        cyclesPoints[1][(i + 1) % cyclesPoints[1].size()],
                        cyclesPoints[1][(i + 2) % cyclesPoints[1].size()],
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
            }
            else
            {
                if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == 1)
                {
                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.beforeFirst,
                        lastMove.first,
                        lastMove.second,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterFirstIndex = (indexOfPoint(lastMove.afterFirst, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterFirst = cyclesPoints[lastMove.cycle][afterAfterFirstIndex];

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        afterAfterFirst,
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.first,
                        lastMove.second,
                        lastMove.beforeSecond,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterSecondIndex = (indexOfPoint(lastMove.afterSecond, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterSecond = cyclesPoints[lastMove.cycle][afterAfterSecondIndex];

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.afterFirst,
                        lastMove.afterSecond,
                        afterAfterSecond,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
                else if (cycleDirection(lastMove.first, lastMove.second, cyclesPoints[lastMove.cycle]) == -1)
                {
                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.second,
                        lastMove.first,
                        lastMove.beforeFirst,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterFirstIndex = (indexOfPoint(lastMove.afterFirst, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterFirst = cyclesPoints[lastMove.cycle][afterAfterFirstIndex];

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        afterAfterFirst,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        lastMove.beforeSecond,
                        lastMove.second,
                        lastMove.first,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);

                    int afterAfterSecondIndex = (indexOfPoint(lastMove.afterSecond, cyclesPoints[lastMove.cycle]) + 1) % cyclesPoints[lastMove.cycle].size();
                    int afterAfterSecond = cyclesPoints[lastMove.cycle][afterAfterSecondIndex];

                    newMove = {VerticesExchange,
                        cyclesPoints[0][i],
                        cyclesPoints[0][(i + 1) % cyclesPoints[0].size()],
                        cyclesPoints[0][(i + 2) % cyclesPoints[0].size()],
                        afterAfterSecond,
                        lastMove.afterSecond,
                        lastMove.afterFirst,
                        0};
                    addMoveToLM(LM, newMove, cyclesPoints, distances);
                }
            }
        }
    }
}

void pastMoves(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances)
{
    // Initialize the LM vector
    vector<pair<Move, int>> LM = initializeLM(cyclesPoints, distances);

    // Main loop - stops when there is no applicable move
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;
        int moveIndex = 0;
        while (moveIndex < LM.size())
        {
            Move move = LM[moveIndex].first;
            int applicabilityStatus = getApplicabilityStatus(move, cyclesPoints);

            if (applicabilityStatus == Applicable)
            {
                // Make move and erase it from LM
                cyclesPoints = makeMove(cyclesPoints, move);
                LM.erase(LM.begin() + moveIndex);

                // Add new moves to LM
                addNewMoves(LM, cyclesPoints, move, distances);

                stopCondition = false;
                break;

            }
            else if (applicabilityStatus == MaybeApplicable)
            {
                moveIndex++;
            }
            else if (applicabilityStatus == InApplicable)
            {
                LM.erase(LM.begin() + moveIndex);
            }
        }
    }
}

bool comparePairs(const pair<int, int>& a, const pair<int, int>& b)
{
    return a.first < b.first;
}

vector<vector<int>> findClosestNeighbors(const vector<vector<int>>& distances, int k)
{
    int n = distances.size();
    vector<vector<int>> closest(n);

    for (int i = 0; i < n; i++)
    {
        vector<pair<int, int>> distIndexPairs;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                distIndexPairs.push_back(make_pair(distances[i][j], j));
            }
        }

        // Sort the pairs based on distance
        sort(distIndexPairs.begin(), distIndexPairs.end(), comparePairs);

        // Take k closest neighbors
        for (int j = 0; j < k && j < distIndexPairs.size(); j++)
        {
            closest[i].push_back(distIndexPairs[j].second);
        }
    }

    return closest;
}

void candidates(vector<vector<int>>& cyclesPoints, const vector<vector<int>>& distances, const vector<vector<int>>& closestNeighbors)
{
    bool stopCondition = false;
    while (!stopCondition)
    {
        stopCondition = true;
        int bestDelta = BIG_M;
        vector<int> bestMove;
        for (int point = 0; point < distances.size(); point++)
        {
            for (int neighbor = 0; neighbor < closestNeighbors[point].size(); neighbor++)
            {
                int firstInFirst = find(cyclesPoints[0].begin(), cyclesPoints[0].end(), point) - cyclesPoints[0].begin();
                int secondInFirst = find(cyclesPoints[0].begin(), cyclesPoints[0].end(), closestNeighbors[point][neighbor]) - cyclesPoints[0].begin();
                int firstInSecond = find(cyclesPoints[1].begin(), cyclesPoints[1].end(), point) - cyclesPoints[1].begin();
                int secondInSecond = find(cyclesPoints[1].begin(), cyclesPoints[1].end(), closestNeighbors[point][neighbor]) - cyclesPoints[1].begin();
                bool firstPointInFirstCycle = firstInFirst < cyclesPoints[0].size();
                bool secondPointInFirstCycle = secondInFirst < cyclesPoints[0].size();

                vector<int> move;
                int delta;
                if (firstPointInFirstCycle && secondPointInFirstCycle)
                {
                    // Exchanging edges in first cycle
                    move = {2, firstInFirst, secondInFirst, 0};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }

                    move = {2,
                        int((firstInFirst - 1 + cyclesPoints[0].size()) % cyclesPoints[0].size()),
                        int((secondInFirst- 1 + cyclesPoints[0].size()) % cyclesPoints[0].size()),
                        0};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }

                }
                else if ((!firstPointInFirstCycle) && (!secondPointInFirstCycle))
                {
                    // Exchanging edges in second cycle
                    move = {2, firstInSecond, secondInSecond, 1};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }

                    move = {2,
                        int((firstInSecond - 1 + cyclesPoints[1].size()) % cyclesPoints[1].size()),
                        int((secondInSecond - 1 + cyclesPoints[1].size()) % cyclesPoints[1].size()),
                        1};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }

                }
                else if (firstPointInFirstCycle && !secondPointInFirstCycle)
                {
                    // Exchanging vertices (first point in first cycle, second in second)
                    move = {1, firstInFirst, secondInSecond, 0};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }

                }
                else if (!firstPointInFirstCycle && secondPointInFirstCycle)
                {
                    // Exchanging vertices (first point in second cycle, second in first)
                    move = {1, firstInSecond, secondInFirst, 0};
                    delta = computeDeltaBasedOnMove(move, cyclesPoints, distances);
                    if ((delta < 0) && (delta < bestDelta))
                    {
                        bestMove = move;
                        bestDelta = delta;
                        stopCondition = false;
                    }
                }
            }
        }

        if (!stopCondition)
        {
            cyclesPoints = makeLMMove(cyclesPoints, translateMoveToLM(bestMove, cyclesPoints));
        }
    }
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

        if (score <= bestResult)
        {
            bestCycles = results[res];
            bestResult = score;
        }
        else if (score >= worstResult)
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
    if (argc != 6)
    {
        cerr << "Usage: " << argv[0] << " <input_filename> <pastMoves|candidates|steepest|regret>";
        cerr << " <output_filename_1> <output_filename_2> <number_of_neighbors>" << endl;
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
            vector<vector<int>> closestNeighbors = findClosestNeighbors(distances, atoi(argv[5]));
            candidates(cyclesPoints, distances, closestNeighbors);
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
