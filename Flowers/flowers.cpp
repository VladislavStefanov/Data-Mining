#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long n, d, l;
long long minDropsCount = -1;

void findMinDropsCount(vector<long long> &drops, vector<long long> &originalDrops, long long left, long long right)
{
    if(left <= right)
    {
        long long middle = (right + left) / 2;

        sort(drops.begin(), drops.begin() + middle);
        bool isWholeGardenWatered = true;

        if(drops[0] > d)
        {
            isWholeGardenWatered = false;
        }
        if(l - drops[middle - 1] > d)
        {
            isWholeGardenWatered = false;
        }
        if(isWholeGardenWatered)
        {
            for(long long i = 1; i < middle; i++)
            {
                if(drops[i] - drops[i - 1] > d)
                {
                    isWholeGardenWatered = false;
                    break;
                }
            }
        }

        if(isWholeGardenWatered)
        {
            minDropsCount = middle;
            drops = originalDrops;
            findMinDropsCount(drops, originalDrops, left, middle - 1);
        } else {
            findMinDropsCount(drops, originalDrops, middle + 1, right);
        }
    }
}

int main()
{
    long long x, a, b;
    cin >> n >> l >> d >> x >> a >> b;

    vector<long long> drops;
    vector<long long> originalDrops;

    drops.reserve(n);
    originalDrops.reserve(n);

    for(long long i = 0; i < n ; i++)
    {
        drops.push_back(x);
        originalDrops.push_back(x);
        x = (x * a + b) % (l + 1);
    }

    findMinDropsCount(drops, originalDrops, 1, n);

    cout << minDropsCount;
    return 0;
}
