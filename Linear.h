#ifndef LINEAR_H
#define LINEAR_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

SC_MODULE(Flatten)
{
    // input size = (6, 6, 256)
    // Flatten: Output size = (9216,)
    sc_in<double> ***in_data;
    sc_out<double> *out_data;

    void run()
    {
        int count = 0;
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                for (int k = 0; k < 6; k++)
                {
                    out_data[count].write(in_data[i][j][k].read());
                    count++;
                }
            }
        }
    }

    SC_CTOR(Flatten)
    {
        in_data = new sc_in<double> **[6];
        for (int i = 0; i < 6; i++)
        {
            in_data[i] = new sc_in<double> *[6];
            for (int j = 0; j < 6; j++)
            {
                in_data[i][j] = new sc_in<double>[256];
            }
        }
        out_data = new sc_out<double>[256 * 6 * 6];

        SC_METHOD(run);
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                for (int k = 0; k < 6; k++)
                {
                    sensitive << in_data[i][j][k];
                }
            }
        }
    }

    ~Flatten()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                delete[] in_data[i][j];
            }
            delete[] in_data[i];
        }
        delete[] in_data;
        delete[] out_data;
    }
};

SC_MODULE(Linear)
{
    const int in_size;
    const int out_size;
    string file_name;

    sc_in<double> *in_data;
    sc_out<double> *out_data;

    void run()
    {
        vector<vector<double>> weight;
        vector<double> bias;

        ifstream weight_file("data/" + file_name + ".txt");
        for (int i = 0; i < out_size; i++)
        {
            vector<double> temp;
            for (int j = 0; j < in_size; j++)
            {
                double w;
                weight_file >> w;
                temp.push_back(w);
            }
            weight.push_back(temp);
        }
        weight_file.close();

        ifstream bias_file("data/" + file_name + "_bias.txt");
        for (int i = 0; i < out_size; i++)
        {
            double b;
            bias_file >> b;
            bias.push_back(b);
        }
        bias_file.close();

        for (int i = 0; i < out_size; i++)
        {
            double sum = 0;
            for (int j = 0; j < in_size; j++)
            {
                sum += in_data[j].read() * weight[i][j];
            }
            out_data[i].write(sum + bias[i]);
        }
    }

    SC_CTOR(Linear) : in_size(0), out_size(0), file_name("")
    {
        in_data = NULL;
        out_data = NULL;
        /*SC_METHOD(run);
        for (int i = 0; i < in_size; i++)
        {
            sensitive << in_data[i];
        }*/
    }

    Linear(sc_module_name name, int in_size, int out_size, string file_name) : in_size(in_size), out_size(out_size), file_name(file_name)
    {
        in_data = new sc_in<double>[in_size];
        out_data = new sc_out<double>[out_size];
        SC_METHOD(run);
        for (int i = 0; i < in_size; i++)
        {
            sensitive << in_data[i];
        }
    }

    ~Linear()
    {
        delete[] in_data;
        delete[] out_data;
    }
};

#endif