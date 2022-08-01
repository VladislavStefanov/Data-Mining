#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>

using namespace std;
using namespace chrono;

struct Node
{
    int** board;
    int dimension;
    int blankTileX;
    int blankTileY;
    int blankPositionInSolution;
    string lastMove;

    bool isSolved()
    {
        int blankPositionInSolutionY = blankPositionInSolution / dimension;
        int blankPositionInSolutionX = blankPositionInSolution % dimension;
        if (board[blankPositionInSolutionY][blankPositionInSolutionX] != 0)
        {
            return false;
        }

        for (int i = 0; i < dimension; i++)
        {
            for(int j = 0; j < dimension; j++)
            {
                bool isPastTheZero = (i * dimension) + j > blankPositionInSolution;
                if (board[i][j] != 0 && board[i][j] != (i * dimension) + j + 1 * !isPastTheZero)
                {
                    return false;
                }
            }
        }
        return true;
    }

    Node(int** board, int dimension, int blankTileX, int blankTileY, int blankPositionInSolution)
    {
        this->board = board;
        this->dimension = dimension;
        this->blankTileX = blankTileX;
        this->blankTileY = blankTileY;
        this->blankPositionInSolution = blankPositionInSolution;
        this->lastMove = "";
    }

    Node(const Node& otherNode)
    {
        dimension = otherNode.dimension;
        blankTileX = otherNode.blankTileX;
        blankTileY = otherNode.blankTileY;
        blankPositionInSolution = otherNode.blankPositionInSolution;
        lastMove = otherNode.lastMove;

        board = new int*[dimension];
        for(int i = 0; i < dimension; i++)
        {
            board[i] = new int[dimension];
            for(int j = 0; j < dimension; j++)
            {
                board[i][j] = otherNode.board[i][j];
            }
        }
    }

    Node& operator=(const Node& otherNode)
    {
        dimension = otherNode.dimension;
        blankTileX = otherNode.blankTileX;
        blankTileY = otherNode.blankTileY;
        blankPositionInSolution = otherNode.blankPositionInSolution;
        lastMove = otherNode.lastMove;

        board = new int*[dimension];
        for(int i = 0; i < dimension; i++)
        {
            board[i] = new int[dimension];
            for(int j = 0; j < dimension; j++)
            {
                board[i][j] = otherNode.board[i][j];
            }
        }

        return *this;
    }

    ~Node()
    {
        for (int i = 0; i < dimension; i++) {
            delete[] board[i];
        }
        delete[] board;
    }

    Node slideLeft()
    {
        Node newNode = *this;
        swap(newNode.board[blankTileY][blankTileX], newNode.board[blankTileY][blankTileX + 1]);
        newNode.blankTileX = blankTileX + 1;
        newNode.lastMove = "left";
        return newNode;
    }

    Node slideRight()
    {
        Node newNode = *this;
        swap(newNode.board[blankTileY][blankTileX], newNode.board[blankTileY][blankTileX - 1]);
        newNode.blankTileX = blankTileX - 1;
        newNode.lastMove = "right";
        return newNode;
    }

    Node slideUp()
    {
        Node newNode = *this;
        swap(newNode.board[blankTileY][blankTileX], newNode.board[blankTileY + 1][blankTileX]);
        newNode.blankTileY = blankTileY + 1;
        newNode.lastMove = "up";
        return newNode;
    }

    Node slideDown()
    {
        Node newNode = *this;
        swap(newNode.board[blankTileY][blankTileX], newNode.board[blankTileY - 1][blankTileX]);
        newNode.blankTileY = blankTileY - 1;
        newNode.lastMove = "down";
        return newNode;
    }

    bool isSlideLeftPossible()
    {
        return blankTileX < dimension - 1;
    }

    bool isSlideRightPossible()
    {
        return blankTileX > 0;
    }

    bool isSlideUpPossible()
    {
        return blankTileY < dimension - 1;
    }

    bool isSlideDownPossible()
    {
        return blankTileY > 0;
    }

    vector<Node> successors()
    {
        vector<Node> successorsVector;
        if (isSlideLeftPossible())
        {
            successorsVector.push_back(slideLeft());
        }

        if (isSlideRightPossible())
        {
            successorsVector.push_back(slideRight());
        }

        if (isSlideUpPossible())
        {
            successorsVector.push_back(slideUp());
        }

        if (isSlideDownPossible())
        {
            successorsVector.push_back(slideDown());
        }

        return successorsVector;
    }

    int estimatedCheapestPathCost()
    {
        int cost = 0;
        for(int i = 0; i < dimension; i++)
        {
            for(int j = 0; j < dimension; j++)
            {
                if (board[i][j] != 0)
                {
                    bool isPastTheZero = board[i][j] > blankPositionInSolution;
                    cost += abs(i - (board[i][j] - 1 * !isPastTheZero) / dimension) + abs(j - (board[i][j] - 1 * !isPastTheZero) % dimension);
                }
            }
        }

        return cost;
    }
};

bool operator== (const Node& node, const Node& otherNode)
    {
        for(int i = 0; i < node.dimension; i++)
        {
            for(int j = 0; j < node.dimension; j++)
            {
                if (node.board[i][j] != otherNode.board[i][j])
                {
                    return false;
                }
            }
        }

        return true;
    }

bool isInPath(vector<Node>& path, const Node& node)
{
    for (Node& pathNode : path)
    {
        if (pathNode == node)
        {
            return true;
        }
    }

    return false;
}

int search(vector<Node>& path, int costToCurrentNode, int bound)
{
    Node& node = path.back();

    int estimatedCheapestPathCostFromBeginning = costToCurrentNode + node.estimatedCheapestPathCost();
    if (estimatedCheapestPathCostFromBeginning > bound)
    {
        return estimatedCheapestPathCostFromBeginning;
    }

    if (node.isSolved())
    {
        return 0;
    }

    int minCost = INT_MAX;
    vector<Node> successors = node.successors();
    for (Node& successor : successors)
    {
        if (!isInPath(path, successor))
        {
            path.push_back(successor);

            int lastCostOverBound = search(path, costToCurrentNode + 1, bound);
            if (lastCostOverBound == 0)
            {
                return 0;
            }

            if (lastCostOverBound < minCost)
            {
                minCost = lastCostOverBound;
            }

            path.pop_back();
        }
    }

    return minCost;
}

vector<string> getDirectionsFromPath(vector<Node>& path)
{
    vector<string> directions;

    for (int i = 1; i < path.size(); i++)
    {
        directions.push_back(path[i].lastMove);
    }

    return directions;
}

int countInversions(Node& root)
{
    int inversions = 0;

    for (int i = 0; i < root.dimension * root.dimension; i++)
    {
        for (int j = i + 1; j < root.dimension * root.dimension; j++)
        {
            int y1 = i / root.dimension;
            int x1 = i % root.dimension;
            int y2 = j / root.dimension;
            int x2 = j % root.dimension;
            if (root.board[y1][x1] != 0 && root.board[y2][x2] != 0 && root.board[y1][x1] > root.board[y2][x2])
            {
                inversions++;
            }
        }
    }

    return inversions;
}

bool isSolvable(Node& root)
{
    int inversions = countInversions(root);

    if (root.dimension % 2 == 1 && inversions % 2 == 0)
    {
        return true;
    }

    if (root.dimension % 2 == 0 && (inversions + root.blankTileY) % 2 == 1)
    {
        return true;
    }

    return false;
}

vector<string> findShortestSolution(Node& root)
{
    if (!isSolvable(root))
    {
        return vector<string>();
    }

    int bound = root.estimatedCheapestPathCost();
    vector<Node> path;
    path.push_back(root);

    while(true)
    {
        int lastCostOverBound = search(path, 0, bound);
        if (lastCostOverBound == 0)
        {
            return getDirectionsFromPath(path);
        }

        bound = lastCostOverBound;
    }
}



int main()
{
    int tileCount;
    cin >> tileCount;

    int blankPositionInSolution;
    cin >> blankPositionInSolution;

    if (blankPositionInSolution < 0)
    {
        blankPositionInSolution = tileCount;
    }

    int dimension = sqrt(tileCount + 1);

    int** board = new int*[dimension];

    int blankTileX = 0;
    int blankTileY = 0;
    for (int i = 0; i < dimension; i++)
    {
        board[i] = new int[dimension];
        for (int j = 0; j < dimension; j++)
        {
            cin >> board[i][j];
            if (board[i][j] == 0)
            {
                blankTileY = i;
                blankTileX = j;
            }
        }
    }


    auto startTime = steady_clock::now();
    Node root (board, dimension, blankTileX, blankTileY, blankPositionInSolution);
    time_point<steady_clock> endTime;
    if (root.isSolved())
    {
        endTime = steady_clock::now();
        cout << 0 << endl;
    }
    else
    {
        vector<string> solution = findShortestSolution(root);
        endTime = steady_clock::now();
        if (solution.empty())
        {
            cout << "Unsolvable" << endl;
        }
        else
        {
            cout << solution.size() << endl;
            for (int i = 0; i < solution.size(); i++)
            {
                cout << solution[i] << endl;
            }
        }
    }


    duration<double> elapsedTime = endTime - startTime;

    cout << elapsedTime.count() << endl;

    return 0;
}
