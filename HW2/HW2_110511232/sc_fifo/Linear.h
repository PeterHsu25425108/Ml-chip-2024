#ifndef LINEAR_H
#define LINEAR_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"
#include "DEBUG.h"

SC_MODULE(Flatten)
{
    // input size = (6, 6, 256)
    // Flatten: Output size = (9216,)
    sc_port<sc_fifo_in_if<double>> ***in_data;
    sc_port<sc_fifo_out_if<double>> *out_data;
    sc_in_clk iclk;

    void run()
    {
        while (true)
        {
            if(DEBUG) cout << "run flatten" << endl;

            int count = 0;
            for (int i = 0; i < 256; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    for (int k = 0; k < 6; k++)
                    {
                        double temp = -1;
                        in_data[i][j][k]->read(temp);
                        out_data[count]->write(temp);
                        count++;
                    }
                }
            }
            
            if(DEBUG) cout << "write flatten" << endl;

            wait();
        }
    }

    SC_CTOR(Flatten)
    {
        // cout << "constructor for Flatten" << endl;
        in_data = new sc_port<sc_fifo_in_if<double>> **[256];
        for (int i = 0; i < 256; i++)
        {
            in_data[i] = new sc_port<sc_fifo_in_if<double>> *[6];
            for (int j = 0; j < 6; j++)
            {
                in_data[i][j] = new sc_port<sc_fifo_in_if<double>>[6];
            }
        }
        out_data = new sc_port<sc_fifo_out_if<double>>[256 * 6 * 6];

        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
    }

    ~Flatten()
    {
        for (int i = 0; i < 256; i++)
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

    sc_port<sc_fifo_in_if<double>> *in_data;
    sc_port<sc_fifo_out_if<double>> *out_data;

    vector<vector<double>> weight;
    vector<double> bias;
    sc_in_clk iclk;

    void run()
    {
        while (true)
        {
            if(DEBUG) cout << "run "<< file_name << endl;
            // a vector to store input data
            vector<double> in_array(in_size, 0);
            for (int i = 0; i < in_size; i++)
            {
                double temp;
                in_data[i]->read(temp);
                in_array[i] = temp;
            }

            for (int i = 0; i < out_size; i++)
            {
                double sum = 0;
                for (int j = 0; j < in_size; j++)
                {
                    double temp;
                    sum += in_array[j] * weight[i][j];
                }
                out_data[i]->write(sum + bias[i]);
            }

            if(DEBUG) cout << "write " << file_name << endl;
            wait();
        }
    }

    SC_CTOR(Linear) : in_size(0), out_size(0), file_name("")
    {
        in_data = NULL;
        out_data = NULL;
    }

    Linear(sc_module_name name, int in_size, int out_size, string file_name) : in_size(in_size), out_size(out_size), file_name(file_name)
    {
        in_data = new sc_port<sc_fifo_in_if<double>>[in_size];
        out_data = new sc_port<sc_fifo_out_if<double>>[out_size];

        ifstream weight_file("../../data/" + file_name + "_weight.txt");
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

        ifstream bias_file("../../data/" + file_name + "_bias.txt");
        for (int i = 0; i < out_size; i++)
        {
            double b;
            bias_file >> b;
            bias.push_back(b);
        }
        bias_file.close();

        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
    }

    ~Linear()
    {
        delete[] in_data;
        delete[] out_data;
    }
};

#endif