#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <set>

using namespace std;

struct Point
{
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}
};

vector<Point> points;
vector<Point> centroids;
vector<vector<Point>> clusters;
double minX, maxX, minY, maxY;

void initCentroids(int k)
{
    srand(time(0));
    for (int i = 0; i < k; i++)
    {
        double x = (rand() % 1000) / 1000.0 * (maxX - minX) + minX;
        double y = (rand() % 1000) / 1000.0 * (maxY - minY) + minY;
        centroids.push_back({x, y});
    }
}

double squaredDistance(Point& p1, Point& p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

void assignToClusters()
{
    for (Point& point : points)
    {
        double minDistance = squaredDistance(point, centroids[0]);
        double closestCentroidIndex = 0;
        for (int i = 1; i < centroids.size(); i++)
        {
            double distance = squaredDistance(point, centroids[i]);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestCentroidIndex = i;
            }
        }

        clusters[closestCentroidIndex].push_back(point);
    }
}

bool updateCentroid()
{
    bool hasChanged = false;
    for (int i = 0; i < centroids.size(); i++)
    {
        if (clusters[i].size() == 0)
        {
            continue;
        }

        double sumX = 0, sumY = 0;
        for (int j = 0; j < clusters[i].size(); j++)
        {
            sumX += clusters[i][j].x;
            sumY += clusters[i][j].y;
        }

        Point newCentroid = {sumX / clusters[i].size(), sumY / clusters[i].size()};

        hasChanged = hasChanged || centroids[i].x != newCentroid.x || centroids[i].y != newCentroid.y;

        centroids[i] = newCentroid;
    }

    return hasChanged;
}

void clusterize(int k)
{
    initCentroids(k);

    while (true)
    {
        assignToClusters();
        bool hasChanged = updateCentroid();
        if (!hasChanged)
        {
            break;
        }

        for (int i = 0; i < k; i++)
        {
            clusters[i].clear();
        }
    }
}

int main()
{
    int k;
    string fileName;
    cin >> fileName >> k;

    centroids.reserve(k);
    clusters.resize(k);

    ifstream file(fileName);
    string line;
    while (getline(file, line))
    {
        double x, y;
        stringstream sstream;
        sstream << line;
        sstream >> x >> y;
        points.push_back({x, y});
    }

    file.close();

    minX = points[0].x;
    maxX = minX;
    minY = points[0].y;
    maxY = minY;

    for (Point& point : points)
    {
        minX = minX > point.x ? point.x : minX;
        minY = minY > point.y ? point.y : minY;
        maxX = maxX < point.x ? point.x : maxX;
        maxY = maxY < point.y ? point.y : maxY;
    }



    return 0;
}
