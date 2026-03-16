#ifndef FC_H
#define FC_H
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void fc(vector<double> &fc_out, vector<double> &fc_in, string fc_file)
{
    vector<vector<double>> fc_w;
    vector<double> fc_b;

    ifstream fc_w_in("data/" + fc_file + "_weight.txt");
    ifstream fc_b_in("data/" + fc_file + "_bias.txt");

    for (int i = 0; i < fc_out.size(); i++)
    {
        vector<double> fc_w_row;
        for (int j = 0; j < fc_in.size(); j++)
        {
            double w;
            fc_w_in >> w;
            fc_w_row.push_back(w);
        }
        fc_w.push_back(fc_w_row);
    }

    for (int i = 0; i < fc_out.size(); i++)
    {
        double b;
        fc_b_in >> b;
        fc_b.push_back(b);
    }

    for (int i = 0; i < fc_out.size(); i++)
    {
        fc_out[i] = 0;
        for (int j = 0; j < fc_in.size(); j++)
        {
            fc_out[i] += fc_in[j] * fc_w[j][i];
        }
        fc_out[i] += fc_b[i];
    }
}

void flatten(vector<double> &fc_in, vector<vector<vector<double>>> &input)
{
    int in_ch = input.size();
    int in_wid = input[0].size();

    for (int i = 0; i < in_ch; i++)
    {
        for (int j = 0; j < in_wid; j++)
        {
            for (int k = 0; k < in_wid; k++)
            {
                fc_in.push_back(input[i][j][k]);
            }
        }
    }
}

#endif