#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

double* x;
double* y;

int n;
int mapSize;

struct Candidate
{
	vector<int> genes;
	double cost = 0;

	void calculateCost()
	{
		for (int i = 0; i < genes.size() - 1; i++)
		{
			int a = genes[i];
			int b = genes[i + 1];
			cost += sqrt((x[a] - x[b]) * (x[a] - x[b]) + (y[a] - y[b]) * (y[a] - y[b]));
		}
	}

	friend bool operator>(Candidate& i1, Candidate& i2)
	{
		return i1.cost > i2.cost;
	}
};

struct Compare
{
	bool operator()(Candidate& lhs, Candidate& rhs)
	{
		return lhs > rhs;
	}
};

priority_queue<Candidate, vector<Candidate>, Compare> currentGeneration;
priority_queue<Candidate, vector<Candidate>, Compare> nextGeneration;

void mutate(Candidate& i)
{
	int randomGene1 = rand() % n;
	int randomGene2 = rand() % n;

	swap(i.genes[randomGene1], i.genes[randomGene2]);
}

void inheritTheRestOfGenesFromParent(Candidate& child, Candidate& parent, int limit)
{
	bool isGenInChild;
	int iter = 0;
	int nextGen;

	for (int i = limit + 1; i < n; i++) {
		isGenInChild = true;
		while (isGenInChild)
		{
			nextGen = parent.genes[iter];
			isGenInChild = false;
			for (int j = 0; j < child.genes.size(); j++)
			{
				if (child.genes[j] == nextGen)
				{
					isGenInChild = true;
					iter++;
					break;
				}
			}
		}
		child.genes.push_back(nextGen);
	}
}

void crossover(Candidate& parent1, Candidate& parent2) {
	Candidate child1, child2;

	int limit = rand() % n;

	for (int i = 0; i <= limit; i++) {
		child1.genes.push_back(parent1.genes[i]);
		child2.genes.push_back(parent2.genes[i]);
	}

	inheritTheRestOfGenesFromParent(child1, parent2, limit);
	inheritTheRestOfGenesFromParent(child2, parent1, limit);

	mutate(child1);
	mutate(child2);

	child1.calculateCost();
	child2.calculateCost();

	nextGeneration.push(child1);
	nextGeneration.push(child2);
}

void reproduce()
{
	Candidate parent1, parent2;
	int generationSize = currentGeneration.size();

	while (currentGeneration.size() > generationSize / 2)
	{
		parent1 = currentGeneration.top();
		currentGeneration.pop();

		parent2 = currentGeneration.top();
		currentGeneration.pop();

		crossover(parent1, parent2);

		nextGeneration.push(parent1);
		nextGeneration.push(parent2);
	}

	while (!currentGeneration.empty()) {
		currentGeneration.pop();
	}
}

void generateCities()
{
	for (int i = 0; i < n; i++)
	{
		x[i] = rand() % mapSize - mapSize / 2;
		y[i] = rand() % mapSize - mapSize / 2;
	}
}

void initializeNextGeneration() {
	currentGeneration = nextGeneration;

	while (!nextGeneration.empty())
	{
		nextGeneration.pop();
	}
}

int main()
{
	cin >> n;

	auto startTime = steady_clock::now();

	mapSize = 4 * sqrt(n);

	x = new double[n];
	y = new double[n];

	generateCities();

	int POPULATION_SIZE = 1000;

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		Candidate candidate;
		for (int j = 0; j < n; j++)
		{
			candidate.genes.push_back(j);
		}
		std::random_shuffle(candidate.genes.begin(), candidate.genes.end());
		candidate.calculateCost();
		currentGeneration.push(candidate);
	}

	int iter = 0;
	int countSameBestCost = 0;
    double bestCost = 0;

	while (true) {
        if (bestCost == currentGeneration.top().cost)
        {
            countSameBestCost++;
        }
        else
        {
            countSameBestCost = 0;
        }

        bestCost = currentGeneration.top().cost;
        if (countSameBestCost > 1000)
        {
            cout << iter << ": " << "Best cost: " << bestCost << endl;
            break;
        }

		if (iter == 1 || iter == 10 || iter == 100 || iter == 1000)
		{
			cout << iter << ": " << "Current best cost: " << bestCost << endl;
		}

		reproduce();
		initializeNextGeneration();
		iter++;
	}

	delete[] x, y;

	auto endTime = steady_clock::now();
	duration<double> elapsedTime = endTime - startTime;
    cout << "Elapsed time: " << elapsedTime.count() << endl;

	return 0;
}
