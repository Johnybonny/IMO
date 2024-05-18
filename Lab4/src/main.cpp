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

vector<vector<int>> multipleStartLocalSearch(const vector<vector<int>>& distances, const vector<IndexedMove>& possibleMoves)
{
    vector<vector<int>> bestCyclesPoints{};
    int bestScore = BIG_M;
    for (int i = 0; i < 100; i++)
    {
        cout << i << "\n";
        // Create random cycles
        vector<vector<int>> cyclesPoints = randomCycle(distances);

        // Run steepest local search algorithm
        steepest(cyclesPoints, distances, possibleMoves);
        int score = getLengthBasedOnPoints(cyclesPoints[0], distances) + getLengthBasedOnPoints(cyclesPoints[1], distances);

        // Remember the best score
        if (score < bestScore)
        {
            bestScore = score;
            bestCyclesPoints = cyclesPoints;
        }
    }
    return bestCyclesPoints;
}

void iteratedLocalSearch()
{
    cout << "Iterated local search\n";
}

void largeScaleNeighborhoodSearch()
{
    cout << "Large-scale neighborhood search\n";
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0] << " <input_filename> <multiple|iterated|large-scale>";
        cerr << " <output_filename_1> <output_filename_2>" << endl;
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
    srand(time(NULL));

    vector<IndexedMove> allPossibleMoves = generateMoves(distances.size() / 2, distances);
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++)
    {
        cout << (iteration + 1) * (100.0 / NUM_OF_ITERATIONS) << "%\n";

        // Start algorithm
        vector<vector<int>> cyclesPoints;
        chrono::steady_clock::time_point beginTimeMeasurement = chrono::steady_clock::now();
        if (string(argv[2]) == "multiple")
        {
            cyclesPoints = multipleStartLocalSearch(distances, allPossibleMoves);
        }
        else if (string(argv[2]) == "iterated")
        {
            iteratedLocalSearch();
        }
        else if (string(argv[2]) == "large-scale")
        {
            largeScaleNeighborhoodSearch();
        }
        else
        {
            cerr << argv[2] << " can be 'multiple', 'iterated' or 'large-scale'" << endl;
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
