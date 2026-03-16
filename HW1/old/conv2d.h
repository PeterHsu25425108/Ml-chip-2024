#ifndef CONV2D_H
#define CONV2D_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void readConvWeight(string filename, vector<vector<vector<vector<double>>>> &weight, int out_ch, int in_ch, int out_wid, int ker_wid)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: cannot open file " << filename << endl;
        return;
    }

    cout << "reading weight" << endl;

    for (int i = 0; i < out_ch; i++)
    {
        for (int j = 0; j < in_ch; j++)
        {
            for (int k = 0; k < ker_wid; k++)
            {
                for (int l = 0; l < ker_wid; l++)
                {
                    file >> weight[i][j][k][l];
                }
            }
        }
    }

    cout << "weight read" << endl;

    file.close();
}

void readConvBias(string filename, vector<double> &bias, int out_ch)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: cannot open file " << filename << endl;
        return;
    }

    for (int i = 0; i < out_ch; i++)
    {
        file >> bias[i];
    }

    file.close();
}

double InnerProduct(int in_ch, int zero_pad, int in_wid, int ker_wid, int stride, vector<vector<vector<double>>> &input, vector<vector<vector<double>>> &weight, int j, int k)
{
    int sum = 0;

    for (int i = 0; i < in_ch; i++)
    {
        for (int l = 0; l < ker_wid; l++)
        {
            for (int m = 0; m < ker_wid; m++)
            {
                if (j * stride + l < zero_pad || j * stride + l >= in_wid + zero_pad || k * stride + m < zero_pad || k * stride + m >= in_wid + zero_pad)
                {
                    continue;
                }
                sum += input[i][-zero_pad + j * stride + l][-zero_pad + k * stride + m] * weight[i][l][m];
            }
        }
    }
    return sum;
}

void Conv2d(int out_ch, int in_ch, int zero_pad, int in_wid, int out_wid, int ker_wid, int stride, vector<vector<vector<double>>> &input, vector<vector<vector<double>>> &output, string filename)
{
    vector<vector<vector<vector<double>>>> weight(out_ch, vector<vector<vector<double>>>(in_ch, vector<vector<double>>(ker_wid, vector<double>(ker_wid))));
    vector<double> bias(out_ch);

    readConvWeight("data/" + filename + "_weight.txt", weight, out_ch, in_ch, out_wid, ker_wid);
    readConvBias("data/" + filename + "_bias.txt", bias, out_ch);

    cout << "params read" << endl;

    for (int i = 0; i < out_ch; i++)
    {
        for (int j = 0; j < out_wid; j++)
        {
            for (int k = 0; k < out_wid; k++)
            {
                output[i][j][k] = InnerProduct(in_ch, zero_pad, in_wid, ker_wid, stride, input, weight[i], j, k);
            }
        }
    }

    cout << "conv applied" << endl;

    for (int i = 0; i < out_ch; i++)
    {
        for (int j = 0; j < out_wid; j++)
        {
            for (int k = 0; k < out_wid; k++)
            {
                output[i][j][k] += bias[i];
            }
        }
    }
}

#endif