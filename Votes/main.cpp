#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>

using namespace std;

vector<string> lines;

int democratsData[3][16];
int republicansData[3][16];

double countDemocrats = 0;
double countRepublicans = 0;

const int initPositionDemocrats = 9;
const int initPositionRepublicans = 11;

void updateData(string line, int updateValue)
{
    bool isRepublican = line[0] == 'r';

    isRepublican ? countRepublicans += updateValue : countDemocrats += updateValue;

    int initPosition = isRepublican ? initPositionRepublicans : initPositionDemocrats;
    int counter = 0;
    for (int i = initPosition; i < line.size(); i += 2)
    {
       switch (line[i])
       {
            case 'y':
               isRepublican ? republicansData[0][counter] += updateValue : democratsData[0][counter] += updateValue;
               break;
            case 'n':
               isRepublican ? republicansData[1][counter] += updateValue : democratsData[1][counter] += updateValue;
               break;
            case '?':
               isRepublican ? republicansData[2][counter] += updateValue : democratsData[2][counter] += updateValue;
               break;
       }
       counter++;
    }
}

bool isLinePredictedCorrectly(string line)
{
    bool isRepublican = line[0] == 'r';
    int initPosition = isRepublican ? initPositionRepublicans : initPositionDemocrats;
    double isRepublicanProbability = 1;
    double isDemocratProbability = 1;

    int attributeIndex = 0;

    for (int i = initPosition; i < line.size(); i += 2)
    {
        switch (line[i])
            {
            case 'y':
               isRepublicanProbability *= republicansData[0][attributeIndex] / countRepublicans;
               isDemocratProbability *= democratsData[0][attributeIndex] / countDemocrats;
               break;
            case 'n':
               isRepublicanProbability *= republicansData[1][attributeIndex] / countRepublicans;
               isDemocratProbability *= democratsData[1][attributeIndex] / countDemocrats;
               break;
            case '?':
               isRepublicanProbability *= republicansData[2][attributeIndex] / countRepublicans;
               isDemocratProbability *= democratsData[2][attributeIndex] / countDemocrats;
               break;
            }
        attributeIndex++;
    }

    isRepublicanProbability *= countRepublicans / (countRepublicans + countDemocrats);
    isDemocratProbability *= countDemocrats / (countRepublicans + countDemocrats);

    return isRepublican ^ (isRepublicanProbability <= isDemocratProbability);
}

double calculateAccuracy(int testInitLineIndex, int testSize)
{
    int correctPredictions = 0;

    for (int i = testInitLineIndex; i < testInitLineIndex + testSize; i++)
    {
        correctPredictions += isLinePredictedCorrectly(lines[i]);
    }

    return correctPredictions * 100.0 / testSize;
}

int main()
{
    ifstream fileWithData("house-votes-84.data", ios::in);
    string line;

    while (getline(fileWithData, line))
    {
        lines.push_back(line);
        updateData(line, 1);
    }

    fileWithData.close();

    int testSize = lines.size()/10;
    int testInitLineIndex;
    double accuraciesSum = 0;
    double accuracy;

    for (int j = 0; j < 10; j++)
    {
        testInitLineIndex = j * testSize;

        for (int i = testInitLineIndex; i < testInitLineIndex + testSize; i++)
        {
            if (testInitLineIndex != 0)
            {
                updateData(lines[i - testSize], 1);
            }

            updateData(lines[i], -1);
        }

        accuracy = calculateAccuracy(testInitLineIndex, testSize);
        cout << accuracy << "%" << endl;
        accuraciesSum += accuracy;
    }

    cout << endl;
    cout << accuraciesSum / 10 << "%" << endl;
}
