#ifndef RE_H
#define RE_H
#include <iostream>
#include <vector>

using namespace std;

void ReLU(double &x)
{
    if (x < 0)
    {
        x = 0;
    }
}

void ReLU(vector<double> &x)
{
    for (int i = 0; i < x.size(); i++)
    {
        ReLU(x[i]);
    }
}

void ReLU(vector<vector<double>> &x)
{
    for (int i = 0; i < x.size(); i++)
    {
        ReLU(x[i]);
    }
}

void ReLU(vector<vector<vector<double>>> &x)
{
    vector<vector<vector<double>>> result;
    for (int i = 0; i < x.size(); i++)
    {
        ReLU(x[i]);
    }
}

#endif