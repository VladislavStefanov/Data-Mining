#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

unordered_set<string> possibleValues[10];
const int MIN_SAMPLE_SIZE = 30;

struct DataSet
{
    vector<vector<string>> data;
    unordered_map<string, int> frequencyTable[2][10];
    int setSize;
    int recCount;
    int noRecCount;

    DataSet(vector<vector<string>> data)
    {
        this->data = data;

        for(int i = 0; i < data.size(); i++)
        {
            bool isRec = data[i][0][0] == 'r';
            isRec ? recCount++ : noRecCount++;
            for (int j = 1; j < data[i].size(); j++)
            {
                frequencyTable[isRec][j][data[i][j]]++;
            }
        }

        setSize = recCount + noRecCount;
    }

    double entropy (double a, double b)
    {
        if (a == 0 || b == 0)
        {
            return 0;
        }

        double pa = a / (a + b);
        double pb = b / (a + b);

        return - (pa * log2(pa) + pb * log2(pb));
    }

    double entropy(int attributeIndex)
    {
        double entr = 0;
        for (auto iter = frequencyTable[0][attributeIndex].begin(); iter != frequencyTable[0][attributeIndex].end(); iter++)
        {
            string attributeValue = iter->first;
            double countInNoRec = iter->second;
            double countInRec = frequencyTable[1][attributeIndex][attributeValue];
            entr += (countInRec + countInNoRec) / setSize * entropy(countInNoRec, countInRec);
        }

        return entr;
    }

    double infoGain(int attributeIndex)
    {
        return entropy(recCount, noRecCount) - entropy(attributeIndex);
    }

    int chooseBestAttribute(unordered_set<int> chosenAttributes)
    {
        double maxInfoGain = -100000;
        int bestAttributeIndex = 1;

        for (int currentAttributeIndex = 1; currentAttributeIndex < 10 ; currentAttributeIndex++)
        {
            if (chosenAttributes.count(currentAttributeIndex) == 0)
            {
                double currentInfoGain = infoGain(currentAttributeIndex);
                if (currentInfoGain > maxInfoGain)
                {
                    maxInfoGain = currentInfoGain;
                    bestAttributeIndex = currentAttributeIndex;
                }
            }
        }

        return bestAttributeIndex;
    }

    DataSet* subSet(int attributeIndex, string value)
    {
        vector<vector<string>> newData;

        for (int i = 0; i < data.size(); i++)
        {
            if(value.compare(data[i][attributeIndex]) == 0)
            {
                newData.push_back(data[i]);
            }
        }

        DataSet* subSet = new DataSet(newData);

        return subSet;
    }
};

struct Node
{
    bool isRec;
    bool isLeaf = false;

    int attributeIndex;
    string value;

	vector<Node*> children;
    unordered_set<int> chosenAttributes;
};

struct Classifier
{
    Node* root;

    Classifier(DataSet* dataSet)
    {
        buildNode(nullptr, dataSet);
    }

    void buildNode(Node* node, DataSet* dataSet)
    {
        Node* child;

        if (node == nullptr)
        {
            root = new Node();
            child = root;
        }
        else
        {
            child = new Node();
            child->chosenAttributes = node->chosenAttributes;
            node->children.push_back(child);
        }

        int bestAttributeIndex = dataSet->chooseBestAttribute(child->chosenAttributes);
        child->attributeIndex = bestAttributeIndex;
        child->chosenAttributes.insert(bestAttributeIndex);
        child->isRec = dataSet->recCount == dataSet->noRecCount ? node->isRec : dataSet->recCount > dataSet->noRecCount;

        unordered_set<string> possibleValuesForBestAttribute = possibleValues[bestAttributeIndex];
        for (auto iter = possibleValuesForBestAttribute.begin(); iter != possibleValuesForBestAttribute.end(); iter++)
        {
            string value = *iter;

            if (dataSet->entropy(dataSet->noRecCount, dataSet->recCount) == 0 || child->chosenAttributes.size() >= 9 || dataSet->data.size() < MIN_SAMPLE_SIZE)
            {
                Node* leaf = new Node();
                leaf->value = value;
                leaf->isLeaf = true;
                leaf->isRec = dataSet->recCount == dataSet->noRecCount ? child->isRec : dataSet->recCount > dataSet->noRecCount;
                child->children.push_back(leaf);
                continue;
            }

            Node* valueNode = new Node();
            valueNode->attributeIndex = bestAttributeIndex;
            valueNode->value = value;
            valueNode->chosenAttributes = child->chosenAttributes;
            valueNode->isRec = dataSet->recCount == dataSet->noRecCount ? child->isRec : dataSet->recCount > dataSet->noRecCount;
            child->children.push_back(valueNode);
            DataSet* subSet = dataSet->subSet(bestAttributeIndex, value);
            buildNode(valueNode, subSet);

            delete subSet;
        }
    }

    bool isAccurate(vector<string> line)
    {
        int attribute;
        string value;
        Node* node = root;

        while(true)
        {
            attribute = node->attributeIndex;
            value = line[attribute];

            for (int i = 0; i < node->children.size(); i++)
            {
                if(value.compare(node->children[i]->value) == 0)
                {
                    if (node->children[i]->isLeaf)
                    {
                        return (node->children[i]->isRec) ? line[0][0] == 'r' : line[0][0] == 'n' ;
                    }

                    node = node->children[i]->children[0];
                    break;
                }
            }
        }
    }
};

double calculateAccuracy(vector<vector<string>> trainData, vector<vector<string>>testData)
{
    DataSet* dataSet = new DataSet(trainData);

    Classifier classifier(dataSet);

    int successCount = 0;

    for (int i = 0 ; i < testData.size(); i++)
    {
        successCount += classifier.isAccurate(testData[i]);
    }

    delete dataSet;

    return (successCount * 100.0) / testData.size();
}

int main()
{
    string line, attribute;
    ifstream filein("breast-cancer.data", ios::in);

    vector<vector<string>> data;

    while(getline(filein, line)){
        int counter = 0;
        stringstream ss(line);
        vector<string> attributes;
        while(getline(ss, attribute, ','))
        {
            attributes.push_back(attribute);
            possibleValues[counter].insert(attribute);
            counter ++;
        }
        data.push_back(attributes);
    }

    srand(time(0));
    random_shuffle(data.begin(), data.end());

    filein.close();

    int testSize = data.size() / 10;
    int testInit;

    double accuraciesSum = 0;

    vector<vector<string>>trainData;
    vector<vector<string>>testData;

    for (int j = 0; j < 10; j++)
    {
        testInit = j * testSize;

        for (int i = 0 ; i < data.size(); i++)
        {
            if (i >= testInit && i < testInit + testSize)
            {
                testData.push_back(data[i]);
            }
            else
            {
                trainData.push_back(data[i]);
            }
        }

        double accuracy = calculateAccuracy(trainData, testData);

        cout << accuracy << '%' << endl;
        accuraciesSum += accuracy;

        trainData.clear();
        testData.clear();
    }

    cout << "-------------" << endl;
    cout << accuraciesSum / 10 << '%' << endl;

    return 0;
}
