#include<iostream>
#include<vector>
#include <stdlib.h>
#include <chrono>

using namespace std;
using namespace chrono;

int timesAttacked(int* queenPositions, int n, int x, int y)
{
    int countAttacked = 0;

    for (int i = 0; i < n; i++)
    {
        if (i == x)
        {
            continue;
        }
        if(queenPositions[i] == y || abs(i - x) == abs(queenPositions[i] - y))
        {
            countAttacked++;
        }
    }

    return countAttacked;
}

void minConflicts(int* queenPositions, int n)
{
    int* conflicts = new int[n];
    int* potentialConflicts = new int[n];
    vector<int> queensWithConflicts;
    vector<int> candidates;

    for (int i = 0; i < 1000; i++)
    {
        int totalConflicts = 0;
        for (int j = 0; j < n; j++)
        {
            conflicts[j] = timesAttacked(queenPositions, n, j, queenPositions[j]);
            totalConflicts += conflicts[j];
        }

        if (totalConflicts == 0)
        {
            return;
        }

        for (int j = 0; j < n; j++)
        {
            if (conflicts[j] > 0)
            {
                queensWithConflicts.push_back(j);
            }
        }

        int x = queensWithConflicts[(rand() % queensWithConflicts.size())];

        queensWithConflicts.clear();

        int minConflicts = n + 1;
        for (int j = 0; j < n; j++)
        {
            potentialConflicts[j] = timesAttacked(queenPositions, n, x, j);
            minConflicts = min(minConflicts, potentialConflicts[j]);
        }

        for (int j = 0; j < n; j++)
        {
            if (minConflicts == potentialConflicts[j])
            {
                candidates.push_back(j);
            }
        }

        int y = candidates[rand() % candidates.size()];
        queenPositions[x] = y;

        candidates.clear();
    }

    delete[] conflicts;
    delete[] potentialConflicts;
}

int main()
{
    int n;
    cin >> n;

    auto startTime = steady_clock::now();

    int* queenPositions = new int[n];
    for (int i = 0; i < n; i++)
    {
        queenPositions[i] = i;
    }

    minConflicts(queenPositions, n);

    auto endTime = steady_clock::now();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (queenPositions[j] == i)
            {
                cout << "* ";
            }
            else
            {
                cout << "_ ";
            }
        }
        cout << endl;
    }

    duration<double> elapsedTime = endTime - startTime;
    cout << elapsedTime.count() << endl;

    delete[] queenPositions;
    return 0;
}
