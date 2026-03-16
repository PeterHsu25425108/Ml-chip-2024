#ifndef POOL_H
#define POOL_H
#include<iostream>
#include<vector>

using namespace std;

void MaxPool2d(vector<vector<vector<double>>> &input, vector<vector<vector<double>>> &output, int ker_wid, int stride)
{
    int in_ch = input.size();
    int in_wid = input[0].size();
    int out_wid = (in_wid - ker_wid) / stride + 1;

    for (int i = 0; i < in_ch; i++)
    {
        for (int j = 0; j < out_wid; j++)
        {
            for (int k = 0; k < out_wid; k++)
            {
                double max = input[i][j * stride][k * stride];
                for (int l = 0; l < ker_wid; l++)
                {
                    for (int m = 0; m < ker_wid; m++)
                    {
                        if (input[i][j * stride + l][k * stride + m] > max)
                        {
                            max = input[i][j * stride + l][k * stride + m];
                        }
                    }
                }
                output[i][j][k] = max;
            }
        }
    }
}

void AvgPool2d(vector<vector<vector<double>>> &input, vector<vector<vector<double>>> &output, int ker_wid, int stride)
{
    int in_ch = input.size();
    int in_wid = input[0].size();
    int out_wid = (in_wid - ker_wid) / stride + 1;

    for (int i = 0; i < in_ch; i++)
    {
        for (int j = 0; j < out_wid; j++)
        {
            for (int k = 0; k < out_wid; k++)
            {
                double sum = 0;
                for (int l = 0; l < ker_wid; l++)
                {
                    for (int m = 0; m < ker_wid; m++)
                    {
                        sum += input[i][j * stride + l][k * stride + m];
                    }
                }
                output[i][j][k] = sum / (ker_wid * ker_wid);
            }
        }
    }
}

#endif